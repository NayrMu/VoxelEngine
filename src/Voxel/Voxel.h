#pragma once 
#ifdef __cplusplus
extern "C" {
#endif

struct Voxel {

    float northFace[12];

    float eastFace[12];

    /*struct Face southFace = {
        { 0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f}
    };

    struct Face westFace = {
        {-0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f}
    };

    struct Face frontFace = {
        {-0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         -0.5f,  0.5f, -0.5f}
    };

    struct Face backFace = {
        {-0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5,
        -0.5f, -0.5f,  0.5f}
    };*/

    unsigned int indices[6];

};

void _InitVoxel(struct Voxel* voxel);

#ifdef __cplusplus
}
#endif