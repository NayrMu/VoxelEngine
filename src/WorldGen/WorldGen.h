#pragma once

#include <cmath>
#include <algorithm>
#include <vector>
#include "../Constants.h"
#include "../GMath/GMath.h"
#include "../Voxel/Voxel.h"

void seedGen(int p[512], int size, unsigned int seed) {

  unsigned int newNum = ~seed;
  newNum += 620;
  newNum %= 255;

  for (int i = 0; i < size; i++) {
    p[i] = newNum;

    newNum *= 300;
    newNum %= 255;
  }
}

double fade(double t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

double Plerp(double t, double a, double b) {
  return a + t * (b - a);
}

float grad(int hash, float x, float y) {
  int h = hash & 7;            // Use first 4 bits for gradient hash
  float grad = 1.0 + (h & 3); // Gradient value 1-4
  if (h & 4) grad = -grad;     // Randomly invert half of them
  return (grad * x + grad * y); // Gradient times (x, y)
}

double noise(double x, double y, int p[512]) {
  int X = (int)floor(x) & 255;
  int Y = (int)floor(y) & 255;

  x -= (double)floor(x);
  y -= (double)floor(y);

  float u = fade(x);
  float v = fade(y);

  

  int a = p[X] + Y, b = p[X + 1] + Y;

// And add blended results from 2 corners of the cell (scaled according to the fade curve values)
  return Plerp(v, Plerp(u, grad(p[a], x, y), grad(p[b], x - 1, y)),
                  Plerp(u, grad(p[a + 1], x, y - 1), grad(p[b + 1], x - 1, y - 1)));
}

void generateChunk(int chunkSize, struct Chunk* chunk, int offsetX, int offsetZ, int offsetY, int p[512]) {
  if (offsetZ = 2) {
  }

  double globalX;
  double globalZ;
  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      globalX = (double)(2* j) + (offsetX * C_chunkSize);
      globalZ = (double)(2* i) + (offsetZ * C_chunkSize);

      // Use the global position as input to the noise function
      float height = noise(globalX * 0.1f, globalZ * 0.1f, p) + 1.0f;
      int newHeight = (height * 0.5f * 28.0f) + 4;
      for (int k = 0; k < newHeight; k++) {
        chunk->chunk[i][j][k].w = 1;
      }
    }
  }
}

void pushFace(std::vector<float>* chunkData, Voxel voxel, vec3 transform, int face) {
  /*
  0 = NorthFace
  1 = SouthFace
  2 = WestFace
  3 = EastFace
  4 = FrontFace
  5 = BackFace
  */
  float temp[30] = {0};
  const float* faceArray = nullptr;

  switch (face) {
    case 0: faceArray = voxel.northFace; break;
    case 1: faceArray = voxel.southFace; break;
    case 2: faceArray = voxel.westFace; break;
    case 3: faceArray = voxel.eastFace; break;
    case 4: faceArray = voxel.frontFace; break;
    case 5: faceArray = voxel.backFace; break;
    default: printf("!!Not a Valid Face ID!!");
  }

  if (faceArray) {
    memcpy(temp, faceArray, sizeof(voxel.northFace));
    translateQuad(temp, transform);
    chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
  }

}

void cullChunk(std::vector<float>* chunkData, Chunk Chunk, int chunkSize, Voxel voxel) {
  int offsettXAdd = (Chunk.offsetX*chunkSize);
  int offsettYAdd = (Chunk.offsetY*chunkSize);
  int offsettZAdd = (Chunk.offsetZ*chunkSize);
  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      for (int k = 0; k < chunkSize; k++) {
        if (Chunk.chunk[i][j][k].w == 1) {
          struct vec3 transform = {0.5f*((float)j + offsettXAdd), 0.5f*((float)k + offsettYAdd), 0.5f*((float)i + offsettZAdd)};

          bool isLeftEndOfRow = j == 0;
          bool isRightEndOfRow = j == chunkSize - 1;
          bool isFrontOfColumn = i == chunkSize - 1;
          bool isBackOfColumn = i == 0;
          bool isTopOfStack = k == chunkSize - 1;
          bool isBottomOfStack = k == 0;

          if (isTopOfStack || (!isTopOfStack && Chunk.chunk[i][j][k + 1].w == 0)) {
            pushFace(chunkData, voxel, transform, 0);
          }

          if (isBottomOfStack || (!isBottomOfStack && Chunk.chunk[i][j][k - 1].w == 0)) {
            pushFace(chunkData, voxel, transform, 1);
          }

          if (isLeftEndOfRow || (!isLeftEndOfRow && Chunk.chunk[i][j - 1][k].w == 0)) {
            pushFace(chunkData, voxel, transform, 2);
          }

          if (isRightEndOfRow || (!isRightEndOfRow && Chunk.chunk[i][j + 1][k].w == 0)) {
            pushFace(chunkData, voxel, transform, 3);
          }

          if (isFrontOfColumn || (!isFrontOfColumn && Chunk.chunk[i + 1][j][k].w == 0)) {
            pushFace(chunkData, voxel, transform, 4);
          }

          if (isBackOfColumn || (!isBackOfColumn && Chunk.chunk[i - 1][j][k].w == 0)) {
            pushFace(chunkData, voxel, transform, 5);
          }
        }
      }
    }
  }
}

