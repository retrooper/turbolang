#pragma once

#include <iostream>
#include <string>

enum LogMode {
    LOG_MODE_ALL, LOG_MODE_WARNING, LOG_MODE_ERROR, LOG_MODE_NONE
};
namespace turbolang{
    struct Logger {
        static LogMode mode;
    };
}

#define LOG_INFO(msg) if (turbolang::Logger::mode == LOG_MODE_ALL) { std::cout << "[TURBOLANG INFO] " << msg << std::endl; }
#define LOG_WARN(msg) if (turbolang::Logger::mode == LOG_MODE_ALL || turbolang::Logger::mode == LOG_MODE_WARNING) { std::cout << "[TURBOLANG WARNING] " << msg << std::endl; }
#define LOG_ERROR(msg) if (turbolang::Logger::mode == LOG_MODE_ALL || turbolang::Logger::mode == LOG_MODE_ERROR) { std::cerr << "[TURBOLANG ERROR] " << msg << std::endl; std::exit(-1); }