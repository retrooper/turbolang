#pragma once

#include "utils/compilermanager.h"
#include "utils/objectwrapper.h"
#include <string>
#include <vector>

namespace turbolang {
    class printer {
    public:
        static void prepare_printer();

        static void printf(const std::string &format, const std::vector<turbolang::objectwrapper>& values);

        static void print(const std::string& message);
        static void println(const std::string& message);
    };
}
