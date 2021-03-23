#pragma once

#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "parser/Parser.h"
#include "function/FunctionCallProcessor.h"
#include <filesystem>
#include <string>
#include <thread>

namespace turbolang {
    class Compiler {
    public:
        static volatile int tasksFinished;

        static void generate_byte_code();

        static void generate_binary();

        static void generate_executables();

        static void execute_binary();
    };

}