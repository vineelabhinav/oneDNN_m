# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vineel/tf_onednn/oneDNN

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vineel/tf_onednn/oneDNN/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/primitives-layer-normalization-cpp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include examples/CMakeFiles/primitives-layer-normalization-cpp.dir/compiler_depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/primitives-layer-normalization-cpp.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/primitives-layer-normalization-cpp.dir/flags.make

examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o: examples/CMakeFiles/primitives-layer-normalization-cpp.dir/flags.make
examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o: /home/vineel/tf_onednn/oneDNN/examples/primitives/layer_normalization.cpp
examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o: examples/CMakeFiles/primitives-layer-normalization-cpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o -MF CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o.d -o CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o -c /home/vineel/tf_onednn/oneDNN/examples/primitives/layer_normalization.cpp

examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.i"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vineel/tf_onednn/oneDNN/examples/primitives/layer_normalization.cpp > CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.i

examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.s"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vineel/tf_onednn/oneDNN/examples/primitives/layer_normalization.cpp -o CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.s

# Object files for target primitives-layer-normalization-cpp
primitives__layer__normalization__cpp_OBJECTS = \
"CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o"

# External object files for target primitives-layer-normalization-cpp
primitives__layer__normalization__cpp_EXTERNAL_OBJECTS =

examples/primitives-layer-normalization-cpp: examples/CMakeFiles/primitives-layer-normalization-cpp.dir/primitives/layer_normalization.cpp.o
examples/primitives-layer-normalization-cpp: examples/CMakeFiles/primitives-layer-normalization-cpp.dir/build.make
examples/primitives-layer-normalization-cpp: src/libdnnl.so.3.2
examples/primitives-layer-normalization-cpp: /usr/lib/aarch64-linux-gnu/libm.so
examples/primitives-layer-normalization-cpp: examples/CMakeFiles/primitives-layer-normalization-cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable primitives-layer-normalization-cpp"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/primitives-layer-normalization-cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/primitives-layer-normalization-cpp.dir/build: examples/primitives-layer-normalization-cpp
.PHONY : examples/CMakeFiles/primitives-layer-normalization-cpp.dir/build

examples/CMakeFiles/primitives-layer-normalization-cpp.dir/clean:
	cd /home/vineel/tf_onednn/oneDNN/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/primitives-layer-normalization-cpp.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/primitives-layer-normalization-cpp.dir/clean

examples/CMakeFiles/primitives-layer-normalization-cpp.dir/depend:
	cd /home/vineel/tf_onednn/oneDNN/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vineel/tf_onednn/oneDNN /home/vineel/tf_onednn/oneDNN/examples /home/vineel/tf_onednn/oneDNN/build /home/vineel/tf_onednn/oneDNN/build/examples /home/vineel/tf_onednn/oneDNN/build/examples/CMakeFiles/primitives-layer-normalization-cpp.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : examples/CMakeFiles/primitives-layer-normalization-cpp.dir/depend

