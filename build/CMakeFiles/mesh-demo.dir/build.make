# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.6/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.6/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gulesh_shukla/cs312/Plane_Contrails

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gulesh_shukla/cs312/Plane_Contrails/build

# Include any dependencies generated for this target.
include CMakeFiles/mesh-demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mesh-demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mesh-demo.dir/flags.make

CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.o: CMakeFiles/mesh-demo.dir/flags.make
CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.o: ../src/meshdemo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gulesh_shukla/cs312/Plane_Contrails/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.o -c /Users/gulesh_shukla/cs312/Plane_Contrails/src/meshdemo.cpp

CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gulesh_shukla/cs312/Plane_Contrails/src/meshdemo.cpp > CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.i

CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gulesh_shukla/cs312/Plane_Contrails/src/meshdemo.cpp -o CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.s

# Object files for target mesh-demo
mesh__demo_OBJECTS = \
"CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.o"

# External object files for target mesh-demo
mesh__demo_EXTERNAL_OBJECTS =

../bin/mesh-demo: CMakeFiles/mesh-demo.dir/src/meshdemo.cpp.o
../bin/mesh-demo: CMakeFiles/mesh-demo.dir/build.make
../bin/mesh-demo: /usr/local/lib/libglfw.dylib
../bin/mesh-demo: CMakeFiles/mesh-demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/gulesh_shukla/cs312/Plane_Contrails/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/mesh-demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mesh-demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mesh-demo.dir/build: ../bin/mesh-demo

.PHONY : CMakeFiles/mesh-demo.dir/build

CMakeFiles/mesh-demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mesh-demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mesh-demo.dir/clean

CMakeFiles/mesh-demo.dir/depend:
	cd /Users/gulesh_shukla/cs312/Plane_Contrails/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gulesh_shukla/cs312/Plane_Contrails /Users/gulesh_shukla/cs312/Plane_Contrails /Users/gulesh_shukla/cs312/Plane_Contrails/build /Users/gulesh_shukla/cs312/Plane_Contrails/build /Users/gulesh_shukla/cs312/Plane_Contrails/build/CMakeFiles/mesh-demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mesh-demo.dir/depend

