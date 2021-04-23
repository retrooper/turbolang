#pragma once
#include <filesystem>
#include <thread>
#include <vector>
#include <stdio.h>
namespace turbolang {
    class Builder {
    public:
        static void buildExecutables();
    private:
        static volatile int tasksFinished;
    };
}