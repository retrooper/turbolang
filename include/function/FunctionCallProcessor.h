#pragma once
#include "function/internal/FunctionInternal.h"
#include <map>
#include <iostream>
namespace turbolang {
    class FunctionCallProcessor {
    public:
        static llvm::Value* process(std::string functionName, std::vector<llvm::Value*>& arguments);
    };
}