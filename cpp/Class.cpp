#include "class/Class.h"
namespace turbolang {
    std::map<std::string, Class> Class::classMap;
    Class::Class(const std::string &name) {
        this->name = name;
        this->structType = nullptr;
    }

    void Class::create() {
        structType = llvm::StructType::create(*LLVMManager::llvmCtx, name);
    }

    unsigned int Class::getMemberIndex(const std::string &memberName) {
        int index = 0;
        for (const auto& member : clsMemberData) {
            if (member.name == memberName) {
                return index;
            }
            index++;
        }
        return -1;
    }
}