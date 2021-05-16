#pragma once
#include <filesystem>
#include <thread>
#include <vector>
#include <stdio.h>
#include <iostream>
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