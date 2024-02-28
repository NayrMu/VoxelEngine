/*
*  Date Started 12/21/2023
*  Date Last Modified 12/31/2023
*  Hours Worked ~22.5
*
*  TODO:
8  !! Fix Segfault in CullChunk !!
*  1. Optimize Chunk Loading
*     - Load only new chunks
*     - Load Chunks one at a time?
*     - Reduce nested loops
*  2. Call compute shader from within second thread.
*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Test/TimeTest.h"

#include "Constants.h"
const int C_chunkSize = 64;
const float C_blockSize = 0.25f;
const int bsConvert = 2;

#include "GMath/GMath.h" // math before voxel
#include "Voxel/Voxel.h"
#include "Shader/Shader.h"
#include "WorldGen/WorldGen.h"
#include "VRenderer/VRenderer.h"

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void gl_frame();
void init_glfw(GLFWwindow *window);

float currentTime;
float deltaTime;

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
    glfwCreateWindow(900, 600, "LearnOpenGL", NULL, NULL);
    

    init_glfw(window);
    
    VRenderer vRender;
    ChunkManager ChunkManager(vRender.get_voxel(), vRender.get_chunk());
    ChunkManager.init_chunk_manager();
    
    currentTime = 0;
    
    while (!glfwWindowShouldClose(window)) {

      processInput(window);
      
      gl_frame();
      
      
      vRender.render_frame(&ChunkManager, &Camera);
      
      glfwSwapInterval(1);
      glfwSwapBuffers(window);
      glfwPollEvents();

      deltaTime = currentTime - glfwGetTime();
      currentTime = glfwGetTime();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  float cameraSpeed = 10.0f * deltaTime;
  struct vec3 vecA;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vecA = VscalarMulitply(cameraSpeed, Camera.get_cam_front());
    Camera.set_cam_pos(addVectors(Camera.get_cam_pos(), vecA));
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vecA = VscalarMulitply(cameraSpeed, Camera.get_cam_front());
    Camera.set_cam_pos(subtractVectors(Camera.get_cam_pos(), vecA));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    vecA = crossProduct(Camera.get_cam_front(), Camera.get_yAxis());
    normalize3d(&vecA);
    vecA = VscalarMulitply(cameraSpeed, vecA);
    Camera.set_cam_pos(subtractVectors(Camera.get_cam_pos(), vecA));
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    vecA = crossProduct(Camera.get_cam_front(), Camera.get_yAxis());
    normalize3d(&vecA);
    vecA = VscalarMulitply(cameraSpeed, vecA);
    Camera.set_cam_pos(addVectors(Camera.get_cam_pos(), vecA));
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
 Camera.mouse_cb(window, xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}



void gl_frame() {
  glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_FRONT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void init_glfw(GLFWwindow *window) {
  if (window == NULL) {
    printf("Failed to create GLFW window");
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
  glfwSetInputMode(window, GLFW_REPEAT, GLFW_FALSE);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      printf("Failed to initialize GLAD");
  } 
  if (GL_ARB_shader_storage_buffer_object) {
      std::cout << "ARB_shader_storage_buffer_object extension is supported." << std::endl;
  } else {
      std::cerr << "ARB_shader_storage_buffer_object extension is not supported." << std::endl;
  }
}