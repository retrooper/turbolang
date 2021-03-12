#pragma once
#include "utils/printer.h"
#include "tokenizer/tokenizer.h"
#include <string>
namespace turbolang {
    class functiondefinition;
    class functioncallprocessor {
    public:
        functioncallprocessor() = default;
        virtual llvm::Value* process(functiondefinition& targetFunction, std::vector<llvm::Value*>& arguments);
        static llvm::Value * external_func_process(functiondefinition& currentFunction, std::vector<llvm::Value*>& arguments);
    };

    class printfcallprocessor : public functioncallprocessor {
        llvm::Value* process(functiondefinition& currentFunction, std::vector<llvm::Value*>& arguments) override;
    };

    class exitcallprocessor : public functioncallprocessor {
        llvm::Value* process(functiondefinition& currentFunction, std::vector<llvm::Value*>& arguments) override;
    };
}