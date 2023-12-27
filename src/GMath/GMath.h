#pragma once

struct vec4 {
    float x, y, z, w;
};
struct vec3 {
    float x, y, z;
};
struct vec2 {
    float x, y;
};

struct mat4 {
    float m[4][4];
};

struct quaternion {
  float w, x, y, z;
};

struct mat4 makeIdentityMatrix();

struct mat4 makeLookAtMatrix(struct vec3 eye, struct vec3 target, struct vec3 up);

void translate(struct vec3 vec, struct mat4* mat);

void translatePoint(struct vec3 transform, struct vec3* point);

struct vec4 multiplyVectorMatrix(struct vec4 vec, struct mat4 mat);

struct vec3 subtractVectors(struct vec3 vec1, struct vec3 vec2);

struct vec3 addVectors(struct vec3 vec1, struct vec3 vec2);

struct vec3 VscalarMulitply(float scalar, struct vec3 vec1);

float lengthOfVector(struct vec3 vec);

struct vec3 crossProduct(struct vec3 vec1, struct vec3 vec2);

float dotProduct(struct vec3 vec1, struct vec3 vec2);

void normalize3d(struct vec3* vec);

struct quaternion axisAngleToQuaternion(struct vec3 axis, float angle);

struct mat4 mat4ByMat4(struct mat4 mat1, struct mat4 mat2);

struct mat4 quatToMat4(struct quaternion quat);

void quatRot(struct quaternion quat, struct mat4* mat);

void scale(struct vec3 vec, struct mat4* mat);

struct mat4 makeProjectionMatrix(float fov, float nearPlane, float farPlane, float width, float height);

#include <math.h>

struct mat4 makeIdentityMatrix() {
  struct mat4 mat = {0};
  mat.m[0][0] = 1.0f;
  mat.m[1][1] = 1.0f;
  mat.m[2][2] = 1.0f;
  mat.m[3][3] = 1.0f;

  return mat;
}

struct mat4 makeLookAtMatrix(struct vec3 eye, struct vec3 target, struct vec3 up) {

  struct mat4 dirMat;

  struct vec3 X, Y, Z;

  Z = subtractVectors(eye, target);
  normalize3d(&Z);
  Y = up;
  X = crossProduct(Y, Z);
  Y = crossProduct(Z, X);
  normalize3d(&X);
  normalize3d(&Y);

  float dotX;
  float dotY;
  float dotZ;
  dotX = dotProduct(X, eye);
  dotY = dotProduct(Y, eye);
  dotZ = dotProduct(Z, eye);

  dirMat.m[0][0] = X.x;
  dirMat.m[0][1] = X.y;
  dirMat.m[0][2] = X.z;
  dirMat.m[1][0] = Y.x;
  dirMat.m[1][1] = Y.y;
  dirMat.m[1][2] = Y.z;
  dirMat.m[2][0] = Z.x;
  dirMat.m[2][1] = Z.y;
  dirMat.m[2][2] = Z.z;
  dirMat.m[0][3] = -1.0f * dotX;
  dirMat.m[1][3] = -1.0f * dotY;
  dirMat.m[2][3] = -1.0f * dotZ;
  dirMat.m[3][0] = 0;
  dirMat.m[3][1] = 0;
  dirMat.m[3][2] = 0;
  dirMat.m[3][3] = 1.0f;

  return dirMat;
}

void translate(struct vec3 vec, struct mat4* mat) {
  mat->m[0][3] += vec.x;
  mat->m[1][3] += vec.y;
  mat->m[2][3] += vec.z;
}
void translateQuad(float array[], vec3 transform) {

  for (int i = 0; i < 30; ++i) {
    array[i] = array[i];
  }

  for (int i = 0; i < 30; i += 5) {
    array[i] += transform.x;
    array[i + 1] += transform.y;
    array[i + 2] += transform.z;
  }
}

struct vec4 multiplyVectorMatrix(struct vec4 vec, struct mat4 mat) {
    struct vec4 returnVec;

  returnVec.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z +
           mat.m[0][3] * vec.w;
  returnVec.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z +
           mat.m[1][3] * vec.w;
  returnVec.z = mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z +
           mat.m[2][3] * vec.w;
  returnVec.w = mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z +
           mat.m[3][3] * vec.w;

    return vec;
}

struct vec3 subtractVectors(struct vec3 vec1, struct vec3 vec2) {
    struct vec3 returnVec;
  returnVec.x = vec1.x - vec2.x;
  returnVec.y = vec1.y - vec2.y;
  returnVec.z = vec1.z - vec2.z;

  return returnVec;
}

struct vec3 addVectors(struct vec3 vec1, struct vec3 vec2) {
    struct vec3 returnVec;
  returnVec.x = vec1.x + vec2.x;
  returnVec.y = vec1.y + vec2.y;
  returnVec.z = vec1.z + vec2.z;

  return returnVec;
}

struct vec3 VscalarMulitply(float scalar, struct vec3 vec1) {
    struct vec3 returnVec;

  returnVec.x = scalar * vec1.x;
  returnVec.y = scalar * vec1.y;
  returnVec.z = scalar * vec1.z;

  return returnVec;
}

float lengthOfVector(struct vec3 vec) {
    float result;

  result = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));

  return result;
}

struct vec3 crossProduct(struct vec3 vec1, struct vec3 vec2) {
     struct vec3 returnVec;
  returnVec.x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
  returnVec.y = (vec1.z * vec2.x) - (vec1.x * vec2.z);
  returnVec.z = (vec1.x * vec2.y) - (vec1.y * vec2.x);

  return returnVec;
}

float dotProduct(struct vec3 vec1, struct vec3 vec2) {
    float result;

  result = (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
  return result;
}

void normalize3d(struct vec3* vec) {
  float dotProduct = vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
  float magnitude = sqrt(dotProduct);

  if (magnitude != 0.0) {
    vec->x /= magnitude;
    vec->y /= magnitude;
    vec->z /= magnitude;
  }
}

struct quaternion axisAngleToQuaternion(struct vec3 axis, float angle) {
  normalize3d(&axis);

  angle *= (M_PI / 180);

  struct quaternion result;

  result.w = cos(angle / 2.0);
  float factor = sin(angle / 2.0);
  result.x = factor * axis.x;
  result.y = factor * axis.y;
  result.z = factor * axis.z;

  return result;
}

struct mat4 mat4ByMat4(struct mat4 mat1, struct mat4 mat2) {
  struct mat4 result =
      makeIdentityMatrix();  // Create a temporary mat to hold the result

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = 0.0;
      for (int k = 0; k < 4; ++k) {
        result.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
      }
    }
  }

  // Copy the result back to mat1
  return result;
}

struct mat4 quatToMat4(struct quaternion quat) {
  struct mat4 mat;
  double w = quat.w, x = quat.x, y = quat.y, z = quat.z;

  mat.m[0][0] = 1.0 - 2.0 * (y * y + z * z);
  mat.m[0][1] = 2.0 * (x * y - w * z);
  mat.m[0][2] = 2.0 * (x * z + w * y);
  mat.m[0][3] = 0.0;

  mat.m[1][0] = 2.0 * (x * y + w * z);
  mat.m[1][1] = 1.0 - 2.0 * (x * x + z * z);
  mat.m[1][2] = 2.0 * (y * z - w * x);
  mat.m[1][3] = 0.0;

  mat.m[2][0] = 2.0 * (x * z - w * y);
  mat.m[2][1] = 2.0 * (y * z + w * x);
  mat.m[2][2] = 1.0 - 2.0 * (x * x + y * y);
  mat.m[2][3] = 0.0;

  mat.m[3][0] = 0.0;
  mat.m[3][1] = 0.0;
  mat.m[3][2] = 0.0;
  mat.m[3][3] = 1.0;

  return mat;
}

void quatRot(struct quaternion quat, struct mat4* mat) {
  struct mat4 quatMat;
  quatMat = quatToMat4(quat);

  *mat = mat4ByMat4(quatMat, *mat);
}

void scale(struct vec3 vec, struct mat4* mat) {
  mat->m[0][0] *= vec.x;
  mat->m[1][1] *= vec.y;
  mat->m[2][2] *= vec.z;
}

struct mat4 makeProjectionMatrix(float fov, float nearPlane, float farPlane, float width, float height) {
    struct mat4 mat = makeIdentityMatrix();
  float a = width / height;

  float r = nearPlane * tan(fov / 2);
  float t = r / a;

  mat.m[0][0] = nearPlane / r;
  mat.m[1][1] = nearPlane / t;
  mat.m[2][2] = -1.0f * ((farPlane + nearPlane) / (farPlane - nearPlane));
  mat.m[2][3] = -2.0f * ((farPlane * nearPlane) / (farPlane - nearPlane));
  mat.m[3][2] = -1.0f;

  return mat;
}