# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /home/linuxbrew/.linuxbrew/Cellar/cmake/3.23.0/bin/cmake

# The command to remove a file.
RM = /home/linuxbrew/.linuxbrew/Cellar/cmake/3.23.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace/turbolang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/turbolang

# Include any dependencies generated for this target.
include CMakeFiles/turbolang.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/turbolang.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/turbolang.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/turbolang.dir/flags.make

CMakeFiles/turbolang.dir/src/Builder.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Builder.cpp.o: src/Builder.cpp
CMakeFiles/turbolang.dir/src/Builder.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/turbolang.dir/src/Builder.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Builder.cpp.o -MF CMakeFiles/turbolang.dir/src/Builder.cpp.o.d -o CMakeFiles/turbolang.dir/src/Builder.cpp.o -c /workspace/turbolang/src/Builder.cpp

CMakeFiles/turbolang.dir/src/Builder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Builder.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Builder.cpp > CMakeFiles/turbolang.dir/src/Builder.cpp.i

CMakeFiles/turbolang.dir/src/Builder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Builder.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Builder.cpp -o CMakeFiles/turbolang.dir/src/Builder.cpp.s

CMakeFiles/turbolang.dir/src/Class.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Class.cpp.o: src/Class.cpp
CMakeFiles/turbolang.dir/src/Class.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/turbolang.dir/src/Class.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Class.cpp.o -MF CMakeFiles/turbolang.dir/src/Class.cpp.o.d -o CMakeFiles/turbolang.dir/src/Class.cpp.o -c /workspace/turbolang/src/Class.cpp

CMakeFiles/turbolang.dir/src/Class.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Class.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Class.cpp > CMakeFiles/turbolang.dir/src/Class.cpp.i

CMakeFiles/turbolang.dir/src/Class.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Class.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Class.cpp -o CMakeFiles/turbolang.dir/src/Class.cpp.s

CMakeFiles/turbolang.dir/src/Compiler.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Compiler.cpp.o: src/Compiler.cpp
CMakeFiles/turbolang.dir/src/Compiler.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/turbolang.dir/src/Compiler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Compiler.cpp.o -MF CMakeFiles/turbolang.dir/src/Compiler.cpp.o.d -o CMakeFiles/turbolang.dir/src/Compiler.cpp.o -c /workspace/turbolang/src/Compiler.cpp

CMakeFiles/turbolang.dir/src/Compiler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Compiler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Compiler.cpp > CMakeFiles/turbolang.dir/src/Compiler.cpp.i

CMakeFiles/turbolang.dir/src/Compiler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Compiler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Compiler.cpp -o CMakeFiles/turbolang.dir/src/Compiler.cpp.s

CMakeFiles/turbolang.dir/src/Function.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Function.cpp.o: src/Function.cpp
CMakeFiles/turbolang.dir/src/Function.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/turbolang.dir/src/Function.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Function.cpp.o -MF CMakeFiles/turbolang.dir/src/Function.cpp.o.d -o CMakeFiles/turbolang.dir/src/Function.cpp.o -c /workspace/turbolang/src/Function.cpp

CMakeFiles/turbolang.dir/src/Function.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Function.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Function.cpp > CMakeFiles/turbolang.dir/src/Function.cpp.i

CMakeFiles/turbolang.dir/src/Function.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Function.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Function.cpp -o CMakeFiles/turbolang.dir/src/Function.cpp.s

CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o: src/FunctionCallProcessor.cpp
CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o -MF CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o.d -o CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o -c /workspace/turbolang/src/FunctionCallProcessor.cpp

CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/FunctionCallProcessor.cpp > CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.i

CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/FunctionCallProcessor.cpp -o CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.s

CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o: src/FunctionInternal.cpp
CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o -MF CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o.d -o CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o -c /workspace/turbolang/src/FunctionInternal.cpp

CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/FunctionInternal.cpp > CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.i

CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/FunctionInternal.cpp -o CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.s

CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o: src/LLVMManager.cpp
CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o -MF CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o.d -o CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o -c /workspace/turbolang/src/LLVMManager.cpp

CMakeFiles/turbolang.dir/src/LLVMManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/LLVMManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/LLVMManager.cpp > CMakeFiles/turbolang.dir/src/LLVMManager.cpp.i

CMakeFiles/turbolang.dir/src/LLVMManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/LLVMManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/LLVMManager.cpp -o CMakeFiles/turbolang.dir/src/LLVMManager.cpp.s

CMakeFiles/turbolang.dir/src/Logger.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Logger.cpp.o: src/Logger.cpp
CMakeFiles/turbolang.dir/src/Logger.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/turbolang.dir/src/Logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Logger.cpp.o -MF CMakeFiles/turbolang.dir/src/Logger.cpp.o.d -o CMakeFiles/turbolang.dir/src/Logger.cpp.o -c /workspace/turbolang/src/Logger.cpp

CMakeFiles/turbolang.dir/src/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Logger.cpp > CMakeFiles/turbolang.dir/src/Logger.cpp.i

CMakeFiles/turbolang.dir/src/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Logger.cpp -o CMakeFiles/turbolang.dir/src/Logger.cpp.s

CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o: src/MathEvaluator.cpp
CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o -MF CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o.d -o CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o -c /workspace/turbolang/src/MathEvaluator.cpp

CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/MathEvaluator.cpp > CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.i

CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/MathEvaluator.cpp -o CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.s

CMakeFiles/turbolang.dir/src/Parser.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Parser.cpp.o: src/Parser.cpp
CMakeFiles/turbolang.dir/src/Parser.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/turbolang.dir/src/Parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Parser.cpp.o -MF CMakeFiles/turbolang.dir/src/Parser.cpp.o.d -o CMakeFiles/turbolang.dir/src/Parser.cpp.o -c /workspace/turbolang/src/Parser.cpp

CMakeFiles/turbolang.dir/src/Parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Parser.cpp > CMakeFiles/turbolang.dir/src/Parser.cpp.i

CMakeFiles/turbolang.dir/src/Parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Parser.cpp -o CMakeFiles/turbolang.dir/src/Parser.cpp.s

CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o: src/SourceCodeReader.cpp
CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o -MF CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o.d -o CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o -c /workspace/turbolang/src/SourceCodeReader.cpp

CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/SourceCodeReader.cpp > CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.i

CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/SourceCodeReader.cpp -o CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.s

CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o: src/TimeUtil.cpp
CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o -MF CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o.d -o CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o -c /workspace/turbolang/src/TimeUtil.cpp

CMakeFiles/turbolang.dir/src/TimeUtil.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/TimeUtil.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/TimeUtil.cpp > CMakeFiles/turbolang.dir/src/TimeUtil.cpp.i

CMakeFiles/turbolang.dir/src/TimeUtil.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/TimeUtil.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/TimeUtil.cpp -o CMakeFiles/turbolang.dir/src/TimeUtil.cpp.s

CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o: src/Tokenizer.cpp
CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o -MF CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o.d -o CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o -c /workspace/turbolang/src/Tokenizer.cpp

CMakeFiles/turbolang.dir/src/Tokenizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Tokenizer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Tokenizer.cpp > CMakeFiles/turbolang.dir/src/Tokenizer.cpp.i

CMakeFiles/turbolang.dir/src/Tokenizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Tokenizer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Tokenizer.cpp -o CMakeFiles/turbolang.dir/src/Tokenizer.cpp.s

CMakeFiles/turbolang.dir/src/Type.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/Type.cpp.o: src/Type.cpp
CMakeFiles/turbolang.dir/src/Type.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/turbolang.dir/src/Type.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/Type.cpp.o -MF CMakeFiles/turbolang.dir/src/Type.cpp.o.d -o CMakeFiles/turbolang.dir/src/Type.cpp.o -c /workspace/turbolang/src/Type.cpp

CMakeFiles/turbolang.dir/src/Type.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/Type.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/Type.cpp > CMakeFiles/turbolang.dir/src/Type.cpp.i

CMakeFiles/turbolang.dir/src/Type.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/Type.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/Type.cpp -o CMakeFiles/turbolang.dir/src/Type.cpp.s

CMakeFiles/turbolang.dir/src/main.cpp.o: CMakeFiles/turbolang.dir/flags.make
CMakeFiles/turbolang.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/turbolang.dir/src/main.cpp.o: CMakeFiles/turbolang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/turbolang.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turbolang.dir/src/main.cpp.o -MF CMakeFiles/turbolang.dir/src/main.cpp.o.d -o CMakeFiles/turbolang.dir/src/main.cpp.o -c /workspace/turbolang/src/main.cpp

CMakeFiles/turbolang.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/turbolang.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/turbolang/src/main.cpp > CMakeFiles/turbolang.dir/src/main.cpp.i

CMakeFiles/turbolang.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/turbolang.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/turbolang/src/main.cpp -o CMakeFiles/turbolang.dir/src/main.cpp.s

# Object files for target turbolang
turbolang_OBJECTS = \
"CMakeFiles/turbolang.dir/src/Builder.cpp.o" \
"CMakeFiles/turbolang.dir/src/Class.cpp.o" \
"CMakeFiles/turbolang.dir/src/Compiler.cpp.o" \
"CMakeFiles/turbolang.dir/src/Function.cpp.o" \
"CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o" \
"CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o" \
"CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o" \
"CMakeFiles/turbolang.dir/src/Logger.cpp.o" \
"CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o" \
"CMakeFiles/turbolang.dir/src/Parser.cpp.o" \
"CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o" \
"CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o" \
"CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o" \
"CMakeFiles/turbolang.dir/src/Type.cpp.o" \
"CMakeFiles/turbolang.dir/src/main.cpp.o"

# External object files for target turbolang
turbolang_EXTERNAL_OBJECTS =

turbolang: CMakeFiles/turbolang.dir/src/Builder.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/Class.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/Compiler.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/Function.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/Logger.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/Parser.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/Type.cpp.o
turbolang: CMakeFiles/turbolang.dir/src/main.cpp.o
turbolang: CMakeFiles/turbolang.dir/build.make
turbolang: /usr/lib/llvm-12/lib/libLLVMSupport.a
turbolang: /usr/lib/llvm-12/lib/libLLVMCore.a
turbolang: /usr/lib/llvm-12/lib/libLLVMIRReader.a
turbolang: /usr/lib/llvm-12/lib/libLLVMAsmParser.a
turbolang: /usr/lib/llvm-12/lib/libLLVMBitReader.a
turbolang: /usr/lib/llvm-12/lib/libLLVMCore.a
turbolang: /usr/lib/llvm-12/lib/libLLVMBinaryFormat.a
turbolang: /usr/lib/llvm-12/lib/libLLVMRemarks.a
turbolang: /usr/lib/llvm-12/lib/libLLVMBitstreamReader.a
turbolang: /usr/lib/llvm-12/lib/libLLVMSupport.a
turbolang: /usr/lib/x86_64-linux-gnu/libz.so
turbolang: /usr/lib/x86_64-linux-gnu/libtinfo.so
turbolang: /usr/lib/llvm-12/lib/libLLVMDemangle.a
turbolang: CMakeFiles/turbolang.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/workspace/turbolang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable turbolang"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/turbolang.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/turbolang.dir/build: turbolang
.PHONY : CMakeFiles/turbolang.dir/build

CMakeFiles/turbolang.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/turbolang.dir/cmake_clean.cmake
.PHONY : CMakeFiles/turbolang.dir/clean

CMakeFiles/turbolang.dir/depend:
	cd /workspace/turbolang && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/turbolang /workspace/turbolang /workspace/turbolang /workspace/turbolang /workspace/turbolang/CMakeFiles/turbolang.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/turbolang.dir/depend

