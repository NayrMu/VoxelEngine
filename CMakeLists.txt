cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("VoxelEngine")

include_directories(source_files PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include)
link_directories(source_files PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib-mingw-w64)

add_subdirectory("src")