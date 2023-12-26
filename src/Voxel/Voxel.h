#pragma once 
#include <string.h>



struct Voxel {

    float northFace[30];

    float eastFace[30];

    float southFace[30];

    float westFace[30];

    float frontFace[30];

    float backFace[30];
    
    unsigned int indices[6];

};
const int atlasWidth = 24;
const int atlasHeight = 8;
const int numTextures = 3;
// Calculate the width and height of each individual texture
const float textureWidth = 1.0f / (float)numTextures;
const float textureHeight = 1.0f;

struct vec2 faceTextureCoords[3];

void _initTexCoords() {
    // Calculate texture coordinates for each texture
    // Calculate texture coordinates for each texture
    for (int i = 0; i < numTextures; ++i) {
        // Calculate the left and right coordinates for the current texture
        float left = i * textureWidth;
        float right = (i + 1) * textureWidth;

        // Set the texture coordinates for the current texture
        faceTextureCoords[i].x = left;
        faceTextureCoords[i].y = 0.0f;  // Assuming the textures are aligned at the top-left corner
    }
}



const float bFace[30] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
};

const float fFace[30] = {
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
};

const float wFace[30] = {
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
};

const float eFace[30] = {
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
};

const float sFace[30] = {
    -0.5f, -0.5f, -0.5f,  faceTextureCoords[2].x, faceTextureCoords[2].y,
     0.5f, -0.5f, -0.5f,  faceTextureCoords[2].x + textureWidth, faceTextureCoords[2].y,
     0.5f, -0.5f,  0.5f,  faceTextureCoords[2].x + textureWidth, faceTextureCoords[2].y + textureHeight,
     0.5f, -0.5f,  0.5f,  faceTextureCoords[2].x + textureWidth, faceTextureCoords[2].y + textureHeight,
    -0.5f, -0.5f,  0.5f,  faceTextureCoords[2].x, faceTextureCoords[2].y + textureHeight,
    -0.5f, -0.5f, -0.5f,  faceTextureCoords[2].x, faceTextureCoords[2].y,
};

float nFace[30] = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

void _InitVoxel(struct Voxel* voxel) {
    
    memcpy(voxel->northFace, nFace, sizeof(nFace));

    memcpy(voxel->eastFace, eFace, sizeof(eFace));

    memcpy(voxel->southFace, sFace, sizeof(sFace));

    memcpy(voxel->westFace, wFace, sizeof(wFace));

    memcpy(voxel->frontFace, fFace, sizeof(fFace));

    memcpy(voxel->backFace, bFace, sizeof(bFace));

    memcpy(voxel->indices, indices, sizeof(indices));

}
