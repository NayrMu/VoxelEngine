#pragma once

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stb_image.h"

typedef struct
{
  unsigned int ID;
} Shader;



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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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

void shader_ArrBuffs(unsigned int VAO, unsigned int VBO, float array[], size_t size) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size, array, GL_STATIC_DRAW);
  // apos attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}