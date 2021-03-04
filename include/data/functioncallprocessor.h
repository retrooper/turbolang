#pragma once
#include "utils/printer.h"
#include "tokenizer/tokenizer.h"
#include "data/functiondefinition.h"
#include <string>
namespace turbolang {
    class functioncallprocessor {
    public:
        functioncallprocessor() = default;
        virtual void process(const turbolang::functiondefinition& currentFunction, std::vector<llvm::Value*>& arguments);
    };

    class printfcallprocessor : public functioncallprocessor {
        void process(const turbolang::functiondefinition& currentFunction, std::vector<llvm::Value*>& arguments) override;
    };
}