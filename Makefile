# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/home/linuxbrew/.linuxbrew/Cellar/cmake/3.23.0/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/linuxbrew/.linuxbrew/Cellar/cmake/3.23.0/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /workspace/turbolang/CMakeFiles /workspace/turbolang//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /workspace/turbolang/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named turbolang

# Build rule for target.
turbolang: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 turbolang
.PHONY : turbolang

# fast build rule for target.
turbolang/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/build
.PHONY : turbolang/fast

#=============================================================================
# Target rules for targets named intrinsics_gen

# Build rule for target.
intrinsics_gen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 intrinsics_gen
.PHONY : intrinsics_gen

# fast build rule for target.
intrinsics_gen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/intrinsics_gen.dir/build.make CMakeFiles/intrinsics_gen.dir/build
.PHONY : intrinsics_gen/fast

#=============================================================================
# Target rules for targets named omp_gen

# Build rule for target.
omp_gen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 omp_gen
.PHONY : omp_gen

# fast build rule for target.
omp_gen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/omp_gen.dir/build.make CMakeFiles/omp_gen.dir/build
.PHONY : omp_gen/fast

#=============================================================================
# Target rules for targets named acc_gen

# Build rule for target.
acc_gen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 acc_gen
.PHONY : acc_gen

# fast build rule for target.
acc_gen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/acc_gen.dir/build.make CMakeFiles/acc_gen.dir/build
.PHONY : acc_gen/fast

src/Builder.o: src/Builder.cpp.o
.PHONY : src/Builder.o

# target to build an object file
src/Builder.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Builder.cpp.o
.PHONY : src/Builder.cpp.o

src/Builder.i: src/Builder.cpp.i
.PHONY : src/Builder.i

# target to preprocess a source file
src/Builder.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Builder.cpp.i
.PHONY : src/Builder.cpp.i

src/Builder.s: src/Builder.cpp.s
.PHONY : src/Builder.s

# target to generate assembly for a file
src/Builder.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Builder.cpp.s
.PHONY : src/Builder.cpp.s

src/Class.o: src/Class.cpp.o
.PHONY : src/Class.o

# target to build an object file
src/Class.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Class.cpp.o
.PHONY : src/Class.cpp.o

src/Class.i: src/Class.cpp.i
.PHONY : src/Class.i

# target to preprocess a source file
src/Class.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Class.cpp.i
.PHONY : src/Class.cpp.i

src/Class.s: src/Class.cpp.s
.PHONY : src/Class.s

# target to generate assembly for a file
src/Class.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Class.cpp.s
.PHONY : src/Class.cpp.s

src/Compiler.o: src/Compiler.cpp.o
.PHONY : src/Compiler.o

# target to build an object file
src/Compiler.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Compiler.cpp.o
.PHONY : src/Compiler.cpp.o

src/Compiler.i: src/Compiler.cpp.i
.PHONY : src/Compiler.i

# target to preprocess a source file
src/Compiler.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Compiler.cpp.i
.PHONY : src/Compiler.cpp.i

src/Compiler.s: src/Compiler.cpp.s
.PHONY : src/Compiler.s

# target to generate assembly for a file
src/Compiler.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Compiler.cpp.s
.PHONY : src/Compiler.cpp.s

src/Function.o: src/Function.cpp.o
.PHONY : src/Function.o

# target to build an object file
src/Function.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Function.cpp.o
.PHONY : src/Function.cpp.o

src/Function.i: src/Function.cpp.i
.PHONY : src/Function.i

# target to preprocess a source file
src/Function.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Function.cpp.i
.PHONY : src/Function.cpp.i

src/Function.s: src/Function.cpp.s
.PHONY : src/Function.s

# target to generate assembly for a file
src/Function.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Function.cpp.s
.PHONY : src/Function.cpp.s

src/FunctionCallProcessor.o: src/FunctionCallProcessor.cpp.o
.PHONY : src/FunctionCallProcessor.o

# target to build an object file
src/FunctionCallProcessor.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.o
.PHONY : src/FunctionCallProcessor.cpp.o

src/FunctionCallProcessor.i: src/FunctionCallProcessor.cpp.i
.PHONY : src/FunctionCallProcessor.i

# target to preprocess a source file
src/FunctionCallProcessor.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.i
.PHONY : src/FunctionCallProcessor.cpp.i

src/FunctionCallProcessor.s: src/FunctionCallProcessor.cpp.s
.PHONY : src/FunctionCallProcessor.s

# target to generate assembly for a file
src/FunctionCallProcessor.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/FunctionCallProcessor.cpp.s
.PHONY : src/FunctionCallProcessor.cpp.s

src/FunctionInternal.o: src/FunctionInternal.cpp.o
.PHONY : src/FunctionInternal.o

# target to build an object file
src/FunctionInternal.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.o
.PHONY : src/FunctionInternal.cpp.o

src/FunctionInternal.i: src/FunctionInternal.cpp.i
.PHONY : src/FunctionInternal.i

# target to preprocess a source file
src/FunctionInternal.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.i
.PHONY : src/FunctionInternal.cpp.i

src/FunctionInternal.s: src/FunctionInternal.cpp.s
.PHONY : src/FunctionInternal.s

# target to generate assembly for a file
src/FunctionInternal.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/FunctionInternal.cpp.s
.PHONY : src/FunctionInternal.cpp.s

src/LLVMManager.o: src/LLVMManager.cpp.o
.PHONY : src/LLVMManager.o

# target to build an object file
src/LLVMManager.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/LLVMManager.cpp.o
.PHONY : src/LLVMManager.cpp.o

src/LLVMManager.i: src/LLVMManager.cpp.i
.PHONY : src/LLVMManager.i

# target to preprocess a source file
src/LLVMManager.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/LLVMManager.cpp.i
.PHONY : src/LLVMManager.cpp.i

src/LLVMManager.s: src/LLVMManager.cpp.s
.PHONY : src/LLVMManager.s

# target to generate assembly for a file
src/LLVMManager.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/LLVMManager.cpp.s
.PHONY : src/LLVMManager.cpp.s

src/Logger.o: src/Logger.cpp.o
.PHONY : src/Logger.o

# target to build an object file
src/Logger.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Logger.cpp.o
.PHONY : src/Logger.cpp.o

src/Logger.i: src/Logger.cpp.i
.PHONY : src/Logger.i

# target to preprocess a source file
src/Logger.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Logger.cpp.i
.PHONY : src/Logger.cpp.i

src/Logger.s: src/Logger.cpp.s
.PHONY : src/Logger.s

# target to generate assembly for a file
src/Logger.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Logger.cpp.s
.PHONY : src/Logger.cpp.s

src/MathEvaluator.o: src/MathEvaluator.cpp.o
.PHONY : src/MathEvaluator.o

# target to build an object file
src/MathEvaluator.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.o
.PHONY : src/MathEvaluator.cpp.o

src/MathEvaluator.i: src/MathEvaluator.cpp.i
.PHONY : src/MathEvaluator.i

# target to preprocess a source file
src/MathEvaluator.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.i
.PHONY : src/MathEvaluator.cpp.i

src/MathEvaluator.s: src/MathEvaluator.cpp.s
.PHONY : src/MathEvaluator.s

# target to generate assembly for a file
src/MathEvaluator.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/MathEvaluator.cpp.s
.PHONY : src/MathEvaluator.cpp.s

src/Parser.o: src/Parser.cpp.o
.PHONY : src/Parser.o

# target to build an object file
src/Parser.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Parser.cpp.o
.PHONY : src/Parser.cpp.o

src/Parser.i: src/Parser.cpp.i
.PHONY : src/Parser.i

# target to preprocess a source file
src/Parser.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Parser.cpp.i
.PHONY : src/Parser.cpp.i

src/Parser.s: src/Parser.cpp.s
.PHONY : src/Parser.s

# target to generate assembly for a file
src/Parser.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Parser.cpp.s
.PHONY : src/Parser.cpp.s

src/SourceCodeReader.o: src/SourceCodeReader.cpp.o
.PHONY : src/SourceCodeReader.o

# target to build an object file
src/SourceCodeReader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.o
.PHONY : src/SourceCodeReader.cpp.o

src/SourceCodeReader.i: src/SourceCodeReader.cpp.i
.PHONY : src/SourceCodeReader.i

# target to preprocess a source file
src/SourceCodeReader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.i
.PHONY : src/SourceCodeReader.cpp.i

src/SourceCodeReader.s: src/SourceCodeReader.cpp.s
.PHONY : src/SourceCodeReader.s

# target to generate assembly for a file
src/SourceCodeReader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/SourceCodeReader.cpp.s
.PHONY : src/SourceCodeReader.cpp.s

src/TimeUtil.o: src/TimeUtil.cpp.o
.PHONY : src/TimeUtil.o

# target to build an object file
src/TimeUtil.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/TimeUtil.cpp.o
.PHONY : src/TimeUtil.cpp.o

src/TimeUtil.i: src/TimeUtil.cpp.i
.PHONY : src/TimeUtil.i

# target to preprocess a source file
src/TimeUtil.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/TimeUtil.cpp.i
.PHONY : src/TimeUtil.cpp.i

src/TimeUtil.s: src/TimeUtil.cpp.s
.PHONY : src/TimeUtil.s

# target to generate assembly for a file
src/TimeUtil.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/TimeUtil.cpp.s
.PHONY : src/TimeUtil.cpp.s

src/Tokenizer.o: src/Tokenizer.cpp.o
.PHONY : src/Tokenizer.o

# target to build an object file
src/Tokenizer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Tokenizer.cpp.o
.PHONY : src/Tokenizer.cpp.o

src/Tokenizer.i: src/Tokenizer.cpp.i
.PHONY : src/Tokenizer.i

# target to preprocess a source file
src/Tokenizer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Tokenizer.cpp.i
.PHONY : src/Tokenizer.cpp.i

src/Tokenizer.s: src/Tokenizer.cpp.s
.PHONY : src/Tokenizer.s

# target to generate assembly for a file
src/Tokenizer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Tokenizer.cpp.s
.PHONY : src/Tokenizer.cpp.s

src/Type.o: src/Type.cpp.o
.PHONY : src/Type.o

# target to build an object file
src/Type.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Type.cpp.o
.PHONY : src/Type.cpp.o

src/Type.i: src/Type.cpp.i
.PHONY : src/Type.i

# target to preprocess a source file
src/Type.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Type.cpp.i
.PHONY : src/Type.cpp.i

src/Type.s: src/Type.cpp.s
.PHONY : src/Type.s

# target to generate assembly for a file
src/Type.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/Type.cpp.s
.PHONY : src/Type.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/turbolang.dir/build.make CMakeFiles/turbolang.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... acc_gen"
	@echo "... intrinsics_gen"
	@echo "... omp_gen"
	@echo "... turbolang"
	@echo "... src/Builder.o"
	@echo "... src/Builder.i"
	@echo "... src/Builder.s"
	@echo "... src/Class.o"
	@echo "... src/Class.i"
	@echo "... src/Class.s"
	@echo "... src/Compiler.o"
	@echo "... src/Compiler.i"
	@echo "... src/Compiler.s"
	@echo "... src/Function.o"
	@echo "... src/Function.i"
	@echo "... src/Function.s"
	@echo "... src/FunctionCallProcessor.o"
	@echo "... src/FunctionCallProcessor.i"
	@echo "... src/FunctionCallProcessor.s"
	@echo "... src/FunctionInternal.o"
	@echo "... src/FunctionInternal.i"
	@echo "... src/FunctionInternal.s"
	@echo "... src/LLVMManager.o"
	@echo "... src/LLVMManager.i"
	@echo "... src/LLVMManager.s"
	@echo "... src/Logger.o"
	@echo "... src/Logger.i"
	@echo "... src/Logger.s"
	@echo "... src/MathEvaluator.o"
	@echo "... src/MathEvaluator.i"
	@echo "... src/MathEvaluator.s"
	@echo "... src/Parser.o"
	@echo "... src/Parser.i"
	@echo "... src/Parser.s"
	@echo "... src/SourceCodeReader.o"
	@echo "... src/SourceCodeReader.i"
	@echo "... src/SourceCodeReader.s"
	@echo "... src/TimeUtil.o"
	@echo "... src/TimeUtil.i"
	@echo "... src/TimeUtil.s"
	@echo "... src/Tokenizer.o"
	@echo "... src/Tokenizer.i"
	@echo "... src/Tokenizer.s"
	@echo "... src/Type.o"
	@echo "... src/Type.i"
	@echo "... src/Type.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
