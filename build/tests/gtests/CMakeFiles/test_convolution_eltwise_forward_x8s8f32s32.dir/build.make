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
include tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/progress.make

# Include the compile flags for this target's objects.
include tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/flags.make

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/flags.make
tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o: /home/vineel/tf_onednn/oneDNN/tests/gtests/main.cpp
tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o -MF CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o.d -o CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o -c /home/vineel/tf_onednn/oneDNN/tests/gtests/main.cpp

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.i"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vineel/tf_onednn/oneDNN/tests/gtests/main.cpp > CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.i

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.s"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vineel/tf_onednn/oneDNN/tests/gtests/main.cpp -o CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.s

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/flags.make
tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o: /home/vineel/tf_onednn/oneDNN/tests/test_thread.cpp
tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o -MF CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o.d -o CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o -c /home/vineel/tf_onednn/oneDNN/tests/test_thread.cpp

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.i"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vineel/tf_onednn/oneDNN/tests/test_thread.cpp > CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.i

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.s"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vineel/tf_onednn/oneDNN/tests/test_thread.cpp -o CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.s

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/flags.make
tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o: /home/vineel/tf_onednn/oneDNN/tests/gtests/test_convolution_eltwise_forward_x8s8f32s32.cpp
tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o -MF CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o.d -o CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o -c /home/vineel/tf_onednn/oneDNN/tests/gtests/test_convolution_eltwise_forward_x8s8f32s32.cpp

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.i"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vineel/tf_onednn/oneDNN/tests/gtests/test_convolution_eltwise_forward_x8s8f32s32.cpp > CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.i

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.s"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vineel/tf_onednn/oneDNN/tests/gtests/test_convolution_eltwise_forward_x8s8f32s32.cpp -o CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.s

# Object files for target test_convolution_eltwise_forward_x8s8f32s32
test_convolution_eltwise_forward_x8s8f32s32_OBJECTS = \
"CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o" \
"CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o" \
"CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o"

# External object files for target test_convolution_eltwise_forward_x8s8f32s32
test_convolution_eltwise_forward_x8s8f32s32_EXTERNAL_OBJECTS =

tests/gtests/test_convolution_eltwise_forward_x8s8f32s32: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/main.cpp.o
tests/gtests/test_convolution_eltwise_forward_x8s8f32s32: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/__/test_thread.cpp.o
tests/gtests/test_convolution_eltwise_forward_x8s8f32s32: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/test_convolution_eltwise_forward_x8s8f32s32.cpp.o
tests/gtests/test_convolution_eltwise_forward_x8s8f32s32: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/build.make
tests/gtests/test_convolution_eltwise_forward_x8s8f32s32: src/libdnnl.so.3.2
tests/gtests/test_convolution_eltwise_forward_x8s8f32s32: tests/gtests/gtest/libdnnl_gtest.a
tests/gtests/test_convolution_eltwise_forward_x8s8f32s32: tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/vineel/tf_onednn/oneDNN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable test_convolution_eltwise_forward_x8s8f32s32"
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/build: tests/gtests/test_convolution_eltwise_forward_x8s8f32s32
.PHONY : tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/build

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/clean:
	cd /home/vineel/tf_onednn/oneDNN/build/tests/gtests && $(CMAKE_COMMAND) -P CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/cmake_clean.cmake
.PHONY : tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/clean

tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/depend:
	cd /home/vineel/tf_onednn/oneDNN/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vineel/tf_onednn/oneDNN /home/vineel/tf_onednn/oneDNN/tests/gtests /home/vineel/tf_onednn/oneDNN/build /home/vineel/tf_onednn/oneDNN/build/tests/gtests /home/vineel/tf_onednn/oneDNN/build/tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/gtests/CMakeFiles/test_convolution_eltwise_forward_x8s8f32s32.dir/depend

