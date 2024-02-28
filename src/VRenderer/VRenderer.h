#pragma once

#include "../Constants.h"
#include "../GMath/GMath.h"
#include "../WorldGen/WorldGen.h"
#include "../Voxel/Voxel.h"
#include "../Shader/Shader.h"
#include "../Camera/Camera.h"

MyCamera Camera(0, 0);

class VRenderer {
private:
  const char *vertexShaderSource = 
    "#version 440 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 texCoord;\n"
    "out vec2 TexCoord;\n"
    "out vec3 FragPos;\n"
    "out vec3 CameraPos;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform vec3 cameraPos;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec3 CameraPos = cameraPos;\n"
    "   vec4 WorldPos = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "   FragPos = vec3(WorldPos);\n"
    "   gl_Position = projection * view * WorldPos;\n"
    "   TexCoord = vec2(texCoord.x, texCoord.y);\n"
    "}\0";
  const char *fragmentShaderSouce = 
    "#version 440 core\n"
    "in vec2 TexCoord;\n"
    "in vec3 FragPos;\n"
    "in vec3 CameraPos;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "    float distFromCam = ((abs(CameraPos.z - FragPos.z) + abs(CameraPos.x - FragPos.x)) / 2.0);\n"
    "    vec4 TexColor = texture(ourTexture, TexCoord);\n"
    "    float atmosphereHeight = 60.0;\n"
    "    if (distFromCam > 130) {\n;"
    "       float offsetDistance = distFromCam - 130;\n"
    "       float rayleighCoefficient = 0.25 * exp(abs(offsetDistance) / atmosphereHeight);\n"
    "       vec3 scatteringColor = vec3(0.4, 0.5, 0.9);\n"
    "       vec3 scatteredLight = TexColor.rgb + (0.14 * ((rayleighCoefficient * scatteringColor) + TexColor.rgb)) ;\n"
    "       FragColor = vec4(scatteredLight, TexColor.a);\n"
    "    }\n"
    "    else { FragColor = TexColor; }\n"
    "}\n";

  int SCR_WIDTH;
  int SCR_HEIGHT;
  
  int prevChunkX;
  int prevChunkY;
  int prevChunkZ;

  int numVerts;

  float currentTime;
  float elapsedTime;
  float lastTime;
  float deltaTime;
  float lastFrame;

  Voxel *UVoxi = new Voxel;
  Chunk *chunk = new Chunk;

  Shader shader;
  unsigned int inBuff;
  unsigned int outBuff;
  unsigned int chunkVBO, chunkVAO;
  unsigned int maxLoadSize = 999999999 * sizeof(float);
  unsigned int textureGrass;
  const char* grassFile = "../../src/Game16x16.png";

  struct mat4 modelMatrix;
  struct mat4 projectionMat;

  void chunk_checker(ChunkManager *ChunkManager, MyCamera *Camera) {
    ChunkManager->updateState(ChunkManager->getCurrenChunk(), Camera->get_cam_pos());
    ChunkManager->setCurrentChunk();
    ChunkManager->setCurrentChunk();
    Camera->set_cam_pos(ChunkManager->getCameraPos());  
  }
  void is_update_chunk(int prevChunkX, int prevChunkY, int prevChunkZ, ChunkManager *ChunkManager) {
    if ((prevChunkX != ChunkManager->getCurrenChunk().x) || (prevChunkY != ChunkManager->getCurrenChunk().y) || (prevChunkZ != ChunkManager->getCurrenChunk().z)) {
      ChunkManager->signal_load();
    }
  }
  void swap_memory(ChunkManager *ChunkManager) {
    if (ChunkManager->get_is_chunksloaded()) {
      int size = ChunkManager->get_world_data().size();
      
      glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
      float* mappedPtr = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, size * sizeof(float), GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_WRITE_BIT);
      if (mappedPtr) {
        memcpy(mappedPtr, ChunkManager->get_world_data().data(), size * sizeof(float));
        glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, size * sizeof(float));
        glUnmapBuffer(GL_ARRAY_BUFFER);
      }
      numVerts = ChunkManager->getNumVerts();
      ChunkManager->clear_world_data();
      ChunkManager->set_is_chunks_loaded();
    }
  }

  void set_uniforms() {
    Camera.set_lookVec(addVectors(Camera.get_cam_pos(), Camera.get_cam_front()));
    struct mat4 viewMat = makeLookAtMatrix(Camera.get_cam_pos(), Camera.get_lookVec(), Camera.get_yAxis());
    

    int viewLoc = glGetUniformLocation(shader.ID, "view");
    
    glUniformMatrix4fv(viewLoc, 1, GL_TRUE, (GLfloat*)&viewMat.m);
    
    int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, (GLfloat*)&projectionMat.m);
    
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (GLfloat*)&modelMatrix.m);
    

    Camera.set_uniforms(&shader);
    
  }
  
  void update_prevs(ChunkManager ChunkManager) {
    prevChunkX = ChunkManager.getCurrenChunk().x;
    prevChunkY = ChunkManager.getCurrenChunk().y;
    prevChunkZ = ChunkManager.getCurrenChunk().z;
  }

public:
  VRenderer() {
    int SCR_WIDTH = 900;
    int SCR_HEIGHT = 600;
    
    int prevChunkX = 1000000;
    int prevChunkY = 1000000;
    int prevChunkZ = 1000000;

    chunk->chunk.resize(C_chunkSize* C_chunkSize* C_chunkSize, 0);

    float currentTime;
    float elapsedTime;
    float lastTime;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    projectionMat = makeProjectionMatrix(110.0f, 0.1f, 100.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);
    modelMatrix = makeIdentityMatrix();

    shader = shader_create(vertexShaderSource, fragmentShaderSouce);
    glGenBuffers(1, &outBuff);
    glGenBuffers(1, &inBuff);
    glGenVertexArrays(1, &chunkVAO);
    glGenBuffers(1, &chunkVBO);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      std::cout << error << std::endl;
    }
    shader_ArrBuffs(chunkVAO, chunkVBO, nullptr, maxLoadSize);
    shader_addTexture(grassFile, &textureGrass);
    glBindTexture(GL_TEXTURE_2D, textureGrass);
    
  }


  void render_frame(ChunkManager *ChunkManager, MyCamera *Camera) {
    shader_use(&shader);
    
    set_uniforms();
    
    chunk_checker(ChunkManager, Camera);
    
    is_update_chunk(prevChunkX, prevChunkY, prevChunkZ, ChunkManager);
    
    swap_memory(ChunkManager);
    
    glDrawArrays(GL_TRIANGLES, 0, numVerts/5);
    update_prevs(*ChunkManager);
  }

  Voxel* get_voxel() {
    return UVoxi;
  }
  Chunk* get_chunk() {
    return chunk;
  }

  ~VRenderer() {
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &chunkVBO);
    glDeleteVertexArrays(1, &chunkVAO);
    glDeleteProgram((GLuint)shader.ID);
  }
  
};