#pragma once

extern const int C_chunkSize;
extern const float C_blockSize;
extern const int bsConvert;

#define CHECK_GL_ERROR() \
do {\
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        std::cerr << "OpenGL error: " << err << std::endl; \
    } \
} while (0)
