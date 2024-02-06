#pragma once 
#include <string.h>
#include <vector>

#include "../Constants.h"

struct Voxel {

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




unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

void _InitVoxel(struct Voxel* voxel) {

    const int atlasWidth = 24;
    const int atlasHeight = 8;
    const int numTextures = 3;
    // Calculate the width and height of each individual texture
    const float textureWidth = 1.0f / (float)numTextures;
    const float textureHeight = 1.0f;

    struct vec2 faceTextureCoords[3] = {
        {0.0f, 0.0f},
        {0.333f, 0.0f},
        {0.666f, 0.0f}
    };

    const float bFace[30] = {
        -0.25f, -0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
        0.25f, -0.25f, -0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y + textureHeight,
        0.25f,  0.25f, -0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
        0.25f,  0.25f, -0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
        -0.25f,  0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y,
        -0.25f, -0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
    };

    const float fFace[30] = {
        
        0.25f, -0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y + textureHeight,
        -0.25f, -0.25f,  0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
        0.25f,  0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
        0.25f,  0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
        -0.25f, -0.25f,  0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
        -0.25f,  0.25f,  0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y,
    };

    const float wFace[30] = {
        -0.25f,  0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y,
        -0.25f,  0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
        -0.25f, -0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
        -0.25f, -0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
        -0.25f,  0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
        -0.25f, -0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y + textureHeight,
    };

    const float eFace[30] = {
        0.25f,  0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
        0.25f,  0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y,
        0.25f, -0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
        0.25f, -0.25f, -0.25f,  faceTextureCoords[1].x + textureWidth, faceTextureCoords[1].y + textureHeight,
        0.25f, -0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y + textureHeight,
        0.25f,  0.25f,  0.25f,  faceTextureCoords[1].x, faceTextureCoords[1].y,
    };

    const float sFace[30] = {
        0.25f, -0.25f, -0.25f,  faceTextureCoords[2].x + textureWidth, faceTextureCoords[2].y,
        -0.25f, -0.25f, -0.25f,  faceTextureCoords[2].x, faceTextureCoords[2].y,
        0.25f, -0.25f,  0.25f,  faceTextureCoords[2].x + textureWidth, faceTextureCoords[2].y + textureHeight,
        0.25f, -0.25f,  0.25f,  faceTextureCoords[2].x + textureWidth, faceTextureCoords[2].y + textureHeight,
        -0.25f, -0.25f, -0.25f,  faceTextureCoords[2].x, faceTextureCoords[2].y,
        -0.25f, -0.25f,  0.25f,  faceTextureCoords[2].x, faceTextureCoords[2].y + textureHeight,
    };

    float nFace[30] = {
        -0.25f,  0.25f, -0.25f,  faceTextureCoords[0].x, faceTextureCoords[0].y,
        0.25f,  0.25f, -0.25f,  faceTextureCoords[0].x + textureWidth, faceTextureCoords[0].y,
        0.25f,  0.25f,  0.25f,  faceTextureCoords[0].x + textureWidth, faceTextureCoords[0].y + textureHeight,
        0.25f,  0.25f,  0.25f,  faceTextureCoords[0].x + textureWidth, faceTextureCoords[0].y + textureHeight,
        -0.25f,  0.25f,  0.25f,  faceTextureCoords[0].x, faceTextureCoords[0].y + textureHeight,
        -0.25f,  0.25f, -0.25f,  faceTextureCoords[0].x, faceTextureCoords[0].y,
    };
    
    memcpy(voxel->northFace, nFace, sizeof(nFace));

    memcpy(voxel->eastFace, eFace, sizeof(eFace));

    memcpy(voxel->southFace, sFace, sizeof(sFace));

    memcpy(voxel->westFace, wFace, sizeof(wFace));

    memcpy(voxel->frontFace, fFace, sizeof(fFace));

    memcpy(voxel->backFace, bFace, sizeof(bFace));

    memcpy(voxel->indices, indices, sizeof(indices));

}
