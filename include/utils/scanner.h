#pragma once
#include "compilermanager.h"
#include <vector>
namespace turbolang {
    class scanner {
    public:
        static void prepare();
        static llvm::Value* scanf(const std::vector<llvm::Value *> &values);
    };
}