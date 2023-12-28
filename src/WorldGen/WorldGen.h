#pragma once

#include <vector>
#include "../Constants.h"
#include "../GMath/GMath.h"
#include "../Voxel/Voxel.h"

void generateChunk(int chunkSize, struct Chunk* chunk, int offsetX, int offsetZ, int offsetY, int p[512]) {
  int bX, bY, bZ;

  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      printf("%d", j);
      for (int k = 0; k < chunkSize; k++) {
        bY = k + offsetY;
        chunk->chunk[i][j][k].w = 1;
      }
    }
  }
}


void cullChunk(std::vector<float>* chunkData, Ivec4 chunk[C_chunkSize][C_chunkSize][C_chunkSize], int chunkSize, Voxel voxel) {
  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      for (int k = 0; k < chunkSize; k++) {
        if (chunk[i][j][k].w == 1) {
          struct vec3 transform = {(float)j, (float)k, (float)i};
          float temp[30] = {0};
          if (j > 0 && j < chunkSize-1) { // if not end of row
            if (chunk[i][j-1][k].w == 0) { //block to left
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
          if (k > 0 && k < chunkSize-1) {
            if (chunk[i][j][k-1].w == 0) {
              memcpy(temp, voxel.southFace, sizeof(voxel.southFace));
              translateQuad(temp, transform);
              chunkData->insert(chunkData->end(), std::begin(temp), std::end(temp));
            }
            if (chunk[i][j][k+1].w == 0) {
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
          }
        }
      }
    }
  }
}