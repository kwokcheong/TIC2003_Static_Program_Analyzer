# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug

# Include any dependencies generated for this target.
include src/autotester/CMakeFiles/autotester.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/autotester/CMakeFiles/autotester.dir/compiler_depend.make

# Include the progress variables for this target.
include src/autotester/CMakeFiles/autotester.dir/progress.make

# Include the compile flags for this target's objects.
include src/autotester/CMakeFiles/autotester.dir/flags.make

src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o: src/autotester/CMakeFiles/autotester.dir/flags.make
src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o: /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/src/autotester/src/TestWrapper.cpp
src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o: src/autotester/CMakeFiles/autotester.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o"
	cd /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/src/autotester && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o -MF CMakeFiles/autotester.dir/src/TestWrapper.cpp.o.d -o CMakeFiles/autotester.dir/src/TestWrapper.cpp.o -c /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/src/autotester/src/TestWrapper.cpp

src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/autotester.dir/src/TestWrapper.cpp.i"
	cd /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/src/autotester && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/src/autotester/src/TestWrapper.cpp > CMakeFiles/autotester.dir/src/TestWrapper.cpp.i

src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/autotester.dir/src/TestWrapper.cpp.s"
	cd /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/src/autotester && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/src/autotester/src/TestWrapper.cpp -o CMakeFiles/autotester.dir/src/TestWrapper.cpp.s

# Object files for target autotester
autotester_OBJECTS = \
"CMakeFiles/autotester.dir/src/TestWrapper.cpp.o"

# External object files for target autotester
autotester_EXTERNAL_OBJECTS =

src/autotester/autotester: src/autotester/CMakeFiles/autotester.dir/src/TestWrapper.cpp.o
src/autotester/autotester: src/autotester/CMakeFiles/autotester.dir/build.make
src/autotester/autotester: src/spa/libspa.a
src/autotester/autotester: /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/lib/autotester/apple/release/libautotester.a
src/autotester/autotester: src/autotester/CMakeFiles/autotester.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable autotester"
	cd /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/src/autotester && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/autotester.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/autotester/CMakeFiles/autotester.dir/build: src/autotester/autotester
.PHONY : src/autotester/CMakeFiles/autotester.dir/build

src/autotester/CMakeFiles/autotester.dir/clean:
	cd /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/src/autotester && $(CMAKE_COMMAND) -P CMakeFiles/autotester.dir/cmake_clean.cmake
.PHONY : src/autotester/CMakeFiles/autotester.dir/clean

src/autotester/CMakeFiles/autotester.dir/depend:
	cd /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/src/autotester /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/src/autotester /Users/kwokcheong/Desktop/nus_projects/TIC2003_Static_Program_Analyzer/A0123456Y/Code-A0123456Y/cmake-build-debug/src/autotester/CMakeFiles/autotester.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/autotester/CMakeFiles/autotester.dir/depend
