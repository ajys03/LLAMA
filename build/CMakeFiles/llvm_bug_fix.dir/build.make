# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/annyw/Desktop/Computer/LLAMA/llvm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/annyw/Desktop/Computer/LLAMA/build

# Include any dependencies generated for this target.
include CMakeFiles/llvm_bug_fix.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/llvm_bug_fix.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/llvm_bug_fix.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/llvm_bug_fix.dir/flags.make

CMakeFiles/llvm_bug_fix.dir/codegen:
.PHONY : CMakeFiles/llvm_bug_fix.dir/codegen

CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o: CMakeFiles/llvm_bug_fix.dir/flags.make
CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o: /Users/annyw/Desktop/Computer/LLAMA/llvm/llvm_bug_fix.c
CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o: CMakeFiles/llvm_bug_fix.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/annyw/Desktop/Computer/LLAMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o -MF CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o.d -o CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o -c /Users/annyw/Desktop/Computer/LLAMA/llvm/llvm_bug_fix.c

CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/annyw/Desktop/Computer/LLAMA/llvm/llvm_bug_fix.c > CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.i

CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/annyw/Desktop/Computer/LLAMA/llvm/llvm_bug_fix.c -o CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.s

# Object files for target llvm_bug_fix
llvm_bug_fix_OBJECTS = \
"CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o"

# External object files for target llvm_bug_fix
llvm_bug_fix_EXTERNAL_OBJECTS =

libllvm_bug_fix.dylib: CMakeFiles/llvm_bug_fix.dir/llvm_bug_fix.o
libllvm_bug_fix.dylib: CMakeFiles/llvm_bug_fix.dir/build.make
libllvm_bug_fix.dylib: CMakeFiles/llvm_bug_fix.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/annyw/Desktop/Computer/LLAMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libllvm_bug_fix.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/llvm_bug_fix.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/llvm_bug_fix.dir/build: libllvm_bug_fix.dylib
.PHONY : CMakeFiles/llvm_bug_fix.dir/build

CMakeFiles/llvm_bug_fix.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/llvm_bug_fix.dir/cmake_clean.cmake
.PHONY : CMakeFiles/llvm_bug_fix.dir/clean

CMakeFiles/llvm_bug_fix.dir/depend:
	cd /Users/annyw/Desktop/Computer/LLAMA/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/annyw/Desktop/Computer/LLAMA/llvm /Users/annyw/Desktop/Computer/LLAMA/llvm /Users/annyw/Desktop/Computer/LLAMA/build /Users/annyw/Desktop/Computer/LLAMA/build /Users/annyw/Desktop/Computer/LLAMA/build/CMakeFiles/llvm_bug_fix.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/llvm_bug_fix.dir/depend
