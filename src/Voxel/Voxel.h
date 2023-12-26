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
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
};

float nFace[30] = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

/*const float nFace[20] = {
    -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,
    0.5f, 0.5f, -0.5f,     1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
};

const float eFace[20] = {
    0.5f, 0.5f, -0.5f,     0.0f, 1.0f,
    0.5f, 0.5f, 0.5f,      1.0f, 1.0f,
    0.5f, -0.5f, 0.5f,     1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,    0.0f, 0.0f
};

const float sFace[20] = {
    0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,    0.0f, 0.0f
};

const float wFace[20] = {
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f
};

const float fFace[20] = {
    -0.5f,  0.5f,  -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f,  -0.5f,   1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f,   1.0f, 0.0f,
    -0.5f,  -0.5f, -0.5f,  0.0f, 0.0f
};

float bFace[20] = {
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
    0.5f, -0.5f,  0.5,     1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f
};*/


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
