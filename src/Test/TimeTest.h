#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void startTimer(double* time) {
  *time = glfwGetTime();
}
void endTimer(double* startTime) {
  double endTime = glfwGetTime();
  *startTime = endTime - *startTime;

  if (*startTime > 0.0000001f) {
    printf("%.9f\n", *startTime);
  }
}