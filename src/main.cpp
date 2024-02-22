/*
*  Date Started 12/21/2023
*  Date Last Modified 12/31/2023
*  Hours Worked ~22.5
*
*  TODO:
*  1. Optimize Chunk Loading
*     - Load only new chunks
*     - Load Chunks one at a time?
*     - Reduce nested loops
*  2. Fix why nothing is being output from compute shaders
*  3. Remove All teh unnessary letfover boiler plate in ChunkManager functions
*  4. Abstract away more boiler plate
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Test/TimeTest.h"

#include "Constants.h"
const int C_chunkSize = 128;
const float C_blockSize = 0.25f;
const int bsConvert = 2;

#include "GMath/GMath.h" // math before voxel
#include "Voxel/Voxel.h"
#include "Shader/Shader.h"
#include "WorldGen/WorldGen.h"

#include <vector>
#include <stdio.h>

#include "stb_image.h"

const unsigned int seed = seedGen();

int lineDraw = -1;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void updateChunks(int *numVerts, Voxel *voxel, Chunk* chunk, unsigned int outBuff, unsigned int inBuff, int *chunkPtr, int *cullPtr, Shader *chunkShader, Shader *cullShader);
void setCurrentChunk();

struct vec3 cameraPos = {32.0f, 33.0f, 32.0f};
struct Ivec3 currentChunk = {0, 0, 0};
struct vec3 cameraFront = {0.0f, 0.0f, -1.0f};
struct vec3 lookVec;
struct vec3 xAxis = {1.0f, 0.0f, 0.0f};
struct vec3 yAxis = {0.0f, 1.0f, 0.0f};
struct vec3 zAxis = {0.0f, 0.0f, 1.0f};

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  SCR_WIDTH / 2.0;
float lastY =  SCR_HEIGHT / 2.0;
float fov   =  45.0f;

float currentTime;
float elapsedTime;
float lastTime;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const char *vertexShaderSource = 
    "#version 440 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 texCoord;\n"
    "out vec2 TexCoord;\n"
    "out vec3 FragPos;\n"
    "out vec3 CameraPos;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform vec3 cameraPos;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   CameraPos = cameraPos;\n"
    "   vec4 WorldPos = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "   FragPos = vec3(WorldPos);\n"
    "   gl_Position = projection * view * WorldPos;\n"
    "   TexCoord = vec2(texCoord.x, texCoord.y);\n"
    "}\0";
const char *fragmentShaderSouce = 
    "#version 440 core\n"
    "in vec2 TexCoord;\n"
    "in vec3 FragPos;\n"
    "in vec3 CameraPos;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "    float distFromCam = ((abs(CameraPos.z - FragPos.z) + abs(CameraPos.x - FragPos.x)) / 2.0);\n"
    "    vec4 TexColor = texture(ourTexture, TexCoord);\n"
    "    float atmosphereHeight = 68.0;\n"
    "    if (distFromCam > 100) {\n;"
    "       float offsetDistance = distFromCam - 120;\n"
    "       float rayleighCoefficient = 0.25 * exp(abs(offsetDistance) / atmosphereHeight);\n"
    "       vec3 scatteringColor = vec3(0.4, 0.5, 0.9);\n"
    "       vec3 scatteredLight = TexColor.rgb + rayleighCoefficient * scatteringColor;\n"
    "       FragColor = vec4(scatteredLight, TexColor.a);\n"
    "    }\n"
    "    else { FragColor = TexColor; }\n"
    "}\n";

const char *noiseComputeShader = 
    "#version 430\n"
    "int noise3(int x, int y, int z) {\n"
    "   return (x + y) / 2;;\n"
    "}\n"
    "\n"
    "layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;\n"
    "// Input and output buffer definitions\n"
    "layout(std430, binding = 0) buffer InputBuffer {\n"
    "    // Input data for each block (e.g., block properties)\n"
    "    int data[];\n"
    "};\n"
    "\n"
    "layout(std430, binding = 1) buffer OutputBuffer {\n"
    "    // Output data for each block (e.g., computation result)\n"
    "    int result[];\n"
    "};\n"
    "\n"
    "void main() {\n"
    "    // Calculate the global block index in the chunk\n"
    "    ivec3 globalBlockID = ivec3(gl_GlobalInvocationID.xyz);\n"
    "\n"
    "    // Convert global block index to buffer index\n"
    "    int flatIndex = (globalBlockID.x * 128) + (globalBlockID.z * 128 * 128) + globalBlockID.y;\n"
    "    result[flatIndex] = noise3(globalBlockID.x, globalBlockID.z, globalBlockID.y);"
    "\n"
    "\n"
    "}\n";
const char* cullComputeShader = 
    "#version 430\n"
    "\n"
    "#define C_chunkSize 128\n"
    "#define isT_Edge(x) (x == C_chunkSize - 1)\n"
    "#define isB_Edge(x) (x == 0)\n"
    "\n"
    "#define T_Neighbor(x, y) (!(isT_Edge(x)) ? int(isB_Edge(y)) : 0) \n"
    "#define B_Neighbor(x, y) (!(isB_Edge(x)) ? int(isB_Edge(y)) : 0)\n"
    "\n"
    "    layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;\n"
    "    // Input and output buffer definitions\n"
    "    layout(std430, binding = 0) buffer InputBuffer {\n"
    "        // Input data for each block (e.g., block properties)\n"
    "        int data[];\n"
    "    };\n"
    "    \n"
    "    layout(std430, binding = 1) buffer OutputBuffer {\n"
    "        // Output data for each block (e.g., computation result)\n"
    "        int result[];\n"
    "    };\n\n"
    "    \n"
    "    void main() {\n"
    "        // Calculate the global block index in the chunk\n"
    "        ivec3 globalBlockID = ivec3(gl_GlobalInvocationID.xyz);\n"
    "       int Bx = globalBlockID.x;\n"
    "       int By = globalBlockID.y;\n"
    "       int Bz = globalBlockID.z;\n"
    "        // Convert global block index to buffer index\n"
    "        int flatIndex = (globalBlockID.x * 128) + (globalBlockID.z * 128 * 128) + globalBlockID.y;\n"
    "       int Xp = flatIndex - C_chunkSize;\n"
    "        int Xn = flatIndex + C_chunkSize;\n"
    "        int Zp = flatIndex - (C_chunkSize*C_chunkSize);\n"
    "        int Zn = flatIndex + (C_chunkSize*C_chunkSize);\n"
    "        int Yp = flatIndex - 1;\n"
    "        int Yn = flatIndex + 1;\n"
    "    \n"
    "        int edgeMask = (int(isT_Edge(By)) << 5) | (int(isB_Edge(By)) << 4) | (int(isB_Edge(Bz)) << 3) \\\n"
    "                         | (int(isT_Edge(Bz)) << 2) | (int(isT_Edge(Bx)) << 1) | (int(isB_Edge(Bx)));\n"
    "        int neighborMask = (int(T_Neighbor(By, data[Yn])) << 5) | (int(B_Neighbor(By, data[Yp])) << 4) \\\n"
    "                         | (int(B_Neighbor(Bz, data[Xp])) << 3) | (int(T_Neighbor(Bz, data[Xn])) << 2) \\\n"
    "                         | (int(T_Neighbor(Bx, data[Zn])) << 1) | int(B_Neighbor(Bx, data[Zp]));\n"
    "       int notEdgeMask = edgeMask ^ 63;\n"
    "       int cmp = (notEdgeMask & neighborMask);\n" 
    "       int pushMask = edgeMask | cmp;\n"
    "       result[flatIndex] = pushMask;\n"
    "    }\n";


const char* depthVertexShaderSource = R"(
    #version 440 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 texCoord;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main()
    {
      vec4 WorldPos = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
      gl_Position = projection * view * WorldPos;
    }
)";

// Depth pre-pass fragment shader code
const char* depthFragmentShaderSource = R"(
    #version 440 core
    
    out float FragDepth;
    void main()
    {
      // Output depth value
      FragDepth = gl_FragCoord.z;
    }
)";
int main() {
    GLint maxCount;
    

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glfwSetInputMode(window, GLFW_REPEAT, GLFW_FALSE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    } 
    if (GL_ARB_shader_storage_buffer_object) {
        std::cout << "ARB_shader_storage_buffer_object extension is supported." << std::endl;
    } else {
        std::cerr << "ARB_shader_storage_buffer_object extension is not supported." << std::endl;
        return -1;
    }


    Shader shaderProgram = shader_create(vertexShaderSource, fragmentShaderSouce);
    Shader Cshader = Cshader_create(noiseComputeShader);
    unsigned int inBuff;
    unsigned int outBuff;
    glGenBuffers(1, &outBuff);
    glGenBuffers(1, &inBuff);
    /*GLint *chunkPtr = nullptr;
    bindComputeBuffs(inBuff, outBuff, (C_chunkSize * C_chunkSize * C_chunkSize), chunkPtr);
    Cshader.inBuff = &inBuff;
    Cshader.outBuff = &outBuff;
    Cshader.outPtr = chunkPtr;
    CHECK_GL_ERROR();
    shader_use(&Cshader);
    CHECK_GL_ERROR();
    Shader Cullshader = Cshader_create(cullComputeShader);
    Cullshader.inBuff = &inBuff;
    Cullshader.outBuff = &outBuff;
    Cullshader.outPtr = chunkPtr;
    //;*/
    Shader depthShader = shader_create(depthVertexShaderSource, depthFragmentShaderSource);
    
    
    CHECK_GL_ERROR();
    
    unsigned int textureGrass;
    const char* grassFile = "../../src/Game16x16.png";
    shader_addTexture(grassFile, &textureGrass);
    unsigned int textureDirt;
    const char* dirtFile = "../../src/DirtAtlas.png";
    shader_addTexture(dirtFile, &textureDirt);

    
    int numVerts = 0;
    Chunk* chunk = new Chunk;
    chunk->chunk.resize(C_chunkSize* C_chunkSize* C_chunkSize, 0);

    unsigned int chunkVBO, chunkVAO;
    glGenVertexArrays(1, &chunkVAO);
    glGenBuffers(1, &chunkVBO);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // Handle the error
        std::cout << error << std::endl;
    }

    struct mat4 projectionMat = makeProjectionMatrix(110.0f, 0.1f, 100.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);
    
    int prevChunkX = 1000000;
    int prevChunkY = 1000000;
    int prevChunkZ = 1000000;
    unsigned int maxLoadSize = 500000000 * sizeof(float);
    
    shader_ArrBuffs(chunkVAO, chunkVBO, nullptr, maxLoadSize);
    
    printf("Init Done!");

    
    glBindTexture(GL_TEXTURE_2D, textureGrass);

    
    Voxel* UVoxi = new Voxel();
    ChunkManager ChunkManager(UVoxi, chunk, &Cshader, &Cshader);
    ChunkManager.init_chunk_manager();



    while (!glfwWindowShouldClose(window)) {
      
      processInput(window);

      
      ChunkManager.updateState(currentChunk, cameraPos);
      
      glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
      //glEnable(GL_DEPTH_TEST);
      glDepthMask(GL_TRUE);
        
      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);
      glCullFace(GL_FRONT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (lineDraw == 1) {
        
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

      CHECK_GL_ERROR();
      lookVec = addVectors(cameraPos, cameraFront);
      struct mat4 viewMat = makeLookAtMatrix(cameraPos, lookVec, yAxis);
      
      shader_use(&shaderProgram);
      int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_TRUE, (GLfloat*)&viewMat.m);
      int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
      glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, (GLfloat*)&projectionMat.m);

      struct mat4 modelMatrix = makeIdentityMatrix();

      int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (GLfloat*)&modelMatrix.m);
      int camLoc = glGetUniformLocation(shaderProgram.ID, "cameraPos");
      glUniform3fv(camLoc, 1, (GLfloat*)&cameraPos);

      shader_use(&depthShader);
      int modelLoc2 = glGetUniformLocation(depthShader.ID, "model");
      glUniformMatrix4fv(modelLoc2, 1, GL_TRUE, (GLfloat*)&modelMatrix.m);
      int viewLoc2 = glGetUniformLocation(depthShader.ID, "view");
      glUniformMatrix4fv(viewLoc2, 1, GL_TRUE, (GLfloat*)&viewMat.m);
      int projectionLoc2 = glGetUniformLocation(depthShader.ID, "projection");
      glUniformMatrix4fv(projectionLoc2, 1, GL_TRUE, (GLfloat*)&projectionMat.m);

      ChunkManager.setCurrentChunk();
      currentChunk = ChunkManager.getCurrenChunk();
      cameraPos = ChunkManager.getCameraPos();

      if ((prevChunkX != currentChunk.x) || (prevChunkY != currentChunk.y) || (prevChunkZ != currentChunk.z)) {
        if (chunk != nullptr) {
          ChunkManager.signal_load();
        }
      }
      if (ChunkManager.get_is_chunksloaded()) {
        
        int size = ChunkManager.get_world_data().size();
        glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
        float* mappedPtr = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, size * sizeof(float), GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_WRITE_BIT);
        
        if (mappedPtr) {
          
          double Time;
          startTimer(&Time);
          memcpy(mappedPtr, ChunkManager.get_world_data().data(), size * sizeof(float));
          endTimer(&Time);
          glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, size * sizeof(float));
          glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        
        numVerts = ChunkManager.getNumVerts();
        ChunkManager.clear_world_data();
        ChunkManager.set_is_chunks_loaded();
      }
      
      /*
      shader_use(&depthShader);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glColorMask(0,0,0,0);
      glDrawArrays(GL_TRIANGLES, 0, numVerts/5);
      */
      shader_use(&shaderProgram);
      glDepthFunc(GL_LEQUAL);
      glColorMask(1,1,1,1);
      glDepthMask(GL_TRUE);
      glDrawArrays(GL_TRIANGLES, 0, numVerts/5);

      
      glfwSwapInterval(0);
      glfwSwapBuffers(window);
      glfwPollEvents();
      

      prevChunkX = currentChunk.x;
      prevChunkY = currentChunk.y;
      prevChunkZ = currentChunk.z;

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame; 
    }
    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    delete chunk;
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &chunkVBO);
    glDeleteVertexArrays(1, &chunkVAO);
    glDeleteProgram((GLuint)shaderProgram.ID);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  float cameraSpeed = 10.0f * deltaTime;
  struct vec3 vecA;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vecA = VscalarMulitply(cameraSpeed, cameraFront);
    cameraPos = addVectors(cameraPos, vecA);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vecA = VscalarMulitply(cameraSpeed, cameraFront);
    cameraPos = subtractVectors(cameraPos, vecA);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    vecA = crossProduct(cameraFront, yAxis);
    normalize3d(&vecA);
    vecA = VscalarMulitply(cameraSpeed, vecA);
    cameraPos = subtractVectors(cameraPos, vecA);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    vecA = crossProduct(cameraFront, yAxis);
    normalize3d(&vecA);
    vecA = VscalarMulitply(cameraSpeed, vecA);
    cameraPos = addVectors(cameraPos, vecA);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    
  }
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    lineDraw *= -1;
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse)
  {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
  }
  
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; 
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
      pitch = 89.0f;
  if(pitch < -89.0f)
      pitch = -89.0f;

  struct vec3 direction;
  direction.x = cos(yaw * (3.14159 / 180)) * cos(pitch * (3.14159 / 180));
  direction.y = sin(pitch * (3.14159 / 180));
  direction.z = sin(yaw * (3.14159 / 180)) * cos(pitch * (3.14159 / 180));
  normalize3d(&direction);
  cameraFront = direction;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

