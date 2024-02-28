#pragma once

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "../stb_image.h"

typedef struct
{
  unsigned int ID;
  unsigned int *inBuff;
  unsigned int *outBuff;
  int *outPtr;
} Shader;

struct IndirectDrawCommand {
    GLuint count;
    GLuint instanceCount;
    GLuint first;
    GLuint baseInstance;
};

Shader Cshader_create(const char* cShaderSource) {
  Shader Cshader;
  Cshader.ID = 0;
  unsigned int compute;

  int success;
  char infoLog[512];
  // compute shader
  compute = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(compute, 1, &cShaderSource, NULL);
  glCompileShader(compute);

  glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(compute, 512, NULL, infoLog);
    printf("!!ERROR!! COMPUTE SHADER COMPILATION FAILED\n %s", infoLog);
    return Cshader;  // Return an empty shader on failure
  }
  
  // shader Program
  Cshader.ID = glCreateProgram();
  glAttachShader(Cshader.ID, compute);
  glLinkProgram(Cshader.ID);

  glGetProgramiv(Cshader.ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(Cshader.ID, 512, NULL, infoLog);
    printf("!!ERROR!! SHADER PROGRAM LINKING FAILED\n %s", infoLog);
    return Cshader;  // Return an empty shader on failure
  }

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(compute);

  return Cshader;
}
Shader shader_create(const char* vertexShaderSource,
                     const char* fragmentShaderSource) {
  Shader shader;
  shader.ID = 0;

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderSource, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    printf("!!ERROR!! VERTEX SHADER COMPILATION FAILED\n %s", infoLog);
    return shader;  // Return an empty shader on failure
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    printf("!!ERROR!! FRAGMENT SHADER COMPILATION FAILED\n %s", infoLog);
    return shader;  // Return an empty shader on failure
  }

  shader.ID = glCreateProgram();
  glAttachShader(shader.ID, vertex);
  glAttachShader(shader.ID, fragment);
  glLinkProgram(shader.ID);

  glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
    printf("!!ERROR!! SHADER PROGRAM LINKING FAILED\n %s", infoLog);
    return shader;  // Return an empty shader on failure
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return shader;
}

void shader_use(const Shader* shader) 
{ 
    glUseProgram(shader->ID); 
}

void shader_setBool(const Shader* shader, const char* name, int value) {
  glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_setInt(const Shader* shader, const char* name, int value) {
  glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_setFloat(const Shader* shader, const char* name, float value) {
  glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

void shader_addTexture(const char* image, unsigned int* texture) {
  int width, height, nrChannels;

  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // load and generate the texture
  unsigned char *data =
      stbi_load(image, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture\n%s", stbi_failure_reason());
  }
  stbi_image_free(data);
}

void shader_ArrBuffs(unsigned int VAO, unsigned int VBO, float* array, size_t size) {

        
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
  
  // apos attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  
}

void updateGLBuffer(std::vector<float> array, int size, int offSet) {
  float* bufferData = new float[size];
  
  std::copy(array.begin(), array.end(), bufferData);
  
  glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), &bufferData[0]);

  delete[] bufferData;
}

void bindComputeBuffs(unsigned int inBuff, unsigned int outBuff, size_t size, GLint *&ptr) {

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, inBuff);
  CHECK_GL_ERROR();
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, size * sizeof(float), nullptr, GL_DYNAMIC_STORAGE_BIT);
  CHECK_GL_ERROR();
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inBuff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, outBuff);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, size * sizeof(float), NULL, GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT);
  CHECK_GL_ERROR();
  
  CHECK_GL_ERROR();
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outBuff);
  CHECK_GL_ERROR();
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, outBuff);
  CHECK_GL_ERROR();
  
  ptr = (GLint *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 0 + size * sizeof(float), GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT);
  CHECK_GL_ERROR();
  

}