#pragma once
#include "compilermanager.h"
#include <stdio.h>
#include <thread>
namespace turbolang {
    class sleeper {
    public:
        static void sleep(long ms);
    };
}