#pragma once

#include "utils/compilermanager.h"
#include "utils/objectwrapper.h"
#include <string>
#include <vector>

namespace turbolang {
    class printer {
    public:
        static void prepare_printer();

        static void printf(const std::vector<llvm::Value*>& values);
    };
}
