#pragma once

#include "../GMath/GMath.h"
class MyCamera {
private:
  struct vec3 cameraPos;
  struct Ivec3 currentChunk;
  struct vec3 cameraFront;
  struct vec3 lookVec;
  struct vec3 xAxis;
  struct vec3 yAxis;
  struct vec3 zAxis;

  bool firstMouse;
  float yaw;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
  float pitch;
  float lastX;
  float lastY;
  float fov;

public:
  MyCamera(int SCR_WIDTH, int SCR_HEIGHT) {
    cameraPos = {32.0f, 33.0f, 32.0f};
    currentChunk = {0, 0, 0};
    cameraFront = {0.0f, 0.0f, -1.0f};
    lookVec;
    xAxis = {1.0f, 0.0f, 0.0f};
    yAxis = {0.0f, 1.0f, 0.0f};
    zAxis = {0.0f, 0.0f, 1.0f};

    firstMouse = true;
    yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    pitch =  0.0f;
    lastX =  SCR_WIDTH / 2.0;
    lastY =  SCR_HEIGHT / 2.0;
    fov   =  45.0f;
  }

  void mouse_cb(GLFWwindow* window, double xpos, double ypos) {
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

public:
  vec3 get_cam_pos() {
    return cameraPos;
  }
  vec3 get_cam_front() {
    return cameraFront;
  }
  vec3 get_xAxis() {
    return xAxis;
  }
  vec3 get_yAxis() {
    return yAxis;
  }
  vec3 get_zAxis() {
    return zAxis;
  }
  vec3 get_lookVec() {
    return lookVec;
  }
  vec3* get_cam_addy() {
    return &cameraPos;
  }

  void set_cam_pos(vec3 input) {
    cameraPos = input;
  }
  void set_cam_front(vec3 input) {
    cameraFront = input;
  }
  void set_xAxis(vec3 input) {
    xAxis = input;
  }
  void set_yAxis(vec3 input) {
    yAxis = input;
  }
  void set_zAxis(vec3 input) {
    zAxis = input;
  }
  void set_lookVec(vec3 input) {
    lookVec = input;
  }

  void set_uniforms(Shader *mainShader) {
    int camLoc = glGetUniformLocation(mainShader->ID, "cameraPos");
    glUniform3fv(camLoc, 1, (GLfloat*)&cameraPos);
  }


};