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

# Utility rule file for test_benchdnn_ip_ci_cpu.

# Include any custom commands dependencies for this target.
include tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/progress.make

tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu: tests/benchdnn/benchdnn
	cd /home/vineel/tf_onednn/oneDNN/build/tests/benchdnn && /home/vineel/tf_onednn/oneDNN/build/tests/benchdnn/benchdnn --mode=C -v1 --engine=cpu --ip --batch=test_ip_ci

test_benchdnn_ip_ci_cpu: tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu
test_benchdnn_ip_ci_cpu: tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/build.make
.PHONY : test_benchdnn_ip_ci_cpu

# Rule to build all files generated by this target.
tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/build: test_benchdnn_ip_ci_cpu
.PHONY : tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/build

tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/clean:
	cd /home/vineel/tf_onednn/oneDNN/build/tests/benchdnn && $(CMAKE_COMMAND) -P CMakeFiles/test_benchdnn_ip_ci_cpu.dir/cmake_clean.cmake
.PHONY : tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/clean

tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/depend:
	cd /home/vineel/tf_onednn/oneDNN/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vineel/tf_onednn/oneDNN /home/vineel/tf_onednn/oneDNN/tests/benchdnn /home/vineel/tf_onednn/oneDNN/build /home/vineel/tf_onednn/oneDNN/build/tests/benchdnn /home/vineel/tf_onednn/oneDNN/build/tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/benchdnn/CMakeFiles/test_benchdnn_ip_ci_cpu.dir/depend

