#pragma once

#include "utils/llvm/LLVMManager.h"
#include <vector>

namespace turbolang {
#define FUNCTION_INTERNAL_OVERRIDE_DEFINITION void load() override; llvm::Value* execute(std::vector<llvm::Value*>& arguments) override

    class FunctionInternal {
    protected:
        static void defineFunction(const std::string &functionName, llvm::Type *returnType,
                                   std::vector<llvm::Type *> &argumentTypes, bool argumentSizeConstant);

        static void defineFunction(const std::string &functionName, llvm::Type *returnType, llvm::Type *argumentType,
                                   bool argumentSizeConstant);

        static llvm::Value *callFunction(const std::string &functionName, std::vector<llvm::Value *> arguments);

        static llvm::Value *callFunction(const std::string &functionName, llvm::Value *argument);

    public:
        virtual void load();

        virtual llvm::Value *execute(std::vector<llvm::Value *> &arguments);
    };

    class PrintfFunctionInternal : public FunctionInternal {
    public:
        FUNCTION_INTERNAL_OVERRIDE_DEFINITION;
    };

    class PrintlnFunctionInternal : public FunctionInternal {
    public:
        FUNCTION_INTERNAL_OVERRIDE_DEFINITION;
    };

    class ExitFunctionInternal : public FunctionInternal {
        FUNCTION_INTERNAL_OVERRIDE_DEFINITION;
    };

    class SleepFunctionInternal : public FunctionInternal {
        FUNCTION_INTERNAL_OVERRIDE_DEFINITION;
    };

    class SystemFunctionInternal : public FunctionInternal {
        FUNCTION_INTERNAL_OVERRIDE_DEFINITION;
    };

    class ScanfFunctionInternal : public FunctionInternal {
        FUNCTION_INTERNAL_OVERRIDE_DEFINITION;
    };

#undef FUNCTION_INTERNAL_OVERRIDE_DEFINITION
}