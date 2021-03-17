#include "utils/Type.h"

namespace turbolang {
    std::map<const std::string, int> variableTypeMap = {
            {"i8",  0},
            {"i16", 1},
            {"i32",   2},
            {"i64", 3},
            {"f32", 4},
            {"f64", 5},
            {"string", 6}
    };

    std::map<const std::string, int> functionTypeMap = {
            {"void", 0},
            {"i8",  1},
            {"i16",  2},
            {"i32",    3},
            {"i64",  4},
            {"f32",  5},
            {"f64",  6},
            {"string",  7},
    };

    llvm::Type* getLLVMTypeByVariableType(int type) {
        switch (type) {
            case 0:
                return LLVMManager::llvmBytecodeBuilder->getInt8Ty();
            case 1:
                return LLVMManager::llvmBytecodeBuilder->getInt16Ty();
            case 2:
                return LLVMManager::llvmBytecodeBuilder->getInt32Ty();
            case 3:
                return LLVMManager::llvmBytecodeBuilder->getInt64Ty();
            case 4:
                return LLVMManager::llvmBytecodeBuilder->getFloatTy();
            case 5:
                return LLVMManager::llvmBytecodeBuilder->getDoubleTy();
            case 6:
                return LLVMManager::llvmBytecodeBuilder->getInt8PtrTy();//byte pointer
            default:
                return nullptr;
        }
    }


    llvm::Type *Type::getLLVMTypeByVariableType(int type) {
        switch (type) {
            case 0:
                return LLVMManager::llvmBytecodeBuilder->getInt8Ty();
            case 1:
                return LLVMManager::llvmBytecodeBuilder->getInt16Ty();
            case 2:
                return LLVMManager::llvmBytecodeBuilder->getInt32Ty();
            case 3:
                return LLVMManager::llvmBytecodeBuilder->getInt64Ty();
            case 4:
                return LLVMManager::llvmBytecodeBuilder->getFloatTy();
            case 5:
                return LLVMManager::llvmBytecodeBuilder->getDoubleTy();
            case 6:
                return LLVMManager::llvmBytecodeBuilder->getInt8PtrTy();//byte pointer
            default:
                return nullptr;
        }
    }

    llvm::Type* Type::getLLVMTypeByFunctionType(int type) {
        switch (type) {
            case 0:
                return LLVMManager::llvmBytecodeBuilder->getVoidTy();
            case 1:
                return LLVMManager::llvmBytecodeBuilder->getInt8Ty();
            case 2:
                return LLVMManager::llvmBytecodeBuilder->getInt16Ty();
            case 3:
                return LLVMManager::llvmBytecodeBuilder->getInt32Ty();
            case 4:
                return LLVMManager::llvmBytecodeBuilder->getInt64Ty();
            case 5:
                return LLVMManager::llvmBytecodeBuilder->getFloatTy();
            case 6:
                return LLVMManager::llvmBytecodeBuilder->getDoubleTy();
            case 7:
                return LLVMManager::llvmBytecodeBuilder->getInt8PtrTy();//byte pointer
            default:
                return nullptr;
        }
    }

    std::optional<int> Type::getVariableType(const std::string &name) {
        if (variableTypeMap.find(name) == variableTypeMap.end()) {
            return std::nullopt;
        }
        return variableTypeMap[name];
    }

    std::optional<int> Type::getFunctionType(const std::string &name) {
        if (functionTypeMap.find(name) == functionTypeMap.end()) {
            return std::nullopt;
        }
        return functionTypeMap[name];
    }
}
