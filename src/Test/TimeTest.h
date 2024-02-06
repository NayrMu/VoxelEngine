#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void startTimer(float* time) {
  *time = glfwGetTime();
}
void endTimer(float* startTime) {
  float endTime = glfwGetTime();
  float totalTime = endTime - *startTime;

  printf("Total Time: %f\n", totalTime);
}