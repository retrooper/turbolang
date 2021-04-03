#pragma once
#include <filesystem>
#include <thread>
namespace turbolang {
    class Builder {
    public:
        static void buildExecutables();
    private:
        static volatile int tasksFinished;
    };
}