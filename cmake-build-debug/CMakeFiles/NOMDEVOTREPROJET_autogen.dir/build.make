# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /mnt/c/Users/Surface/Documents/GitHub/project_cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Surface/Documents/GitHub/project_cpp/cmake-build-debug

# Utility rule file for NOMDEVOTREPROJET_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/NOMDEVOTREPROJET_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/NOMDEVOTREPROJET_autogen.dir/progress.make

CMakeFiles/NOMDEVOTREPROJET_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Surface/Documents/GitHub/project_cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target NOMDEVOTREPROJET"
	/usr/bin/cmake -E cmake_autogen /mnt/c/Users/Surface/Documents/GitHub/project_cpp/cmake-build-debug/CMakeFiles/NOMDEVOTREPROJET_autogen.dir/AutogenInfo.json Debug

NOMDEVOTREPROJET_autogen: CMakeFiles/NOMDEVOTREPROJET_autogen
NOMDEVOTREPROJET_autogen: CMakeFiles/NOMDEVOTREPROJET_autogen.dir/build.make
.PHONY : NOMDEVOTREPROJET_autogen

# Rule to build all files generated by this target.
CMakeFiles/NOMDEVOTREPROJET_autogen.dir/build: NOMDEVOTREPROJET_autogen
.PHONY : CMakeFiles/NOMDEVOTREPROJET_autogen.dir/build

CMakeFiles/NOMDEVOTREPROJET_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NOMDEVOTREPROJET_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NOMDEVOTREPROJET_autogen.dir/clean

CMakeFiles/NOMDEVOTREPROJET_autogen.dir/depend:
	cd /mnt/c/Users/Surface/Documents/GitHub/project_cpp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Surface/Documents/GitHub/project_cpp /mnt/c/Users/Surface/Documents/GitHub/project_cpp /mnt/c/Users/Surface/Documents/GitHub/project_cpp/cmake-build-debug /mnt/c/Users/Surface/Documents/GitHub/project_cpp/cmake-build-debug /mnt/c/Users/Surface/Documents/GitHub/project_cpp/cmake-build-debug/CMakeFiles/NOMDEVOTREPROJET_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NOMDEVOTREPROJET_autogen.dir/depend

