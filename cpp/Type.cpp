#include "utils/Type.h"

namespace turbolang {
    struct TypeData {
        llvm::Type *type;
        int index;
        bool isSigned;
        bool validSign = true;
        TypeData(llvm::Type *type, int index, bool isSigned) {
            this->type = type;
            this->index = index;
            this->isSigned = isSigned;
        }

        TypeData(llvm::Type *type, int index, bool isSigned, bool validSign) {
            this->type = type;
            this->index = index;
            this->isSigned = isSigned;
            this->validSign = validSign;
        }
    };

    std::map<const std::string, TypeData> typeMap;

    void initMap() {
        typeMap = {
                {"void",     {llvm::Type::getVoidTy(*LLVMManager::llvmCtx),      0, true, false}},
                {"bool",     {llvm::Type::getInt1Ty(*LLVMManager::llvmCtx),      1, true, false}},
                {"i8",       {llvm::Type::getInt8Ty(*LLVMManager::llvmCtx),      2, true}},
                {"u8",       {llvm::Type::getInt8Ty(*LLVMManager::llvmCtx),      3, false}},
                {"i16",      {llvm::Type::getInt16Ty(*LLVMManager::llvmCtx),     4, true}},
                {"u16",      {llvm::Type::getInt16Ty(*LLVMManager::llvmCtx),     5, false}},
                {"i32",      {llvm::Type::getInt32Ty(*LLVMManager::llvmCtx),     6, true}},
                {"u32",      {llvm::Type::getInt32Ty(*LLVMManager::llvmCtx),     7, false}},
                {"i64",      {llvm::Type::getInt64Ty(*LLVMManager::llvmCtx),     8, true}},
                {"u64",      {llvm::Type::getInt64Ty(*LLVMManager::llvmCtx),     9, false}},
                {"f32",      {llvm::Type::getFloatTy(*LLVMManager::llvmCtx),     10, true, false}},
                {"f64",      {llvm::Type::getDoubleTy(*LLVMManager::llvmCtx),    11, true, false}},
        };
    }

    llvm::Type *Type::getLLVMType(const int &type, const std::string &extraData) {
        if (type == DATA_TYPE_CLASS) {
            return Class::classMap[extraData].structType;
        }
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
        if (Class::classMap.find(name) != Class::classMap.end()) {
            return DATA_TYPE_CLASS;
        }
        return std::nullopt;
    }

    std::optional<DataType> Type::getType(const llvm::Type *type, const bool& isSigned) {
        if (llvm::isa<llvm::StructType>(type)) {
            return DATA_TYPE_CLASS;
        }
        if (typeMap.empty()) {
            initMap();
        }

        for (const auto& iter : typeMap) {
            bool sameID = iter.second.type->getTypeID() == type->getTypeID();
            bool found = sameID && (!iter.second.validSign || iter.second.isSigned == isSigned);
            if (found) {
                return static_cast<DataType>(iter.second.index);
            }
        }
        return std::nullopt;
    }

    bool Type::isTypeSigned(const int &type) {
        for (const auto& iter : typeMap) {
            bool found = iter.second.validSign && iter.second.isSigned;
            if (found) {
                return static_cast<DataType>(iter.second.index);
            }
        }
        return false;
    }
}
