# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\rkmun\source\repos\VoxelEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\rkmun\source\repos\VoxelEngine\build

# Include any dependencies generated for this target.
include src/CMakeFiles/source_files.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/source_files.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/source_files.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/source_files.dir/flags.make

src/CMakeFiles/source_files.dir/main.cpp.obj: src/CMakeFiles/source_files.dir/flags.make
src/CMakeFiles/source_files.dir/main.cpp.obj: src/CMakeFiles/source_files.dir/includes_CXX.rsp
src/CMakeFiles/source_files.dir/main.cpp.obj: C:/Users/rkmun/source/repos/VoxelEngine/src/main.cpp
src/CMakeFiles/source_files.dir/main.cpp.obj: src/CMakeFiles/source_files.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\rkmun\source\repos\VoxelEngine\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/source_files.dir/main.cpp.obj"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/source_files.dir/main.cpp.obj -MF CMakeFiles\source_files.dir\main.cpp.obj.d -o CMakeFiles\source_files.dir\main.cpp.obj -c C:\Users\rkmun\source\repos\VoxelEngine\src\main.cpp

src/CMakeFiles/source_files.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/source_files.dir/main.cpp.i"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\rkmun\source\repos\VoxelEngine\src\main.cpp > CMakeFiles\source_files.dir\main.cpp.i

src/CMakeFiles/source_files.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/source_files.dir/main.cpp.s"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\rkmun\source\repos\VoxelEngine\src\main.cpp -o CMakeFiles\source_files.dir\main.cpp.s

src/CMakeFiles/source_files.dir/glad.c.obj: src/CMakeFiles/source_files.dir/flags.make
src/CMakeFiles/source_files.dir/glad.c.obj: src/CMakeFiles/source_files.dir/includes_C.rsp
src/CMakeFiles/source_files.dir/glad.c.obj: C:/Users/rkmun/source/repos/VoxelEngine/src/glad.c
src/CMakeFiles/source_files.dir/glad.c.obj: src/CMakeFiles/source_files.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\rkmun\source\repos\VoxelEngine\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/source_files.dir/glad.c.obj"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/source_files.dir/glad.c.obj -MF CMakeFiles\source_files.dir\glad.c.obj.d -o CMakeFiles\source_files.dir\glad.c.obj -c C:\Users\rkmun\source\repos\VoxelEngine\src\glad.c

src/CMakeFiles/source_files.dir/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/source_files.dir/glad.c.i"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\rkmun\source\repos\VoxelEngine\src\glad.c > CMakeFiles\source_files.dir\glad.c.i

src/CMakeFiles/source_files.dir/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/source_files.dir/glad.c.s"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\rkmun\source\repos\VoxelEngine\src\glad.c -o CMakeFiles\source_files.dir\glad.c.s

src/CMakeFiles/source_files.dir/Voxel/Voxel.c.obj: src/CMakeFiles/source_files.dir/flags.make
src/CMakeFiles/source_files.dir/Voxel/Voxel.c.obj: src/CMakeFiles/source_files.dir/includes_C.rsp
src/CMakeFiles/source_files.dir/Voxel/Voxel.c.obj: C:/Users/rkmun/source/repos/VoxelEngine/src/Voxel/Voxel.c
src/CMakeFiles/source_files.dir/Voxel/Voxel.c.obj: src/CMakeFiles/source_files.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\rkmun\source\repos\VoxelEngine\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/CMakeFiles/source_files.dir/Voxel/Voxel.c.obj"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/source_files.dir/Voxel/Voxel.c.obj -MF CMakeFiles\source_files.dir\Voxel\Voxel.c.obj.d -o CMakeFiles\source_files.dir\Voxel\Voxel.c.obj -c C:\Users\rkmun\source\repos\VoxelEngine\src\Voxel\Voxel.c

src/CMakeFiles/source_files.dir/Voxel/Voxel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/source_files.dir/Voxel/Voxel.c.i"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\rkmun\source\repos\VoxelEngine\src\Voxel\Voxel.c > CMakeFiles\source_files.dir\Voxel\Voxel.c.i

src/CMakeFiles/source_files.dir/Voxel/Voxel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/source_files.dir/Voxel/Voxel.c.s"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\rkmun\source\repos\VoxelEngine\src\Voxel\Voxel.c -o CMakeFiles\source_files.dir\Voxel\Voxel.c.s

# Object files for target source_files
source_files_OBJECTS = \
"CMakeFiles/source_files.dir/main.cpp.obj" \
"CMakeFiles/source_files.dir/glad.c.obj" \
"CMakeFiles/source_files.dir/Voxel/Voxel.c.obj"

# External object files for target source_files
source_files_EXTERNAL_OBJECTS =

src/source_files.exe: src/CMakeFiles/source_files.dir/main.cpp.obj
src/source_files.exe: src/CMakeFiles/source_files.dir/glad.c.obj
src/source_files.exe: src/CMakeFiles/source_files.dir/Voxel/Voxel.c.obj
src/source_files.exe: src/CMakeFiles/source_files.dir/build.make
src/source_files.exe: src/CMakeFiles/source_files.dir/linkLibs.rsp
src/source_files.exe: src/CMakeFiles/source_files.dir/objects1.rsp
src/source_files.exe: src/CMakeFiles/source_files.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\rkmun\source\repos\VoxelEngine\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable source_files.exe"
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\source_files.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/source_files.dir/build: src/source_files.exe
.PHONY : src/CMakeFiles/source_files.dir/build

src/CMakeFiles/source_files.dir/clean:
	cd /d C:\Users\rkmun\source\repos\VoxelEngine\build\src && $(CMAKE_COMMAND) -P CMakeFiles\source_files.dir\cmake_clean.cmake
.PHONY : src/CMakeFiles/source_files.dir/clean

src/CMakeFiles/source_files.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\rkmun\source\repos\VoxelEngine C:\Users\rkmun\source\repos\VoxelEngine\src C:\Users\rkmun\source\repos\VoxelEngine\build C:\Users\rkmun\source\repos\VoxelEngine\build\src C:\Users\rkmun\source\repos\VoxelEngine\build\src\CMakeFiles\source_files.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/source_files.dir/depend

