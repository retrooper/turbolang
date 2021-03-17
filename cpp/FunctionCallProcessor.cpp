#include "function/FunctionCallProcessor.h"

namespace turbolang {
    std::map<std::string, FunctionInternal*> FunctionCallProcessor::internalFunctions;
    void FunctionCallProcessor::load() {
        internalFunctions["printf"] = new PrintfFunctionInternal();
        internalFunctions["println"] = new PrintlnFunctionInternal();
        internalFunctions["scanf"] = new ScanfFunctionInternal();
        internalFunctions["exit"] = new ExitFunctionInternal();
        internalFunctions["sleep"] = new SleepFunctionInternal();
        internalFunctions["system"] = new SystemFunctionInternal();

        for (const auto& internalFunctionIt : internalFunctions) {
            internalFunctionIt.second->load();
        }
    }

    void FunctionCallProcessor::unload() {
        for (auto& funcInternal : internalFunctions) {
            delete funcInternal.second;
        }
    }

    llvm::Value *
    turbolang::FunctionCallProcessor::process(std::string functionName, std::vector<llvm::Value *> &arguments) {
        //INTERNAL FUNCTION
        if (internalFunctions.find(functionName) != internalFunctions.end()) {
            FunctionInternal* functionInternal = internalFunctions[functionName];
            bool argumentTypesMatch = true;
            for (llvm::Value* argument : arguments) {
                //TODO check if the argument types match, if not, set var to false and break
            }
            if (argumentTypesMatch) {
                return functionInternal->execute(arguments);
            }
        }
        //EXTERNAL FUNCTION
        llvm::Function* llvmFunction = LLVMManager::llvmModule->getFunction(functionName);
        return LLVMManager::llvmBytecodeBuilder->CreateCall(llvmFunction, arguments);
    }

    bool FunctionCallProcessor::isInbuilt(const std::string& name) {
        return internalFunctions.find(name) != internalFunctions.end();
    }
}
