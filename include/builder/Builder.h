#pragma once
#include <filesystem>
#include <thread>
#include <vector>
#include <cstdio>
#include <iostream>
#include "utils/Logger.h"
namespace turbolang {
    class Builder {
    public:
        static void buildExecutables();
    private:
        static volatile int tasksFinished;
    public:
        static std::vector<std::string> libraries;
    };
}