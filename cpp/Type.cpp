#include "utils/Type.h"

namespace turbolang {
    struct TypeData {
        llvm::Type* type;
        int index;
        TypeData(llvm::Type* type, int index) {
            this->type = type;
            this->index = index;
        }
    };
    std::map<const std::string, TypeData> typeMap;

    void initMap() {
        typeMap = {
                {"void", {llvm::Type::getVoidTy(*LLVMManager::llvmCtx), 0}},
                {"bool", {llvm::Type::getInt1Ty(*LLVMManager::llvmCtx), 1}},
                {"bool_ptr", {llvm::Type::getInt1PtrTy(*LLVMManager::llvmCtx), 2}},
                {"i8",  {llvm::Type::getInt8Ty(*LLVMManager::llvmCtx), 3}},
                {"u8", {llvm::Type::getInt8Ty(*LLVMManager::llvmCtx), 4}},
                {"i8_ptr", {llvm::Type::getInt8PtrTy(*LLVMManager::llvmCtx), 5}},
                {"u8_ptr", {llvm::Type::getInt8PtrTy(*LLVMManager::llvmCtx), 6}},
                {"i16", {llvm::Type::getInt16Ty(*LLVMManager::llvmCtx), 7}},
                {"u16", {llvm::Type::getInt16Ty(*LLVMManager::llvmCtx), 8}},
                {"i16_ptr", {llvm::Type::getInt16PtrTy(*LLVMManager::llvmCtx), 9}},
                {"u16_ptr", {llvm::Type::getInt16PtrTy(*LLVMManager::llvmCtx), 10}},
                {"i32",   {llvm::Type::getInt32Ty(*LLVMManager::llvmCtx), 11}},
                {"u32",   {llvm::Type::getInt32Ty(*LLVMManager::llvmCtx), 12}},
                {"i32_ptr", {llvm::Type::getInt32PtrTy(*LLVMManager::llvmCtx), 13}},
                {"u32_ptr", {llvm::Type::getInt32PtrTy(*LLVMManager::llvmCtx), 14}},
                {"i64", {llvm::Type::getInt64Ty(*LLVMManager::llvmCtx), 15}},
                {"u64", {llvm::Type::getInt64Ty(*LLVMManager::llvmCtx), 16}},
                {"i64_ptr", {llvm::Type::getInt64PtrTy(*LLVMManager::llvmCtx), 17}},
                {"ui64_ptr", {llvm::Type::getInt64PtrTy(*LLVMManager::llvmCtx), 18}},
                {"f32",{ llvm::Type::getFloatTy(*LLVMManager::llvmCtx), 19}},
                {"f32_ptr", {llvm::Type::getFloatPtrTy(*LLVMManager::llvmCtx), 20}},
                {"f64", {llvm::Type::getDoubleTy(*LLVMManager::llvmCtx), 21}},
                {"f64_ptr", {llvm::Type::getDoublePtrTy(*LLVMManager::llvmCtx), 22}}
        };
    }

    llvm::Type *Type::getLLVMType(const int& type) {
        if (typeMap.empty()) {
            initMap();
        }
        for (auto iter = typeMap.rbegin(); iter != typeMap.rend(); ++iter) {
            if (type == iter->second.index) {
                return iter->second.type;
            }
        }
        return nullptr;
    }

    std::optional<DataType> Type::getType(const std::string &name) {
        if (typeMap.empty()) {
            initMap();
        }
        for (auto iter = typeMap.rbegin(); iter != typeMap.rend(); ++iter) {
            if (iter->first == name) {
                return static_cast<DataType>(iter->second.index);
            }
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
