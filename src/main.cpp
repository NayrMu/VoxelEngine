/*
*  Date Started 12/21/2023
*  Date Last Modified 12/24/2023
*  Hours Worked ~11.5
*
*  TODO:
*  4. Implement own face culling
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Constants.h"
#include "GMath/GMath.h" // math before voxel
#include "Voxel/Voxel.h"
#include "Shader/Shader.h"
#include "WorldGen/WorldGen.h"

#include <vector>
#include <stdio.h>

#include "stb_image.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

struct vec3 cameraPos = {0.0f, 0.0f, 3.0f};
struct vec3 cameraFront = {0.0f, 0.0f, -1.0f};
struct vec3 lookVec;
struct vec3 xAxis = {1.0f, 0.0f, 0.0f};
struct vec3 yAxis = {0.0f, 1.0f, 0.0f};
struct vec3 zAxis = {0.0f, 0.0f, 1.0f};

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
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
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   TexCoord = vec2(texCoord.x, texCoord.y);\n"
    "}\0";
const char *fragmentShaderSouce = 
    "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "    FragColor = texture(ourTexture, TexCoord);\n"
    "}\n";

int main() {
    
    struct Voxel voxel;
    _InitVoxel(&voxel);
    

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

    
    Chunk chunk;
    chunk.offsetX = 0;
    chunk.offsetY = 0;
    chunk.offsetZ = 0;
    int p[512];
    struct Ivec4 chungus[C_chunkSize][C_chunkSize][C_chunkSize] = {
      {{{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},}},
      
      {{{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},}},

      {{{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},}},

      {{{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},}},

      {{{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},}},

      {{{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1}},
      {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 1},}},
    };

    generateChunk(C_chunkSize, &chunk, chunk.offsetX, chunk.offsetY, chunk.offsetZ, p);
    
    std::vector<float> allData;
    allData.reserve(92160);
    cullChunk(&allData, chunk.chunk, C_chunkSize, voxel);
    printf("%d", allData.size());
    float verticesArray[allData.size()];
    std::copy(allData.begin(), allData.end(), verticesArray);

    unsigned int VBO, VAO;
    shader_ArrBuffs(VAO, VBO, verticesArray, sizeof(verticesArray));

    struct mat4 projectionMat = makeProjectionMatrix(45.0f, 0.1f, 100.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);

    while (!glfwWindowShouldClose(window)) {
      
      processInput(window);
      
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      

      lookVec = addVectors(cameraPos, cameraFront);
      struct mat4 viewMat = makeLookAtMatrix(cameraPos, lookVec, yAxis);

      int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_TRUE, (GLfloat*)&viewMat.m);
      int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
      glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, (GLfloat*)&projectionMat.m);

      struct mat4 modelMatrix = makeIdentityMatrix();


      int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (GLfloat*)&modelMatrix.m);
      
      shader_use(&shaderProgram);

      glBindTexture(GL_TEXTURE_2D, textureGrass);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesArray)/5);
      

      glfwSwapInterval(0);
      glfwSwapBuffers(window);
      glfwPollEvents();

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame; 
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram((GLuint)shaderProgram.ID);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  float cameraSpeed = 2.0f * deltaTime; // adjust accordingly
  struct vec3 vecA;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vecA = VscalarMulitply(cameraSpeed, cameraFront);
    cameraPos = addVectors(cameraPos, vecA);
  }
    //normalize3d(&cameraPos);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vecA = VscalarMulitply(cameraSpeed, cameraFront);
    cameraPos = subtractVectors(cameraPos, vecA);
  }
    //normalize3d(&cameraPos);
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
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}