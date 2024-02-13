#pragma once

#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include "../Constants.h"
#include "../GMath/GMath.h"
#include "../Voxel/Voxel.h"
#include "../Noise/noise.h"
//#include "../Noise/snoise.h"
#include "../Test/TimeTest.h"

#define FASTFLOOR(x) ( ((int)(x)<(x)) ? ((int)x) : ((int)x-1 ) )


unsigned int seedGen() {

  unsigned int newNum = rand() % 1000;
  newNum += 620;

  newNum *= 3;

  newNum = newNum & 620;

  return newNum;
  
}



void generateChunk(int chunkSize, struct Chunk* chunk, int offsetX, int offsetZ, int offsetY, int seed) {
  
   
  //
  double globalX;
  double globalZ;
  double globalY;
  int flatIndex = 0;
  float wavelength1 = C_chunkSize / 0.65f;
  float wavelength2 = C_chunkSize / 3.41f;
  
  for (int i = 0; i < chunkSize; i++) {
    
    globalZ = (double)(i) + (offsetZ * C_chunkSize);
    float freqZ = globalZ / wavelength2;
    for (int j = 0; j < chunkSize; j++) {
      
      globalX = (double)(j) + (offsetX * C_chunkSize);
      float freqX = globalX / wavelength2;
      float hillHeight = 14.0f * (noise2(globalX / wavelength1, globalZ / wavelength1) + 0.9f);

      for (int k = 0; k < chunkSize; k++) {
        globalY = (double)(k) + (offsetY * C_chunkSize);
        flatIndex = i * (chunkSize * chunkSize) + (j * chunkSize) + k;

        float density = 85.0f * (noise3(freqX, freqZ,  globalY / wavelength2) + 0.6f);

        float Time;
        //startTimer(&Time);
        float height = (LERP(0.23f, (hillHeight + density), hillHeight)) + 10.0f;
        //endTimer(&Time);


        
        if (k < height) {
          chunk->chunk[flatIndex].w = 1;
        }
      }
    }
  }
  //
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
 
  float* faceArray = nullptr;

  switch (face) {
    case 32: faceArray = voxel.getNFace(); break;
    case 16: faceArray = voxel.getSFace(); break;
    case 8: faceArray = voxel.getWFace(); break;
    case 4: faceArray = voxel.getEFace(); break;
    case 2: faceArray = voxel.getFFace(); break;
    case 1: faceArray = voxel.getBFace(); break;
    default: return;
  }

  if (faceArray) {
    translateQuad(faceArray, transform);
    chunkData->insert(chunkData->end(), faceArray, faceArray + 30);
  }
  
}

void cullChunk(std::vector<float>* chunkData, Chunk Chunk, int chunkSize, Voxel *voxel) {

  

  int offsettXAdd = (Chunk.offsetX*chunkSize);
  int offsettYAdd = (Chunk.offsetY*chunkSize);
  int offsettZAdd = (Chunk.offsetZ*chunkSize);
  int flatIndex = 0;

  for (int i = 0; i < chunkSize; i++) {
    for (int j = 0; j < chunkSize; j++) {
      float Time;
      //startTimer(&Time);
      for (int k = 0; k < chunkSize; k++) {
        flatIndex = i * (chunkSize * chunkSize) + (j * chunkSize) + k;
        
        
        if (k < chunkSize - 1 && Chunk.chunk[flatIndex+1].w != 0) {
          Chunk.chunk[flatIndex].w = 2;
        }

        if (k < chunkSize - 6 && Chunk.chunk[flatIndex+6].w != 0) {
          Chunk.chunk[flatIndex].w = 3;
        }

        int Jprev = flatIndex - chunkSize;
        int Iprev = flatIndex - (chunkSize*chunkSize);
        int Kprev = flatIndex - 1;
        int Jnext = flatIndex + chunkSize;
        int Inext = flatIndex + (chunkSize*chunkSize);
        int Knext = flatIndex + 1;

        
        
        if (Chunk.chunk[flatIndex].w != 0) {
          voxel->setTextureOffset(Chunk.chunk[flatIndex].w);
          
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

          
          
          isNotBlockAbove = !(isTopOfStack) ? (Chunk.chunk[Knext].w == 0) : false;
          isNotBlockBelow = !(isBottomOfStack) ? (Chunk.chunk[Kprev].w == 0) : false;
          isNotBlockToLeft = !(isLeftEndOfRow) ? Chunk.chunk[Jprev].w == 0 : false;
          isNotBlockToRight = !(isRightEndOfRow) ? Chunk.chunk[Jnext].w == 0 : false;
          isNotBlockInFront = !(isFrontOfColumn ) ? (Chunk.chunk[Inext].w == 0) : false;
          isNotBlockBehind = !(isBackOfColumn) ? (Chunk.chunk[Iprev].w == 0) : false;

          unsigned char edgeMask  = 0;
          edgeMask = (isTopOfStack << 5) | (isBottomOfStack << 4) | (isLeftEndOfRow << 3) | 
            (isRightEndOfRow << 2) | (isFrontOfColumn << 1) | isBackOfColumn;
          unsigned char notEdgeMask = (!isTopOfStack << 5) | (!isBottomOfStack << 4) | (!isLeftEndOfRow << 3) | 
            (!isRightEndOfRow << 2) | (!isFrontOfColumn << 1) | !isBackOfColumn;
          unsigned char neighborMask = 0;
          neighborMask = (isNotBlockAbove << 5) | (isNotBlockBelow << 4) | (isNotBlockToLeft << 3) | 
            (isNotBlockToRight<< 2) | (isNotBlockInFront << 1) | isNotBlockBehind;
          unsigned char con2 = (notEdgeMask & neighborMask);
          unsigned char pushMask = edgeMask | con2;
          unsigned char checker = 1; // x000001
          //float Time;
          //startTimer(&Time);

          if (pushMask != 0) {
            for (int b = 0; b < 6; b++) {
              pushFace(chunkData, *voxel, transform, checker & pushMask);
              checker = checker << 1;
            }
          }
          //endTimer(&Time);
          
        }
        //endTimer(&Time);
      }
    }
  }
}

