/*
*  Date Started 12/21/2023
*  Date Last Modified 12/24/2023
*  Hours Worked ~7.5
*
*  TODO:
*  1. Fix Undefined error for all self created library functions
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "GMath/GMath.h" // math before voxel
#include "Voxel/Voxel.h"
#include "Shader/Shader.h"
#include <vector>

#include "stb_image.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

struct vec3 cameraPos = {8.0f, 50.0f, 8.0f};
struct vec3 cameraFront = {0.0f, 0.0f, -1.0f};
struct vec3 lookVec;
struct vec3 xAxis = {1.0f, 0.0f, 0.0f};
struct vec3 yAxis = {0.0f, 1.0f, 0.0f};
struct vec3 zAxis = {0.0f, 0.0f, 1.0f};

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


    int width, height, nrChannels;

    unsigned int textureGrass;
    glGenTextures(1, &textureGrass);
    glBindTexture(GL_TEXTURE_2D, textureGrass);
    // set the texture wrapping/filtering options (on the currently bound texture
    // object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    unsigned char *dataGrass =
        stbi_load("C:/Users/rkmun/source/repos/VoxelEngine/src/NewPiskel.jpg", &width, &height, &nrChannels, 0);
    if (dataGrass) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                  GL_UNSIGNED_BYTE, dataGrass);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      printf("Failed to load texture\n%s", stbi_failure_reason());
    }
    stbi_image_free(dataGrass);
    
    std::vector<float> allData;
    allData.insert(allData.end(), std::begin(voxel.eastFace), std::end(voxel.eastFace));
    allData.insert(allData.end(), std::begin(voxel.northFace), std::end(voxel.northFace));
    allData.insert(allData.end(), std::begin(voxel.southFace), std::end(voxel.southFace));
    allData.insert(allData.end(), std::begin(voxel.westFace), std::end(voxel.westFace));
    allData.insert(allData.end(), std::begin(voxel.frontFace), std::end(voxel.frontFace));
    allData.insert(allData.end(), std::begin(voxel.backFace), std::end(voxel.backFace));

    float verticesArray[allData.size()];
    std::copy(allData.begin(), allData.end(), verticesArray);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(voxel.indices), &voxel.indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArray), &verticesArray[0], GL_STATIC_DRAW);
    // apos attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    struct mat4 projectionMat = makeProjectionMatrix(45.0f, 0.1f, 100.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);
    while (!glfwWindowShouldClose(window)) {
    
      processInput(window);
      
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      

      lookVec = addVectors(cameraPos, cameraFront);
      //struct mat4 viewMat = makeLookAtMatrix(cameraPos, lookVec, yAxis);
      struct mat4 viewMat = makeIdentityMatrix();

      int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_TRUE, (GLfloat*)&viewMat.m);
      int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
      glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, (GLfloat*)&projectionMat.m);

      struct mat4 modelMatrix = makeIdentityMatrix();
      struct quaternion quat = axisAngleToQuaternion(xAxis, 10.0f);
      quatRot(quat, &modelMatrix);
      struct vec3 mV = {0.0f, 0.0f, -3.0f};
      translate(mV, &modelMatrix);


      int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (GLfloat*)&modelMatrix.m);
      
      shader_use(&shaderProgram);

      glBindTexture(GL_TEXTURE_2D, textureGrass);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapInterval(0);
      glfwSwapBuffers(window);
      glfwPollEvents();
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
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}