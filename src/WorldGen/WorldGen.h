#pragma once

#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include "../Constants.h"
#include "../GMath/GMath.h"
#include "../Voxel/Voxel.h"
#include "../Noise/noise.h"

#define FASTFLOOR(x) ( ((int)(x)<(x)) ? ((int)x) : ((int)x-1 ) )

unsigned int seedGen() {

  unsigned int newNum = rand() % 1000;
  newNum += 620;

  newNum *= 3;

  newNum = newNum & 620;

  return newNum;
  
}



void generateChunk(int chunkSize, struct Chunk* chunk, int offsetX, int offsetZ, int offsetY, int seed) {
  
  double globalX;
  double globalZ;
  int flatIndex = 0;
  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      
      globalX = (double)(j) + (offsetX * C_chunkSize);
      globalZ = (double)(i) + (offsetZ * C_chunkSize);

      // Use the global position as input to the noise function
      float height = noise2(globalX * 0.034f, globalZ * 0.034f) + 1.0f;
      
      int newHeight = (height *  32);
      //printf("X: %d Z: %d Y: %d\n", offsetX, offsetZ, newHeight);
      for (int k = 0; k < chunkSize; k++) {
        flatIndex = i * (chunkSize * chunkSize) + (j * chunkSize) + k;
        if (k < newHeight) {
          chunk->chunk[flatIndex].w = 1;
        }
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
  int flatIndex = 0;

  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      for (int k = 0; k < chunkSize; k++) {
        flatIndex = i * (chunkSize * chunkSize) + (j * chunkSize) + k;

        
        int Jprev = flatIndex - chunkSize;
        int Iprev = flatIndex - (chunkSize*chunkSize);
        int Kprev = flatIndex - 1;
        int Jnext = flatIndex + chunkSize;
        int Inext = flatIndex + (chunkSize*chunkSize);
        int Knext = flatIndex + 1;

        if (Chunk.chunk[flatIndex].w == 1) {
          struct vec3 transform = {0.5f*((float)j + offsettXAdd), 0.5f*((float)k + offsettYAdd), 0.5f*((float)i + offsettZAdd)};

          bool isLeftEndOfRow = j == 0;
          bool isRightEndOfRow = j == chunkSize-1;
          bool isFrontOfColumn = i == chunkSize-1;
          bool isBackOfColumn = i == 0;
          bool isTopOfStack = k == chunkSize-1;
          bool isBottomOfStack = k == 0;

          bool isNotBlockToLeft = false;
          bool isNotBlockToRight = false;
          bool isNotBlockBehind = false;
          bool isNotBlockInFront = false;
          bool isNotBlockAbove = false;
          bool isNotBlockBelow = false;

          if (!(isLeftEndOfRow)) {
            isNotBlockToLeft = Chunk.chunk[Jprev].w == 0;
          }
          if (!(isRightEndOfRow)) {
            isNotBlockToRight = Chunk.chunk[Jnext].w == 0;
          }
          if (!(isFrontOfColumn )) {
            isNotBlockInFront = Chunk.chunk[Inext].w == 0;
          }
          if (!(isBackOfColumn)) {
            isNotBlockBehind = Chunk.chunk[Iprev].w == 0;
          }
          if (!(isTopOfStack)) {
            isNotBlockAbove = Chunk.chunk[Knext].w == 0;
          }
          if (!(isBottomOfStack)) {
            isNotBlockBelow = Chunk.chunk[Kprev].w == 0;
          }

          if (isTopOfStack) {
            pushFace(chunkData, voxel, transform, 0);
          }
          if (!(isTopOfStack) && isNotBlockAbove) {
            pushFace(chunkData, voxel, transform, 0);
          }
          if (isBottomOfStack) {
            pushFace(chunkData, voxel, transform, 1);
          }
          if (!(isBottomOfStack) && isNotBlockBelow) {
            pushFace(chunkData, voxel, transform, 1);
          }
          if (isLeftEndOfRow) {
            pushFace(chunkData, voxel, transform, 2);
          }
          if (!(isLeftEndOfRow) && isNotBlockToLeft) {
            pushFace(chunkData, voxel, transform, 2);
          }
          if (isRightEndOfRow) {
            pushFace(chunkData, voxel, transform, 3);
          }
          if (!(isRightEndOfRow) && isNotBlockToRight) {
            pushFace(chunkData, voxel, transform, 3);
          }
          if (isFrontOfColumn) {
            pushFace(chunkData, voxel, transform, 4);
          }
          if (!(isFrontOfColumn) && isNotBlockInFront) {
            pushFace(chunkData, voxel, transform, 4);
          }
          if (isBackOfColumn) {
            pushFace(chunkData, voxel, transform, 5);
          }
          if (!(isBackOfColumn) && isNotBlockBehind) {
            pushFace(chunkData, voxel, transform, 5);
          }
        }
      }
    }
  }
}

