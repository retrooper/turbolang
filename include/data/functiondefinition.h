#pragma once
#include "type.h"
#include "parameterdefinition.h"
#include <string>
#include <vector>

namespace turbolang {
    struct functiondefinition {
    public:
        std::string name;
        std::vector<parameterdefinition> parameters;
    };
}