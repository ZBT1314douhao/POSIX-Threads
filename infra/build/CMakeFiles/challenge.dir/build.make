# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lumia/workspace/documents/POSIX-Threads/infra

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lumia/workspace/documents/POSIX-Threads/infra/build

# Include any dependencies generated for this target.
include CMakeFiles/challenge.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/challenge.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/challenge.dir/flags.make

CMakeFiles/challenge.dir/src/tests/test_main.cpp.o: CMakeFiles/challenge.dir/flags.make
CMakeFiles/challenge.dir/src/tests/test_main.cpp.o: ../src/tests/test_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/challenge.dir/src/tests/test_main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/challenge.dir/src/tests/test_main.cpp.o -c /home/lumia/workspace/documents/POSIX-Threads/infra/src/tests/test_main.cpp

CMakeFiles/challenge.dir/src/tests/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/challenge.dir/src/tests/test_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lumia/workspace/documents/POSIX-Threads/infra/src/tests/test_main.cpp > CMakeFiles/challenge.dir/src/tests/test_main.cpp.i

CMakeFiles/challenge.dir/src/tests/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/challenge.dir/src/tests/test_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lumia/workspace/documents/POSIX-Threads/infra/src/tests/test_main.cpp -o CMakeFiles/challenge.dir/src/tests/test_main.cpp.s

CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.requires:

.PHONY : CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.requires

CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.provides: CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.requires
	$(MAKE) -f CMakeFiles/challenge.dir/build.make CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.provides.build
.PHONY : CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.provides

CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.provides.build: CMakeFiles/challenge.dir/src/tests/test_main.cpp.o


CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o: CMakeFiles/challenge.dir/flags.make
CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o: ../src/tests/test_thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o -c /home/lumia/workspace/documents/POSIX-Threads/infra/src/tests/test_thread.cpp

CMakeFiles/challenge.dir/src/tests/test_thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/challenge.dir/src/tests/test_thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lumia/workspace/documents/POSIX-Threads/infra/src/tests/test_thread.cpp > CMakeFiles/challenge.dir/src/tests/test_thread.cpp.i

CMakeFiles/challenge.dir/src/tests/test_thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/challenge.dir/src/tests/test_thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lumia/workspace/documents/POSIX-Threads/infra/src/tests/test_thread.cpp -o CMakeFiles/challenge.dir/src/tests/test_thread.cpp.s

CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.requires:

.PHONY : CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.requires

CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.provides: CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.requires
	$(MAKE) -f CMakeFiles/challenge.dir/build.make CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.provides.build
.PHONY : CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.provides

CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.provides.build: CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o


CMakeFiles/challenge.dir/src/os/mutex.cpp.o: CMakeFiles/challenge.dir/flags.make
CMakeFiles/challenge.dir/src/os/mutex.cpp.o: ../src/os/mutex.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/challenge.dir/src/os/mutex.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/challenge.dir/src/os/mutex.cpp.o -c /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/mutex.cpp

CMakeFiles/challenge.dir/src/os/mutex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/challenge.dir/src/os/mutex.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/mutex.cpp > CMakeFiles/challenge.dir/src/os/mutex.cpp.i

CMakeFiles/challenge.dir/src/os/mutex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/challenge.dir/src/os/mutex.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/mutex.cpp -o CMakeFiles/challenge.dir/src/os/mutex.cpp.s

CMakeFiles/challenge.dir/src/os/mutex.cpp.o.requires:

.PHONY : CMakeFiles/challenge.dir/src/os/mutex.cpp.o.requires

CMakeFiles/challenge.dir/src/os/mutex.cpp.o.provides: CMakeFiles/challenge.dir/src/os/mutex.cpp.o.requires
	$(MAKE) -f CMakeFiles/challenge.dir/build.make CMakeFiles/challenge.dir/src/os/mutex.cpp.o.provides.build
.PHONY : CMakeFiles/challenge.dir/src/os/mutex.cpp.o.provides

CMakeFiles/challenge.dir/src/os/mutex.cpp.o.provides.build: CMakeFiles/challenge.dir/src/os/mutex.cpp.o


CMakeFiles/challenge.dir/src/os/singleton.cpp.o: CMakeFiles/challenge.dir/flags.make
CMakeFiles/challenge.dir/src/os/singleton.cpp.o: ../src/os/singleton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/challenge.dir/src/os/singleton.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/challenge.dir/src/os/singleton.cpp.o -c /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/singleton.cpp

CMakeFiles/challenge.dir/src/os/singleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/challenge.dir/src/os/singleton.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/singleton.cpp > CMakeFiles/challenge.dir/src/os/singleton.cpp.i

CMakeFiles/challenge.dir/src/os/singleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/challenge.dir/src/os/singleton.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/singleton.cpp -o CMakeFiles/challenge.dir/src/os/singleton.cpp.s

CMakeFiles/challenge.dir/src/os/singleton.cpp.o.requires:

.PHONY : CMakeFiles/challenge.dir/src/os/singleton.cpp.o.requires

CMakeFiles/challenge.dir/src/os/singleton.cpp.o.provides: CMakeFiles/challenge.dir/src/os/singleton.cpp.o.requires
	$(MAKE) -f CMakeFiles/challenge.dir/build.make CMakeFiles/challenge.dir/src/os/singleton.cpp.o.provides.build
.PHONY : CMakeFiles/challenge.dir/src/os/singleton.cpp.o.provides

CMakeFiles/challenge.dir/src/os/singleton.cpp.o.provides.build: CMakeFiles/challenge.dir/src/os/singleton.cpp.o


CMakeFiles/challenge.dir/src/os/thread.cpp.o: CMakeFiles/challenge.dir/flags.make
CMakeFiles/challenge.dir/src/os/thread.cpp.o: ../src/os/thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/challenge.dir/src/os/thread.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/challenge.dir/src/os/thread.cpp.o -c /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/thread.cpp

CMakeFiles/challenge.dir/src/os/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/challenge.dir/src/os/thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/thread.cpp > CMakeFiles/challenge.dir/src/os/thread.cpp.i

CMakeFiles/challenge.dir/src/os/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/challenge.dir/src/os/thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lumia/workspace/documents/POSIX-Threads/infra/src/os/thread.cpp -o CMakeFiles/challenge.dir/src/os/thread.cpp.s

CMakeFiles/challenge.dir/src/os/thread.cpp.o.requires:

.PHONY : CMakeFiles/challenge.dir/src/os/thread.cpp.o.requires

CMakeFiles/challenge.dir/src/os/thread.cpp.o.provides: CMakeFiles/challenge.dir/src/os/thread.cpp.o.requires
	$(MAKE) -f CMakeFiles/challenge.dir/build.make CMakeFiles/challenge.dir/src/os/thread.cpp.o.provides.build
.PHONY : CMakeFiles/challenge.dir/src/os/thread.cpp.o.provides

CMakeFiles/challenge.dir/src/os/thread.cpp.o.provides.build: CMakeFiles/challenge.dir/src/os/thread.cpp.o


CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o: CMakeFiles/challenge.dir/flags.make
CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o: ../src/utils/colorfulprint.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o -c /home/lumia/workspace/documents/POSIX-Threads/infra/src/utils/colorfulprint.cpp

CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lumia/workspace/documents/POSIX-Threads/infra/src/utils/colorfulprint.cpp > CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.i

CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lumia/workspace/documents/POSIX-Threads/infra/src/utils/colorfulprint.cpp -o CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.s

CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.requires:

.PHONY : CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.requires

CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.provides: CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.requires
	$(MAKE) -f CMakeFiles/challenge.dir/build.make CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.provides.build
.PHONY : CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.provides

CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.provides.build: CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o


# Object files for target challenge
challenge_OBJECTS = \
"CMakeFiles/challenge.dir/src/tests/test_main.cpp.o" \
"CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o" \
"CMakeFiles/challenge.dir/src/os/mutex.cpp.o" \
"CMakeFiles/challenge.dir/src/os/singleton.cpp.o" \
"CMakeFiles/challenge.dir/src/os/thread.cpp.o" \
"CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o"

# External object files for target challenge
challenge_EXTERNAL_OBJECTS =

challenge: CMakeFiles/challenge.dir/src/tests/test_main.cpp.o
challenge: CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o
challenge: CMakeFiles/challenge.dir/src/os/mutex.cpp.o
challenge: CMakeFiles/challenge.dir/src/os/singleton.cpp.o
challenge: CMakeFiles/challenge.dir/src/os/thread.cpp.o
challenge: CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o
challenge: CMakeFiles/challenge.dir/build.make
challenge: CMakeFiles/challenge.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable challenge"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/challenge.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/challenge.dir/build: challenge

.PHONY : CMakeFiles/challenge.dir/build

CMakeFiles/challenge.dir/requires: CMakeFiles/challenge.dir/src/tests/test_main.cpp.o.requires
CMakeFiles/challenge.dir/requires: CMakeFiles/challenge.dir/src/tests/test_thread.cpp.o.requires
CMakeFiles/challenge.dir/requires: CMakeFiles/challenge.dir/src/os/mutex.cpp.o.requires
CMakeFiles/challenge.dir/requires: CMakeFiles/challenge.dir/src/os/singleton.cpp.o.requires
CMakeFiles/challenge.dir/requires: CMakeFiles/challenge.dir/src/os/thread.cpp.o.requires
CMakeFiles/challenge.dir/requires: CMakeFiles/challenge.dir/src/utils/colorfulprint.cpp.o.requires

.PHONY : CMakeFiles/challenge.dir/requires

CMakeFiles/challenge.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/challenge.dir/cmake_clean.cmake
.PHONY : CMakeFiles/challenge.dir/clean

CMakeFiles/challenge.dir/depend:
	cd /home/lumia/workspace/documents/POSIX-Threads/infra/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lumia/workspace/documents/POSIX-Threads/infra /home/lumia/workspace/documents/POSIX-Threads/infra /home/lumia/workspace/documents/POSIX-Threads/infra/build /home/lumia/workspace/documents/POSIX-Threads/infra/build /home/lumia/workspace/documents/POSIX-Threads/infra/build/CMakeFiles/challenge.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/challenge.dir/depend

