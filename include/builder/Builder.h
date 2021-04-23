#pragma once
#include <filesystem>
#include <thread>
#include <vector>
namespace turbolang {
    class Builder {
    public:
        static void buildExecutables();
    private:
        static volatile int tasksFinished;
    };
}