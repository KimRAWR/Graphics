# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /Users/kimrawr/Graphics/FinalProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kimrawr/Graphics/FinalProject

# Include any dependencies generated for this target.
include CMakeFiles/FinalProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FinalProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FinalProject.dir/flags.make

CMakeFiles/FinalProject.dir/source/main.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/source/main.cpp.o: source/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/FinalProject.dir/source/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FinalProject.dir/source/main.cpp.o -c /Users/kimrawr/Graphics/FinalProject/source/main.cpp

CMakeFiles/FinalProject.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/source/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/source/main.cpp > CMakeFiles/FinalProject.dir/source/main.cpp.i

CMakeFiles/FinalProject.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/source/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/source/main.cpp -o CMakeFiles/FinalProject.dir/source/main.cpp.s

CMakeFiles/FinalProject.dir/source/main.cpp.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/source/main.cpp.o.requires

CMakeFiles/FinalProject.dir/source/main.cpp.o.provides: CMakeFiles/FinalProject.dir/source/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/source/main.cpp.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/source/main.cpp.o.provides

CMakeFiles/FinalProject.dir/source/main.cpp.o.provides.build: CMakeFiles/FinalProject.dir/source/main.cpp.o

CMakeFiles/FinalProject.dir/source/GLSL.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/source/GLSL.cpp.o: source/GLSL.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/FinalProject.dir/source/GLSL.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FinalProject.dir/source/GLSL.cpp.o -c /Users/kimrawr/Graphics/FinalProject/source/GLSL.cpp

CMakeFiles/FinalProject.dir/source/GLSL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/source/GLSL.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/source/GLSL.cpp > CMakeFiles/FinalProject.dir/source/GLSL.cpp.i

CMakeFiles/FinalProject.dir/source/GLSL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/source/GLSL.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/source/GLSL.cpp -o CMakeFiles/FinalProject.dir/source/GLSL.cpp.s

CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.requires

CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.provides: CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.provides

CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.provides.build: CMakeFiles/FinalProject.dir/source/GLSL.cpp.o

CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o: source/tiny_obj_loader.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o -c /Users/kimrawr/Graphics/FinalProject/source/tiny_obj_loader.cc

CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/source/tiny_obj_loader.cc > CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.i

CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/source/tiny_obj_loader.cc -o CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.s

CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.requires

CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.provides: CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.provides

CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.provides.build: CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o

CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o: source/RenderingHelper.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o -c /Users/kimrawr/Graphics/FinalProject/source/RenderingHelper.cpp

CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/source/RenderingHelper.cpp > CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.i

CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/source/RenderingHelper.cpp -o CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.s

CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.requires

CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.provides: CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.provides

CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.provides.build: CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o

CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o: source/givenFunctions.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o -c /Users/kimrawr/Graphics/FinalProject/source/givenFunctions.cpp

CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/source/givenFunctions.cpp > CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.i

CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/source/givenFunctions.cpp -o CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.s

CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.requires

CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.provides: CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.provides

CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.provides.build: CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o

CMakeFiles/FinalProject.dir/source/Penguin.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/source/Penguin.cpp.o: source/Penguin.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/FinalProject.dir/source/Penguin.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FinalProject.dir/source/Penguin.cpp.o -c /Users/kimrawr/Graphics/FinalProject/source/Penguin.cpp

CMakeFiles/FinalProject.dir/source/Penguin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/source/Penguin.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/source/Penguin.cpp > CMakeFiles/FinalProject.dir/source/Penguin.cpp.i

CMakeFiles/FinalProject.dir/source/Penguin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/source/Penguin.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/source/Penguin.cpp -o CMakeFiles/FinalProject.dir/source/Penguin.cpp.s

CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.requires

CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.provides: CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.provides

CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.provides.build: CMakeFiles/FinalProject.dir/source/Penguin.cpp.o

CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o: source/shaderProgram.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o -c /Users/kimrawr/Graphics/FinalProject/source/shaderProgram.cpp

CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/source/shaderProgram.cpp > CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.i

CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/source/shaderProgram.cpp -o CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.s

CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.requires

CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.provides: CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.provides

CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.provides.build: CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o

CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o: dependencies/glad/src/glad.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kimrawr/Graphics/FinalProject/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o   -c /Users/kimrawr/Graphics/FinalProject/dependencies/glad/src/glad.c

CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/kimrawr/Graphics/FinalProject/dependencies/glad/src/glad.c > CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.i

CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/kimrawr/Graphics/FinalProject/dependencies/glad/src/glad.c -o CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.s

CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.requires:
.PHONY : CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.requires

CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.provides: CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.provides

CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.provides.build: CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o

# Object files for target FinalProject
FinalProject_OBJECTS = \
"CMakeFiles/FinalProject.dir/source/main.cpp.o" \
"CMakeFiles/FinalProject.dir/source/GLSL.cpp.o" \
"CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o" \
"CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o" \
"CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o" \
"CMakeFiles/FinalProject.dir/source/Penguin.cpp.o" \
"CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o" \
"CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o"

# External object files for target FinalProject
FinalProject_EXTERNAL_OBJECTS =

FinalProject: CMakeFiles/FinalProject.dir/source/main.cpp.o
FinalProject: CMakeFiles/FinalProject.dir/source/GLSL.cpp.o
FinalProject: CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o
FinalProject: CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o
FinalProject: CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o
FinalProject: CMakeFiles/FinalProject.dir/source/Penguin.cpp.o
FinalProject: CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o
FinalProject: CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o
FinalProject: CMakeFiles/FinalProject.dir/build.make
FinalProject: dependencies/glfw/src/libglfw3.a
FinalProject: CMakeFiles/FinalProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable FinalProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FinalProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FinalProject.dir/build: FinalProject
.PHONY : CMakeFiles/FinalProject.dir/build

CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/source/main.cpp.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/source/GLSL.cpp.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/source/tiny_obj_loader.cc.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/source/RenderingHelper.cpp.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/source/givenFunctions.cpp.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/source/Penguin.cpp.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/source/shaderProgram.cpp.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/dependencies/glad/src/glad.c.o.requires
.PHONY : CMakeFiles/FinalProject.dir/requires

CMakeFiles/FinalProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FinalProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FinalProject.dir/clean

CMakeFiles/FinalProject.dir/depend:
	cd /Users/kimrawr/Graphics/FinalProject && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kimrawr/Graphics/FinalProject /Users/kimrawr/Graphics/FinalProject /Users/kimrawr/Graphics/FinalProject /Users/kimrawr/Graphics/FinalProject /Users/kimrawr/Graphics/FinalProject/CMakeFiles/FinalProject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FinalProject.dir/depend

