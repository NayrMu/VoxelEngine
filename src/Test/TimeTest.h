#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void startTimer(float* time) {
  *time = glfwGetTime();
}
void endTimer(float* startTime) {
  float endTime = glfwGetTime();
  *startTime = endTime - *startTime;

  if (*startTime > 0.00001f) {
    printf("%f\n", *startTime);
  }
}