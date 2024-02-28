# VoxelEngine
 Voxel Engine in C/C++ with opengl.

## Demonstration: 
<img src='https://github.com/NayrMu/VoxelEngine/blob/main/src/MineCloneV0.7.gif' title='Video Demo' width='' alt='Video Demo' />

## Performance:
With -O3 optimization, it averages about 134 fps with a world size of 15x15 chunks at 128x128x128 chunk size at ~400,000,000 voxels. It takes ~14 seconds to generate the whole world which is done asynchronously. 
It is significantly quicker with a world size of 7x7 chunks only taking ~3 seconds to generate.

## Short Term Goals:
- Lighting
- Collisions
- Vegitation

## Long Term Goals:
- ECS (Entity Component System)
- Biomes
- Gameplay
- Networking?
