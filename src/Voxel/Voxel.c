#include "Voxel.h"
#include <string.h>

const float nFace[12] = {
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f 
};

const float temp[12] = {
    -0.05f, 0.05f, -0.05f,
    0.05f, 0.05f, -0.05f,
    0.05f, -0.05f, -0.05f,
    -0.05f, -0.05f, -0.05f 
};

const float eFace[12] = {
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f
};

unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

void _InitVoxel(struct Voxel* voxel) {
    
    memcpy(voxel->northFace, temp, sizeof(temp));

    memcpy(voxel->eastFace, eFace, sizeof(eFace));

    memcpy(voxel->indices, indices, sizeof(indices));

}