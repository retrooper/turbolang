#include "utils/Type.h"

namespace turbolang {
    std::map<const std::string, llvm::Type*> typeMap;

    void initMap() {
        typeMap = {
                {"void", llvm::Type::getVoidTy(*LLVMManager::llvmCtx)},
                {"bool", llvm::Type::getInt1Ty(*LLVMManager::llvmCtx)},
                {"bool_ptr", llvm::Type::getInt1PtrTy(*LLVMManager::llvmCtx)},
                {"int8",  llvm::Type::getInt8Ty(*LLVMManager::llvmCtx)},
                {"uint8", llvm::Type::getInt8Ty(*LLVMManager::llvmCtx)},
                {"int8_ptr", llvm::Type::getInt8PtrTy(*LLVMManager::llvmCtx)},
                {"uint8_ptr", llvm::Type::getInt8PtrTy(*LLVMManager::llvmCtx)},
                {"int16", llvm::Type::getInt16Ty(*LLVMManager::llvmCtx)},
                {"uint16", llvm::Type::getInt16Ty(*LLVMManager::llvmCtx)},
                {"int16_ptr", llvm::Type::getInt16PtrTy(*LLVMManager::llvmCtx)},
                {"uint16_ptr", llvm::Type::getInt16PtrTy(*LLVMManager::llvmCtx)},
                {"int32",   llvm::Type::getInt32Ty(*LLVMManager::llvmCtx)},
                {"uint32",   llvm::Type::getInt32Ty(*LLVMManager::llvmCtx)},
                {"int32_ptr", llvm::Type::getInt32PtrTy(*LLVMManager::llvmCtx)},
                {"uint32_ptr", llvm::Type::getInt32PtrTy(*LLVMManager::llvmCtx)},
                {"int64", llvm::Type::getInt64Ty(*LLVMManager::llvmCtx)},
                {"uint64", llvm::Type::getInt64Ty(*LLVMManager::llvmCtx)},
                {"int64_ptr", llvm::Type::getInt64PtrTy(*LLVMManager::llvmCtx)},
                {"uint64_ptr", llvm::Type::getInt64PtrTy(*LLVMManager::llvmCtx)},
                {"f32", llvm::Type::getFloatTy(*LLVMManager::llvmCtx)},
                {"f32_ptr", llvm::Type::getFloatPtrTy(*LLVMManager::llvmCtx)},
                {"f64", llvm::Type::getDoubleTy(*LLVMManager::llvmCtx)},
                {"f64_ptr", llvm::Type::getDoublePtrTy(*LLVMManager::llvmCtx)}
        };
    }

    llvm::Type *Type::getLLVMType(const int& type) {
        if (typeMap.empty()) {
            initMap();
        }
        int index = 0;
        for (auto iter = typeMap.rbegin(); iter != typeMap.rend(); ++iter) {
            if (index == type) {
                return iter->second;
            }
            index++;
        }
        return nullptr;
    }

    std::optional<DataType> Type::getType(const std::string &name) {
        if (typeMap.empty()) {
            initMap();
        }
        int index = 0;
        for (auto iter = typeMap.rbegin(); iter != typeMap.rend(); ++iter) {
            if (iter->first == name) {
                return static_cast<DataType>(index);
            }
            index++;
        }
        return std::nullopt;
    }

    unsigned int Type::getIntBitCount(const std::string& name) {
        auto type = getType(name);
        if (type.has_value()) {
            return getLLVMType(type.value())->getIntegerBitWidth();
        }
        return -1;
    }
}
