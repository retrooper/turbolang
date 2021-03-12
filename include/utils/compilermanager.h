#pragma once

#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "parser/parser.h"
#include "data/functioncallprocessor.h"
#include <string>

namespace turbolang {
    class compilermanager {
    public:
        static bool output_to_file;
        static llvm::LLVMContext llvmContext;
        static llvm::IRBuilder<> llvmIRBuilder;
        static std::unique_ptr<llvm::Module> llvmModule;
        static std::map<std::string, class functioncallprocessor*> functionCallProcessorMap;
        static std::map<std::string, functiondefinition> functions;
        static void prepare();
        static void generate_byte_code(int exitCode);
        static void cleanup_byte_code(class parser& parser);
        friend class parser;
        static void generate_binary();
        static void execute_binary();
    };
}