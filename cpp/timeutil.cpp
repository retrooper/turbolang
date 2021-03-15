#include "utils/timeutil.h"
auto startTime = std::chrono::high_resolution_clock::now();
long turbolang::get_current_nano() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
}

long turbolang::get_current_millis() {
    auto now = std::chrono::high_resolution_clock::now();
    auto since_epoch = now - startTime;
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
    return millis.count();
}

long turbolang::get_current_seconds() {
    auto now = std::chrono::high_resolution_clock::now();
    auto since_epoch = now - startTime;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
    return seconds.count();
}