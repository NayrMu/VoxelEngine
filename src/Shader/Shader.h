#pragma once

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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