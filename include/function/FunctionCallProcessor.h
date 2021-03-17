#pragma once
#include "function/internal/FunctionInternal.h"
#include <map>
namespace turbolang {
    class FunctionCallProcessor {
    private:
        static std::map<std::string, FunctionInternal*> internalFunctions;
    public:
        static void load();
        static void unload();
        static bool isInbuilt(const std::string& name);
        static llvm::Value* process(std::string functionName, std::vector<llvm::Value*>& arguments);
    };
}