#pragma once
#include "compilermanager.h"
#include <stdlib.h>
namespace turbolang {
    class exiter {
    public:
        static void prepare();
        static void exit(llvm::Value* exitCodeValue);
    };
}