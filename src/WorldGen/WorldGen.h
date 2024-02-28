#pragma once

#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include <thread>
#include "../Constants.h"
#include "../GMath/GMath.h"
#include "../Voxel/Voxel.h"
#include "../Noise/noise.h"
#include "../Test/TimeTest.h"

#define isT_Edge(x) (x == C_chunkSize - 1)
#define isB_Edge(x) (x == 0)

#define T_Neighbor(x, y) (!(isT_Edge(x)) ? (isB_Edge(y)) : 0) 
#define B_Neighbor(x, y) (!(isB_Edge(x)) ? (isB_Edge(y)) : 0)

#define FASTFLOOR(x) ( ((int)(x)<(x)) ? ((int)x) : ((int)x-1 ) )

class ChunkManager {
private:
  Ivec3  m_currentChunk = {0, 0, 0};
  vec3 m_cameraPos;
  bool m_loadNewChunks = false;
  bool m_chunksLoaded = false;

  int m_numVerts;
  Voxel *m_UVoxI;
  Chunk *m_chunk;
  Shader *m_chunkShader;
  Shader *m_cullShader;
  std::vector<float> worldData;

  void chunkQueue() {
    while(true) {
      generate_chunks();
    }
  }

  void generateChunk(int chunkSize, int offsetX, int offsetZ, int offsetY, int seed) {
    double globalX;
    double globalZ;
    double globalY;
    int flatIndex = 0;
    float wavelength1 = C_chunkSize / 2.65f;
    float wavelength2 = C_chunkSize / 0.8f;
    GLint maxCount = 0;

    /*
    CHECK_GL_ERROR();
    glDispatchCompute(C_chunkSize / 8, C_chunkSize / 8, C_chunkSize / 8);
    GLsync syncObject = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    CHECK_GL_ERROR();
    glClientWaitSync(syncObject, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
    CHECK_GL_ERROR();
    */
    
    for (int i = 0; i < chunkSize; i++) {
      
      globalZ = (double)(i) + (offsetZ * C_chunkSize);
      float freqZ = globalZ / wavelength2;

      for (int j = 0; j < chunkSize; j++) {
        
        globalX = (double)(j) + (offsetX * C_chunkSize);
        float freqX = globalX / wavelength2;

        float noise1 = (noise2(freqX, freqZ) + 0.6f);

        for (int k = 0; k < chunkSize; k++) {
          globalY = (double)(k) + (offsetY * C_chunkSize);
          float freqY = globalY / wavelength1;

          flatIndex = i * (chunkSize * chunkSize) + (j * chunkSize) + k;

          float noise2 = noise3(globalX / wavelength1, globalZ / wavelength1, freqY) + 0.6f;
          
          int height = LERP(0.6, noise1, (noise2 + noise1)) * 80;
          //int height = Cshader.outPtr[flatIndex];
          
          if (k < height) {
            m_chunk->chunk[flatIndex] = 1;
          }
        }
      }
    }
    //glDeleteSync(syncObject);
  }

  void pushFace(std::vector<float>* chunkData, Voxel voxel, vec3 transform, int face) {
    float* faceArray = nullptr;
    if (face & 32) {
      faceArray = voxel.getNFace(); 
      translateQuad(faceArray, transform);
      chunkData->insert(chunkData->end(), faceArray, faceArray + 30);
    }
    if (face & 16) {
      faceArray = voxel.getSFace();
      translateQuad(faceArray, transform);
      chunkData->insert(chunkData->end(), faceArray, faceArray + 30);
    }
    if (face & 8) {
      faceArray = voxel.getWFace();
      translateQuad(faceArray, transform);
      //chunkData->insert(chunkData->end(), faceArray, faceArray + 30);
    }
    if (face & 4) {
      faceArray = voxel.getEFace();
      translateQuad(faceArray, transform);
      //chunkData->insert(chunkData->end(), faceArray, faceArray + 30);
    }
    if (face & 2) {
      faceArray = voxel.getFFace();
      translateQuad(faceArray, transform);
      //chunkData->insert(chunkData->end(), faceArray, faceArray + 30);
    }
    if (face & 1) {
      faceArray = m_UVoxI->getBFace();
      translateQuad(faceArray, transform);
      if (faceArray) {
        
      }
      //chunkData->insert(chunkData->end(), faceArray, faceArray + 30);
    }
    return;
  }

  void cullChunk(std::vector<float>* chunkData, Chunk Chunk, int chunkSize, Voxel *voxel) {
    /*
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, *CullShader.inBuff);
    CHECK_GL_ERROR();
    int *ptr = Chunk.chunk.data();
    int size = Chunk.chunk.size();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size * sizeof(float), ptr);
    CHECK_GL_ERROR();
    glDispatchCompute(C_chunkSize / 8, C_chunkSize / 8, C_chunkSize / 8);
    GLsync syncObject = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    CHECK_GL_ERROR();
    glClientWaitSync(syncObject, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
    CHECK_GL_ERROR();
    */
    int offsettXAdd = (Chunk.offsetX*chunkSize);
    int offsettYAdd = (Chunk.offsetY*chunkSize);
    int offsettZAdd = (Chunk.offsetZ*chunkSize);
    int flatIndex = 0;
    struct vec3 transform;
    for (int i = 0; i < chunkSize; i++) {
      
      transform.z = 0.5f*((float)i + offsettZAdd);

      for (int j = 0; j < chunkSize; j++) {

        transform.x = 0.5f*((float)j + offsettXAdd);

        for (int k = 0; k < chunkSize; k++) {

          transform.y = 0.5f*((float)k + offsettYAdd);
          flatIndex = i * (chunkSize * chunkSize) + (j * chunkSize) + k;
          int Jprev = flatIndex - chunkSize;
          int Jnext = flatIndex + chunkSize;
          int Iprev = flatIndex - (chunkSize*chunkSize);
          int Inext = flatIndex + (chunkSize*chunkSize);
          int Kprev = flatIndex - 1;
          int Knext = flatIndex + 1;
          
          if (k < chunkSize - 1 && Chunk.chunk[flatIndex+1] != 0) {
            Chunk.chunk[flatIndex] = 2;
          }
          if (k < chunkSize - 6 && Chunk.chunk[flatIndex+6] != 0) {
            Chunk.chunk[flatIndex] = 3;
          }
          
          if (Chunk.chunk[flatIndex] != 0) {
            voxel->setTextureOffset(Chunk.chunk[flatIndex]);

            int edgeMask = (int(isT_Edge(k)) << 5) | (int(isB_Edge(k)) << 4) | (int(isB_Edge(j)) << 3)
                             | (int(isT_Edge(j)) << 2) | (int(isT_Edge(i)) << 1) | (int(isB_Edge(i)));
            int neighborMask = (int(T_Neighbor(k, Chunk.chunk[Knext])) << 5) | (int(B_Neighbor(k, Chunk.chunk[Kprev])) << 4)
                            | (int(B_Neighbor(j, Chunk.chunk[Jprev])) << 3) | (int(T_Neighbor(j, Chunk.chunk[Jnext])) << 2)
                             | (int(T_Neighbor(i, Chunk.chunk[Inext])) << 1) | int(B_Neighbor(i, Chunk.chunk[Iprev]));
            int notEdgeMask = edgeMask ^ 63;
            int cmp = (notEdgeMask & neighborMask);
            int pushMask = edgeMask | cmp;
            pushFace(chunkData, *voxel, transform, pushMask);
          }
        }
      }
    }
    //glDeleteSync(syncObject);
    //CHECK_GL_ERROR();
  }

  void updateChunks() {
    setCurrentChunk();
    int seed;
    int flatIndex = 0;
    std::vector<float> allData;
    allData.reserve(400000000);
    int chunkInfo[25] = {0};
    int worldSize = 5;
    int totalLoadedSize = 0;
    for (int i = 0; i < worldSize; i++) {
      for (int j = 0; j < worldSize; j++) {
        
        if (i == 0) {
          flatIndex = j;
        }
        else { flatIndex = i * worldSize + j; }
        
        this->m_chunk->offsetX = j + this->m_currentChunk.x - (worldSize / 2);
        this->m_chunk->offsetY = 0;
        this->m_chunk->offsetZ = i + this->m_currentChunk.z - (worldSize / 2);

        
        if (( (std::abs(this->m_chunk->offsetX) - std::abs( this->m_currentChunk.x)) > (worldSize / 2) ) 
        && (std::abs(this->m_chunk->offsetZ) - std::abs( this->m_currentChunk.z) > (worldSize / 2))) {
          printf("Skipped\n");
          continue;
        }
        
        generateChunk(C_chunkSize, this->m_chunk->offsetX, this->m_chunk->offsetZ, this->m_chunk->offsetY, seed);
        printf("naha\n");
        cullChunk(&allData, *(this->m_chunk), C_chunkSize, this->m_UVoxI);
        printf("noinoi\n");
        
        this->m_numVerts = allData.size();
        
        chunkInfo[flatIndex] = totalLoadedSize;
        
        totalLoadedSize += this->m_numVerts;
        
        worldData.insert(worldData.end(), std::make_move_iterator(allData.begin()), std::make_move_iterator(allData.end()));
        allData.clear();
        
        std::fill(this->m_chunk->chunk.begin(), this->m_chunk->chunk.end(), 0);
        
      }
    }
    this->m_numVerts = totalLoadedSize;
  }

  void __attribute__((optimize("O0"))) generate_chunks() {
    if (m_loadNewChunks) {
      printf("Running\n");
      double Time;
      startTimer(&Time);
      this->updateChunks();
      endTimer(&Time);
      this->m_loadNewChunks = false;
      this->m_chunksLoaded = true;
    }
  }
public:
  ChunkManager(Voxel *voxel, Chunk* chunk) {
    this->m_UVoxI = voxel;
    this->m_chunk = chunk;
  }
  void init_chunk_manager() {
    std::thread t(ChunkManager::chunkQueue, this);
    t.detach();
  }
  void signal_load() {
    if (!(this->m_loadNewChunks)) {
      this->m_loadNewChunks = true;
    }
  }
  Ivec3 getCurrenChunk() {
    return m_currentChunk;
  }
  vec3 getCameraPos() {
    return m_cameraPos;
  }
  int getNumVerts() {
    return m_numVerts;
  }
  std::vector<float> get_world_data() {
    return worldData;
  }
  bool get_is_chunksloaded() {
    return m_chunksLoaded;
  }

  void updateState(Ivec3 cC, vec3 cP) {
    this->m_currentChunk = cC;
    this->m_cameraPos = cP;
  }
  void setCurrentChunk() {
    this->m_currentChunk.x = ((int)(this->m_cameraPos.x * bsConvert) / C_chunkSize);
    if (this->m_cameraPos.x < 0) {
      this->m_currentChunk.x -=1;
    }
    this->m_currentChunk.y = ((int)(this->m_cameraPos.y * bsConvert) / C_chunkSize);
    if (this->m_cameraPos.y < 0) {
      this->m_currentChunk.y -=1;
    }
    this->m_currentChunk.z = ((int)(this->m_cameraPos.z * bsConvert ) / C_chunkSize);
    if (this->m_cameraPos.z < 0) {
      this->m_currentChunk.z -=1;
    }
  }
  void set_is_chunks_loaded() {
    this->m_chunksLoaded = false;
  }
  void clear_world_data() {
    this->worldData.clear();
  }
  
};

unsigned int seedGen() {
  unsigned int newNum = rand() % 1000;
  newNum += 620;

  newNum *= 3;

  newNum = newNum & 620;

  return newNum;
}









