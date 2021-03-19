#include "utils/Type.h"

namespace turbolang {
    struct data {
        int index;
        unsigned int bitCount;

        data() = default;

        data(int index) {
            this->index = index;
            this->bitCount = 0;
        }

        data(int index, unsigned int bitCount) {
            this->index = index;
            this->bitCount = bitCount;
        }
    };
    std::map<const std::string, data> variableTypeMap = {
            {"i8",  {0, 8}},
            {"i16", {1, 16}},
            {"i32",   {2, 32}},
            {"i64", {3, 64}},
            {"f32", 4},
            {"f64", 5},
            {"i8_ptr", 6},
            {"i16_ptr", 7},
            {"i32_ptr", 8},
            {"i64_ptr", 9},
            {"f32_ptr", 10},
            {"f64_ptr", 11}
    };

    std::map<const std::string, data> functionTypeMap = {
            {"void", 0},
            {"i8",  {1, 8}},
            {"i16",  {2, 16}},
            {"i32",    {3, 32}},
            {"i64",  {4, 64}},
            {"f32",  5},
            {"f64",  6},
            {"i8_ptr", 7},
            {"i16_ptr", 8},
            {"i32_ptr", 9},
            {"i64_ptr", 10},
            {"f32_ptr", 11},
            {"f64_ptr", 12}
    };

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
                return LLVMManager::llvmBytecodeBuilder->getInt8PtrTy();
            case 7:
                return llvm::IntegerType::getInt16PtrTy(*LLVMManager::llvmCtx);
            case 8:
                return llvm::IntegerType::getInt32PtrTy(*LLVMManager::llvmCtx);
            case 9:
                return llvm::IntegerType::getInt64PtrTy(*LLVMManager::llvmCtx);
            case 10:
                return llvm::IntegerType::getFloatPtrTy(*LLVMManager::llvmCtx);
            case 11:
                return llvm::IntegerType::getDoublePtrTy(*LLVMManager::llvmCtx);
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
                return LLVMManager::llvmBytecodeBuilder->getInt8PtrTy();
            case 8:
                return llvm::IntegerType::getInt16PtrTy(*LLVMManager::llvmCtx);
            case 9:
                return llvm::IntegerType::getInt32PtrTy(*LLVMManager::llvmCtx);
            case 10:
                return llvm::IntegerType::getInt64PtrTy(*LLVMManager::llvmCtx);
            case 11:
                return llvm::IntegerType::getFloatPtrTy(*LLVMManager::llvmCtx);
            case 12:
                return llvm::IntegerType::getDoublePtrTy(*LLVMManager::llvmCtx);
            default:
                return nullptr;
        }
    }

    std::optional<int> Type::getVariableType(const std::string &name) {
        if (variableTypeMap.find(name) == variableTypeMap.end()) {
            return std::nullopt;
        }
        return variableTypeMap[name].index;
    }

    std::optional<int> Type::getFunctionType(const std::string &name) {
        if (functionTypeMap.find(name) == functionTypeMap.end()) {
            return std::nullopt;
        }
        return functionTypeMap[name].index;
    }

    unsigned int Type::getVariableIntBitCount(const std::string& name) {
        if (variableTypeMap.find(name) == variableTypeMap.end()) {
            return 0;
        }
        return variableTypeMap[name].bitCount;
    }

    unsigned int Type::getFunctionIntBitCount(const std::string& name) {
        if (functionTypeMap.find(name) == functionTypeMap.end()) {
            return 0;
        }
        return functionTypeMap[name].bitCount;
    }
}
