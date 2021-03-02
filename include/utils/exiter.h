#pragma once
#include "compilermanager.h"
#include <stdlib.h>
namespace turbolang {
    class exiter {
    public:
        static void prepare_exiter();
        static void exit(int code);
    };
}