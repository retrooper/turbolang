#pragma once
#include "type.h"
#include "parameterdefinition.h"
#include "utils/compilermanager.h"
#include <string>
#include <vector>
#include <map>
namespace turbolang {
    struct functiondefinition {
        functiondefinition() = default;
        std::string name;
        std::vector<parameterdefinition> parameters;
        std::map<std::string, llvm::Value*> variableMap;
        std::map<std::string, llvm::AllocaInst*> allocaMap;
    };
}