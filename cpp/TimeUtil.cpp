#include "utils/TimeUtil.h"
auto startTime = std::chrono::high_resolution_clock::now();
long turbolang::getCurrentNanoTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
}

long turbolang::getCurrentMicroTime() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
}

long turbolang::getCurrentMillisTime() {
    auto now = std::chrono::high_resolution_clock::now();
    auto since_epoch = now - startTime;
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
    return millis.count();
}

long turbolang::getCurrentSecondsTime() {
    auto now = std::chrono::high_resolution_clock::now();
    auto since_epoch = now - startTime;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
    return seconds.count();
}