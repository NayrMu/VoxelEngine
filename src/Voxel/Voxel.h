#pragma once 
#include <string.h>
#include <vector>

#include "../Constants.h"

struct voxel {

  float northFace[30];

  float eastFace[30];

  float southFace[30];

  float westFace[30];

  float frontFace[30];

  float backFace[30];
  
  unsigned int indices[6];

};

struct Chunk {
  std::vector<Ivec4> chunk;

  int offsetX = 0;
  int offsetY = 0;
  int offsetZ = 0;
};

class Voxel  {

private:
  voxel Cube;
  unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
  const int atlasWidth = 48;
  const int atlasHeight = 8;
  const int numTextures = 6;
  // Calculate the width and height of each individual texture
  const float textureWidth = 1.0f / (float)numTextures;
  const float textureHeight = 1.0f;
  struct vec2 faceTextureCoords[6] = {
    {0.0f, 0.0f},
    {0.166f, 0.0f},
    {0.333f, 0.0f},
    {0.5f, 0.0f},
    {0.666f, 0.0f},
    {0.833f, 0.0f},
  };

  int currentTexIdx[3] = {0, 1, 2};
  int grassTexIdx[3] = {0, 1, 2};
  int dirtTexIdx[3] = {2, 2, 2};
  int stoneTexIdx[3] = {3, 4, 5};

  float SideXWC = faceTextureCoords[currentTexIdx[1]].x + textureWidth;
  float SideXNC = faceTextureCoords[currentTexIdx[1]].x;
  float SideYWC = faceTextureCoords[currentTexIdx[1]].y + textureHeight;
  float SideYNC = faceTextureCoords[currentTexIdx[1]].y;
  float BotXWC = faceTextureCoords[currentTexIdx[2]].x + textureWidth;
  float BotXNC = faceTextureCoords[currentTexIdx[2]].x;
  float BotYWC = faceTextureCoords[currentTexIdx[2]].y + textureHeight;
  float BotYNC = faceTextureCoords[currentTexIdx[2]].y;
  float TopXWC = faceTextureCoords[currentTexIdx[0]].x + textureWidth;
  float TopXNC = faceTextureCoords[currentTexIdx[0]].x;
  float TopYWC = faceTextureCoords[currentTexIdx[0]].y + textureHeight;
  float TopYNC = faceTextureCoords[currentTexIdx[0]].y;

  

public:
  void setTextureOffset(int blockType) {
    
    switch (blockType) {
      case 1:
        currentTexIdx[0] = grassTexIdx[0];
        currentTexIdx[1] = grassTexIdx[1];
        currentTexIdx[2] = grassTexIdx[2];
        break;
      case 2:
        currentTexIdx[0] = dirtTexIdx[0];
        currentTexIdx[1] = dirtTexIdx[1];
        currentTexIdx[2] = dirtTexIdx[2];
        break;
      case 3:
        currentTexIdx[0] = stoneTexIdx[0];
        currentTexIdx[1] = stoneTexIdx[1];
        currentTexIdx[2] = stoneTexIdx[2];
        break;
    };

    SideXWC = faceTextureCoords[currentTexIdx[1]].x + textureWidth;
    SideXNC = faceTextureCoords[currentTexIdx[1]].x;
    SideYWC = faceTextureCoords[currentTexIdx[1]].y + textureHeight;
    SideYNC = faceTextureCoords[currentTexIdx[1]].y;
    BotXWC = faceTextureCoords[currentTexIdx[2]].x + textureWidth;
    BotXNC = faceTextureCoords[currentTexIdx[2]].x;
    BotYWC = faceTextureCoords[currentTexIdx[2]].y + textureHeight;
    BotYNC = faceTextureCoords[currentTexIdx[2]].y;
    TopXWC = faceTextureCoords[currentTexIdx[0]].x + textureWidth;
    TopXNC = faceTextureCoords[currentTexIdx[0]].x;
    TopYWC = faceTextureCoords[currentTexIdx[0]].y + textureHeight;
    TopYNC = faceTextureCoords[currentTexIdx[0]].y;

    float bFaceCoords[12] = {
      SideXWC, SideYWC,
      SideXNC, SideYWC,
      SideXNC, SideYNC,
      SideXNC, SideYNC,
      SideXWC, SideYNC,
      SideXWC, SideYWC,
    };
    float fFaceCoords[12] = {
      SideXNC, SideYWC,
      SideXWC, SideYWC,
      SideXNC, SideYNC,
      SideXNC, SideYNC,
      SideXWC, SideYWC,
      SideXWC, SideYNC,
    };
    float wFaceCoords[12] = {
      SideXWC, SideYNC,
      SideXNC, SideYNC,
      SideXWC, SideYWC,
      SideXWC, SideYWC,
      SideXNC, SideYNC,
      SideXNC, SideYWC,
    };
    float eFaceCoords[12] = {
      SideXNC, SideYNC,
      SideXWC, SideYNC,
      SideXWC, SideYWC,
      SideXWC, SideYWC,
      SideXNC, SideYWC,
      SideXNC, SideYNC,
    };
    float nFaceCoords[12] = {
      TopXNC, TopYNC,
      TopXWC, TopYNC,
      TopXWC, TopYWC,
      TopXWC, TopYWC,
      TopXNC, TopYWC,
      TopXNC, TopYNC,
    };
    float sFaceCoords[12] = {
      BotXWC, BotYNC,
      BotXNC, BotYNC,
      BotXWC, BotYWC,
      BotXWC, BotYWC,
      BotXNC, BotYNC,
      BotXNC, BotYWC,
    };

    for (int i = 0; i < 6; i++) {
      int offSet = i*5;
      Cube.backFace[3+offSet] = bFaceCoords[0+2*i];
      Cube.backFace[4+offSet] = bFaceCoords[1+2*i];
      Cube.frontFace[3+offSet] = fFaceCoords[0+2*i];
      Cube.frontFace[4+offSet] = fFaceCoords[1+2*i];
      Cube.northFace[3+offSet] = nFaceCoords[0+2*i];
      Cube.northFace[4+offSet] = nFaceCoords[1+2*i];
      Cube.southFace[3+offSet] = sFaceCoords[0+2*i];
      Cube.southFace[4+offSet] = sFaceCoords[1+2*i];
      Cube.westFace[3+offSet] = wFaceCoords[0+2*i];
      Cube.westFace[4+offSet] = wFaceCoords[1+2*i];
      Cube.eastFace[3+offSet] = eFaceCoords[0+2*i];
      Cube.eastFace[4+offSet] = eFaceCoords[1+2*i];
    }

  }

  Voxel() {

    const float bFace[30] = {
      -C_blockSize, -C_blockSize, -C_blockSize,  SideXWC, SideYWC,
      C_blockSize, -C_blockSize, -C_blockSize,  SideXNC, SideYWC,
      C_blockSize,  C_blockSize, -C_blockSize,  SideXNC, SideYNC,
      C_blockSize,  C_blockSize, -C_blockSize,  SideXNC, SideYNC,
      -C_blockSize,  C_blockSize, -C_blockSize,  SideXWC, SideYNC,
      -C_blockSize, -C_blockSize, -C_blockSize,  SideXWC, SideYWC,
    };

    const float fFace[30] = {
        
      C_blockSize, -C_blockSize,  C_blockSize,  SideXNC, SideYWC,
      -C_blockSize, -C_blockSize,  C_blockSize,  SideXWC, SideYWC,
      C_blockSize,  C_blockSize,  C_blockSize,  SideXNC, SideYNC,
      C_blockSize,  C_blockSize,  C_blockSize,  SideXNC, SideYNC,
      -C_blockSize, -C_blockSize,  C_blockSize,  SideXWC, SideYWC,
      -C_blockSize,  C_blockSize,  C_blockSize,  SideXWC, SideYNC,
    };

    const float wFace[30] = {
      -C_blockSize,  C_blockSize, -C_blockSize,  SideXWC, SideYNC,
      -C_blockSize,  C_blockSize,  C_blockSize,  SideXNC, SideYNC,
      -C_blockSize, -C_blockSize, -C_blockSize,  SideXWC, SideYWC,
      -C_blockSize, -C_blockSize, -C_blockSize,  SideXWC, SideYWC,
      -C_blockSize,  C_blockSize,  C_blockSize,  SideXNC, SideYNC,
      -C_blockSize, -C_blockSize,  C_blockSize,  SideXNC, SideYWC,
    };

    const float eFace[30] = {
      C_blockSize,  C_blockSize,  C_blockSize,  SideXNC, SideYNC,
      C_blockSize,  C_blockSize, -C_blockSize,  SideXWC, SideYNC,
      C_blockSize, -C_blockSize, -C_blockSize,  SideXWC, SideYWC,
      C_blockSize, -C_blockSize, -C_blockSize,  SideXWC, SideYWC,
      C_blockSize, -C_blockSize,  C_blockSize,  SideXNC, SideYWC,
      C_blockSize,  C_blockSize,  C_blockSize,  SideXNC, SideYNC,
    };

    const float sFace[30] = {
      C_blockSize, -C_blockSize, -C_blockSize,  BotXWC, BotYNC,
      -C_blockSize, -C_blockSize, -C_blockSize,  BotXNC, BotYNC,
      C_blockSize, -C_blockSize,  C_blockSize,  BotXWC, BotYWC,
      C_blockSize, -C_blockSize,  C_blockSize,  BotXWC, BotYWC,
      -C_blockSize, -C_blockSize, -C_blockSize,  BotXNC, BotYNC,
      -C_blockSize, -C_blockSize,  C_blockSize,  BotXNC, BotYWC,
    };

    float nFace[30] = {
      -C_blockSize,  C_blockSize, -C_blockSize,  TopXNC, TopYNC,
      C_blockSize,  C_blockSize, -C_blockSize,  TopXWC, TopYNC,
      C_blockSize,  C_blockSize,  C_blockSize,  TopXWC, TopYWC,
      C_blockSize,  C_blockSize,  C_blockSize,  TopXWC, TopYWC,
      -C_blockSize,  C_blockSize,  C_blockSize,  TopXNC, TopYWC,
      -C_blockSize,  C_blockSize, -C_blockSize,  TopXNC, TopYNC,
    };
  

    memcpy(Cube.northFace, nFace, sizeof(nFace));

    memcpy(Cube.eastFace, eFace, sizeof(eFace));

    memcpy(Cube.southFace, sFace, sizeof(sFace));

    memcpy(Cube.westFace, wFace, sizeof(wFace));

    memcpy(Cube.frontFace, fFace, sizeof(fFace));

    memcpy(Cube.backFace, bFace, sizeof(bFace));

    memcpy(Cube.indices, indices, sizeof(indices));

  }

  float* getNFace() {
    return Cube.northFace;
  }
  float* getSFace() {
    return Cube.southFace;
  }
  float* getWFace() {
    return Cube.westFace;
  }
  float* getEFace() {
    return Cube.eastFace;
  }
  float* getFFace() {
    return Cube.frontFace;
  }
  float* getBFace() {
    return Cube.backFace;
  }
};