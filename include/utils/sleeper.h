#pragma once
#include "compilermanager.h"
#include <unistd.h>
#include <thread>
namespace turbolang {
    class sleeper {
    public:
        static void prepare();
        static llvm::Value* sleep(llvm::Value *arg);
    };
}