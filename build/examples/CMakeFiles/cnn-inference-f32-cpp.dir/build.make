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
include examples/CMakeFiles/cnn-inference-f32-cpp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include examples/CMakeFiles/cnn-inference-f32-cpp.dir/compiler_depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/cnn-inference-f32-cpp.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/cnn-inference-f32-cpp.dir/flags.make

examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o: examples/CMakeFiles/cnn-inference-f32-cpp.dir/flags.make
examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o: /home/vineel/tf_onednn/oneDNN/examples/cnn_inference_f32.cpp
examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o: examples/CMakeFiles/cnn-inference-f32-cpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o -MF CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o.d -o CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o -c /home/vineel/tf_onednn/oneDNN/examples/cnn_inference_f32.cpp

examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.i"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vineel/tf_onednn/oneDNN/examples/cnn_inference_f32.cpp > CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.i

examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.s"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vineel/tf_onednn/oneDNN/examples/cnn_inference_f32.cpp -o CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.s

# Object files for target cnn-inference-f32-cpp
cnn__inference__f32__cpp_OBJECTS = \
"CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o"

# External object files for target cnn-inference-f32-cpp
cnn__inference__f32__cpp_EXTERNAL_OBJECTS =

examples/cnn-inference-f32-cpp: examples/CMakeFiles/cnn-inference-f32-cpp.dir/cnn_inference_f32.cpp.o
examples/cnn-inference-f32-cpp: examples/CMakeFiles/cnn-inference-f32-cpp.dir/build.make
examples/cnn-inference-f32-cpp: src/libdnnl.so.3.2
examples/cnn-inference-f32-cpp: /usr/lib/aarch64-linux-gnu/libm.so
examples/cnn-inference-f32-cpp: examples/CMakeFiles/cnn-inference-f32-cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cnn-inference-f32-cpp"
	cd /home/vineel/tf_onednn/oneDNN/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cnn-inference-f32-cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/cnn-inference-f32-cpp.dir/build: examples/cnn-inference-f32-cpp
.PHONY : examples/CMakeFiles/cnn-inference-f32-cpp.dir/build

examples/CMakeFiles/cnn-inference-f32-cpp.dir/clean:
	cd /home/vineel/tf_onednn/oneDNN/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/cnn-inference-f32-cpp.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/cnn-inference-f32-cpp.dir/clean

examples/CMakeFiles/cnn-inference-f32-cpp.dir/depend:
	cd /home/vineel/tf_onednn/oneDNN/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vineel/tf_onednn/oneDNN /home/vineel/tf_onednn/oneDNN/examples /home/vineel/tf_onednn/oneDNN/build /home/vineel/tf_onednn/oneDNN/build/examples /home/vineel/tf_onednn/oneDNN/build/examples/CMakeFiles/cnn-inference-f32-cpp.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : examples/CMakeFiles/cnn-inference-f32-cpp.dir/depend

