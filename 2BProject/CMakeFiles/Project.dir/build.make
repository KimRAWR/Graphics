# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.1

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/karre/Graphics/2BProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/karre/Graphics/2BProject

# Include any dependencies generated for this target.
include CMakeFiles/Project.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Project.dir/flags.make

CMakeFiles/Project.dir/source/main.cpp.o: CMakeFiles/Project.dir/flags.make
CMakeFiles/Project.dir/source/main.cpp.o: source/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/karre/Graphics/2BProject/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Project.dir/source/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Project.dir/source/main.cpp.o -c /Users/karre/Graphics/2BProject/source/main.cpp

CMakeFiles/Project.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project.dir/source/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/karre/Graphics/2BProject/source/main.cpp > CMakeFiles/Project.dir/source/main.cpp.i

CMakeFiles/Project.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project.dir/source/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/karre/Graphics/2BProject/source/main.cpp -o CMakeFiles/Project.dir/source/main.cpp.s

CMakeFiles/Project.dir/source/main.cpp.o.requires:
.PHONY : CMakeFiles/Project.dir/source/main.cpp.o.requires

CMakeFiles/Project.dir/source/main.cpp.o.provides: CMakeFiles/Project.dir/source/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Project.dir/build.make CMakeFiles/Project.dir/source/main.cpp.o.provides.build
.PHONY : CMakeFiles/Project.dir/source/main.cpp.o.provides

CMakeFiles/Project.dir/source/main.cpp.o.provides.build: CMakeFiles/Project.dir/source/main.cpp.o

CMakeFiles/Project.dir/source/GLSL.cpp.o: CMakeFiles/Project.dir/flags.make
CMakeFiles/Project.dir/source/GLSL.cpp.o: source/GLSL.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/karre/Graphics/2BProject/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Project.dir/source/GLSL.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Project.dir/source/GLSL.cpp.o -c /Users/karre/Graphics/2BProject/source/GLSL.cpp

CMakeFiles/Project.dir/source/GLSL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project.dir/source/GLSL.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/karre/Graphics/2BProject/source/GLSL.cpp > CMakeFiles/Project.dir/source/GLSL.cpp.i

CMakeFiles/Project.dir/source/GLSL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project.dir/source/GLSL.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/karre/Graphics/2BProject/source/GLSL.cpp -o CMakeFiles/Project.dir/source/GLSL.cpp.s

CMakeFiles/Project.dir/source/GLSL.cpp.o.requires:
.PHONY : CMakeFiles/Project.dir/source/GLSL.cpp.o.requires

CMakeFiles/Project.dir/source/GLSL.cpp.o.provides: CMakeFiles/Project.dir/source/GLSL.cpp.o.requires
	$(MAKE) -f CMakeFiles/Project.dir/build.make CMakeFiles/Project.dir/source/GLSL.cpp.o.provides.build
.PHONY : CMakeFiles/Project.dir/source/GLSL.cpp.o.provides

CMakeFiles/Project.dir/source/GLSL.cpp.o.provides.build: CMakeFiles/Project.dir/source/GLSL.cpp.o

CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o: CMakeFiles/Project.dir/flags.make
CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o: source/tiny_obj_loader.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/karre/Graphics/2BProject/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o -c /Users/karre/Graphics/2BProject/source/tiny_obj_loader.cc

CMakeFiles/Project.dir/source/tiny_obj_loader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project.dir/source/tiny_obj_loader.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/karre/Graphics/2BProject/source/tiny_obj_loader.cc > CMakeFiles/Project.dir/source/tiny_obj_loader.cc.i

CMakeFiles/Project.dir/source/tiny_obj_loader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project.dir/source/tiny_obj_loader.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/karre/Graphics/2BProject/source/tiny_obj_loader.cc -o CMakeFiles/Project.dir/source/tiny_obj_loader.cc.s

CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.requires:
.PHONY : CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.requires

CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.provides: CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.requires
	$(MAKE) -f CMakeFiles/Project.dir/build.make CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.provides.build
.PHONY : CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.provides

CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.provides.build: CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o

CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o: CMakeFiles/Project.dir/flags.make
CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o: dependencies/glad/src/glad.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/karre/Graphics/2BProject/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o   -c /Users/karre/Graphics/2BProject/dependencies/glad/src/glad.c

CMakeFiles/Project.dir/dependencies/glad/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Project.dir/dependencies/glad/src/glad.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/karre/Graphics/2BProject/dependencies/glad/src/glad.c > CMakeFiles/Project.dir/dependencies/glad/src/glad.c.i

CMakeFiles/Project.dir/dependencies/glad/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Project.dir/dependencies/glad/src/glad.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/karre/Graphics/2BProject/dependencies/glad/src/glad.c -o CMakeFiles/Project.dir/dependencies/glad/src/glad.c.s

CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.requires:
.PHONY : CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.requires

CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.provides: CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.requires
	$(MAKE) -f CMakeFiles/Project.dir/build.make CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.provides.build
.PHONY : CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.provides

CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.provides.build: CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o

# Object files for target Project
Project_OBJECTS = \
"CMakeFiles/Project.dir/source/main.cpp.o" \
"CMakeFiles/Project.dir/source/GLSL.cpp.o" \
"CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o" \
"CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o"

# External object files for target Project
Project_EXTERNAL_OBJECTS =

Project: CMakeFiles/Project.dir/source/main.cpp.o
Project: CMakeFiles/Project.dir/source/GLSL.cpp.o
Project: CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o
Project: CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o
Project: CMakeFiles/Project.dir/build.make
Project: dependencies/glfw/src/libglfw3.a
Project: CMakeFiles/Project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Project"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Project.dir/build: Project
.PHONY : CMakeFiles/Project.dir/build

CMakeFiles/Project.dir/requires: CMakeFiles/Project.dir/source/main.cpp.o.requires
CMakeFiles/Project.dir/requires: CMakeFiles/Project.dir/source/GLSL.cpp.o.requires
CMakeFiles/Project.dir/requires: CMakeFiles/Project.dir/source/tiny_obj_loader.cc.o.requires
CMakeFiles/Project.dir/requires: CMakeFiles/Project.dir/dependencies/glad/src/glad.c.o.requires
.PHONY : CMakeFiles/Project.dir/requires

CMakeFiles/Project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Project.dir/clean

CMakeFiles/Project.dir/depend:
	cd /Users/karre/Graphics/2BProject && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/karre/Graphics/2BProject /Users/karre/Graphics/2BProject /Users/karre/Graphics/2BProject /Users/karre/Graphics/2BProject /Users/karre/Graphics/2BProject/CMakeFiles/Project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Project.dir/depend

