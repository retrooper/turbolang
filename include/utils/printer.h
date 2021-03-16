#pragma once

#include "utils/compilermanager.h"
#include <string>
#include <vector>

namespace turbolang {
    class printer {
    public:
        static void prepare();
        static llvm::Value* printf(const std::vector<llvm::Value *> &values);
        static llvm::Value* println(const std::vector<llvm::Value *> &values);
    };
}
