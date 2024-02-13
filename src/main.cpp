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
*  2. Add Different Blocks
*  3. Include vertical loading
*  4. Cull Chunks Edges
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
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 860;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void updateChunks(int *numVerts, Voxel *voxel, Chunk* chunk);
void setCurrentChunk();

struct vec3 cameraPos = {32.0f, 33.0f, 32.0f};
int currentChunk[3] = {0, 0, 0};
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
    "#version 330 core\n"
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
    "#version 330 core\n"
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

int main() {
    
    Voxel* UVoxi = new Voxel();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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


    Shader shaderProgram = shader_create(vertexShaderSource, fragmentShaderSouce);

    unsigned int textureGrass;
    const char* grassFile = "../../src/GrassAtlas.png";
    shader_addTexture(grassFile, &textureGrass);
    unsigned int textureDirt;
    const char* dirtFile = "../../src/DirtAtlas.png";
    shader_addTexture(dirtFile, &textureDirt);

    
    int numVerts = 0;
    Chunk* chunk = new Chunk;
    chunk->chunk.resize(C_chunkSize* C_chunkSize* C_chunkSize, {0,0,0,0});

    unsigned int chunkVBO, chunkVAO;
    glGenVertexArrays(1, &chunkVAO);
    glGenBuffers(1, &chunkVBO);

    struct mat4 projectionMat = makeProjectionMatrix(110.0f, 0.1f, 100.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);
    
    int prevChunkX = 1000000;
    int prevChunkY = 1000000;
    int prevChunkZ = 1000000;
    unsigned int maxLoadSize = 40000000 * sizeof(float);
    
    shader_ArrBuffs(chunkVAO, chunkVBO, nullptr, maxLoadSize);
    printf("Init Done!");

    
    glBindTexture(GL_TEXTURE_2D, textureGrass);

    while (!glfwWindowShouldClose(window)) {
      
      
      processInput(window);
      
      glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);
      glCullFace(GL_FRONT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (lineDraw == 1) {
        
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
      
      

      lookVec = addVectors(cameraPos, cameraFront);
      struct mat4 viewMat = makeLookAtMatrix(cameraPos, lookVec, yAxis);

      int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_TRUE, (GLfloat*)&viewMat.m);
      int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
      glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, (GLfloat*)&projectionMat.m);

      struct mat4 modelMatrix = makeIdentityMatrix();

      int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (GLfloat*)&modelMatrix.m);

      int camLoc = glGetUniformLocation(shaderProgram.ID, "cameraPos");
      glUniform3fv(camLoc, 1, (GLfloat*)&cameraPos);
      
      shader_use(&shaderProgram);

      setCurrentChunk();

      if ((prevChunkX != currentChunk[0]) || (prevChunkY != currentChunk[1]) || (prevChunkZ != currentChunk[2])) {
        if (chunk != nullptr) {
          
          float Time;
          startTimer(&Time);
          updateChunks(&numVerts, UVoxi, chunk);
          int numTris = (numVerts * 0.6f) / 3;
          printf("\n%d\n", numTris);
          endTimer(&Time);
        }
      }
      
      glDrawArrays(GL_TRIANGLES, 0, numVerts/5);


      glfwSwapInterval(1);
      glfwSwapBuffers(window);
      glfwPollEvents();
      
      

      prevChunkX = currentChunk[0];
      prevChunkY = currentChunk[1];
      prevChunkZ = currentChunk[2];

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame; 
    }
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

void updateChunks(int *numVerts, Voxel *voxel, Chunk* chunk) {
  
  setCurrentChunk();
  int flatIndex = 0;
  std::vector<float> allData;
  allData.reserve(6000000);
  int chunkInfo[49] = {0};
  int worldSize = 7;
  int totalLoadedSize = 0;
  for (int i = 0; i < worldSize; i++) {
    for (int j = 0; j < worldSize; j++) {
      
      if (i == 0) {
        flatIndex = j;
      }
      else { flatIndex = i * worldSize + j; }
      
      
      chunk->offsetX = j + currentChunk[0] - (worldSize / 2);
      chunk->offsetY = 0;
      chunk->offsetZ = i + currentChunk[2] - (worldSize / 2);

      
      if (( (std::abs(chunk->offsetX) - std::abs(currentChunk[0])) > (worldSize / 2) ) 
      && (std::abs(chunk->offsetZ) - std::abs(currentChunk[2]) > (worldSize / 2))) {
        printf("Skipped\n");
        continue;
      }
      
      
      generateChunk(C_chunkSize, chunk, chunk->offsetX, chunk->offsetZ, chunk->offsetY, seed);
      
      
      
      cullChunk(&allData, *chunk, C_chunkSize, voxel);
      *numVerts = allData.size();
      
      chunkInfo[flatIndex] = totalLoadedSize;
      
      totalLoadedSize += *numVerts;
      
      glBufferSubData(GL_ARRAY_BUFFER, chunkInfo[flatIndex] * sizeof(float), allData.size() * sizeof(float), &allData[0]);
      
      allData.clear();
      
      std::fill(chunk->chunk.begin(), chunk->chunk.end(), Ivec4{0, 0, 0, 0});
      
      
    }
  }
  *numVerts = totalLoadedSize;
}

void setCurrentChunk() {
  currentChunk[0] = ((int)(cameraPos.x * bsConvert) / C_chunkSize);
  if (cameraPos.x < 0) {
    currentChunk[0] -=1;
  }
  currentChunk[1] = ((int)(cameraPos.y * bsConvert) / C_chunkSize);
  if (cameraPos.y < 0) {
    currentChunk[1] -=1;
  }
  currentChunk[2] = ((int)(cameraPos.z * bsConvert ) / C_chunkSize);
  if (cameraPos.z < 0) {
    currentChunk[2] -=1;
  }
}