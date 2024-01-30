#pragma once

#include <vector>
#include "../Constants.h"
#include "../GMath/GMath.h"
#include "../Voxel/Voxel.h"

void generateChunk(int chunkSize, struct Chunk* chunk, int offsetX, int offsetZ, int offsetY, int p[512]) {
  int bX, bY, bZ;

  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      for (int k = 0; k < chunkSize; k++) {
        bY = k + offsetY;
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
 switch (face) {
  case 0:
    memcpy(temp, voxel.northFace, sizeof(voxel.northFace));
    translateQuad(temp, transform);
    chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
    break;
  case 1:
    memcpy(temp, voxel.southFace, sizeof(voxel.southFace));
    translateQuad(temp, transform);
    chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
    break;
  case 2:
    memcpy(temp, voxel.westFace, sizeof(voxel.westFace));
    translateQuad(temp, transform);
    chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
    break;
  case 3:
    memcpy(temp, voxel.eastFace, sizeof(voxel.eastFace));
    translateQuad(temp, transform);
    chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
    break;
  case 4:
    memcpy(temp, voxel.frontFace, sizeof(voxel.frontFace));
    translateQuad(temp, transform);
    chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
    break;
  case 5:
    memcpy(temp, voxel.backFace, sizeof(voxel.backFace));
    translateQuad(temp, transform);
    chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
    break;
  default:
    printf("!!Not a Valid Face ID!!");
 }

}

void cullChunk(std::vector<float>* chunkData, Chunk Chunk, int chunkSize, Voxel voxel) {
  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      for (int k = 0; k < chunkSize; k++) {
        if (Chunk.chunk[i][j][k].w == 1) {
          struct vec3 transform = {(float)j + (Chunk.offsetX*chunkSize), (float)k + (Chunk.offsetY*chunkSize), (float)i + (Chunk.offsetZ*chunkSize)};
          float temp[30] = {0};

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

          if (!(isLeftEndOfRow || isRightEndOfRow)) {
            isNotBlockToLeft = Chunk.chunk[i][j-1][k].w == 0;
            isNotBlockToRight = Chunk.chunk[i][j+1][k].w == 0;
          }
          if (!(isFrontOfColumn || isBackOfColumn)) {
            isNotBlockBehind = Chunk.chunk[i-1][j][k].w == 0;
            isNotBlockInFront = Chunk.chunk[i+1][j][k].w == 0;
          }
          if (!(isTopOfStack || isBottomOfStack)) {
            isNotBlockAbove = Chunk.chunk[i][j][k+1].w == 0;
            isNotBlockBelow = Chunk.chunk[i][j][k-1].w == 0;
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

          /*
          if (j > 0 && j < chunkSize-1) { // if not end of row
            if (chunk[i][j-1][k].w == 0) { // block to left
              memcpy(temp, voxel.westFace, sizeof(voxel.westFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
            if (chunk[i][j+1][k].w == 0) { // block to right
              memcpy(temp, voxel.eastFace, sizeof(voxel.eastFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          else if (j==0) { // left end of row
            memcpy(temp, voxel.westFace, sizeof(voxel.westFace));
            translateQuad(temp, transform);
            chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            if (chunk[i][j+1][k].w == 0) {
              memcpy(temp, voxel.eastFace, sizeof(voxel.eastFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          else if (j==chunkSize-1) { // right end of row
            memcpy(temp, voxel.eastFace, sizeof(voxel.eastFace));
            translateQuad(temp, transform);
            chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            if (chunk[i][j-1][k].w == 0) {
              memcpy(temp, voxel.westFace, sizeof(voxel.westFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          if ( i > 0 && i < chunkSize-1) {  // if not end of column
            if (chunk[i-1][j][k].w == 0) {  // block behind
              memcpy(temp, voxel.backFace, sizeof(voxel.backFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
            if (chunk[i+1][j][k].w == 0) {  // block in front
              memcpy(temp, voxel.frontFace, sizeof(voxel.frontFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          else if (i==0) {  // back of coloumn
            memcpy(temp, voxel.backFace, sizeof(voxel.backFace));
            translateQuad(temp, transform);
            chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            if (chunk[i+1][j][k].w == 0) {  // block in front
              memcpy(temp, voxel.frontFace, sizeof(voxel.frontFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          else if (i==chunkSize-1) {  // front of column
            memcpy(temp, voxel.frontFace, sizeof(voxel.frontFace));
            translateQuad(temp, transform);
            chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            if (chunk[i-1][j][k].w == 0) {  // block behind
              memcpy(temp, voxel.backFace, sizeof(voxel.backFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          if (k > 0 && k < chunkSize-1) { // if not end of stack
            if (chunk[i][j][k-1].w == 0) { // block below
              memcpy(temp, voxel.southFace, sizeof(voxel.southFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
            if (chunk[i][j][k+1].w == 0) { // block above
              memcpy(temp, voxel.northFace, sizeof(voxel.northFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          else if (k==0) {  // bottom of stack
            memcpy(temp, voxel.southFace, sizeof(voxel.southFace));
            translateQuad(temp, transform);
            chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            if (chunk[i][j][k+1].w == 0) {  // block above
              memcpy(temp, voxel.northFace, sizeof(voxel.northFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }
          else if (k==chunkSize-1) {  // top of stack
            memcpy(temp, voxel.northFace, sizeof(voxel.northFace));
            translateQuad(temp, transform);
            chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            if (chunk[i][j][k-1].w == 0) {  // block below
              memcpy(temp, voxel.southFace, sizeof(voxel.southFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
          }*/
        }
      }
    }
  }
}

