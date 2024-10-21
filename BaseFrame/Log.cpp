#include <iostream>
#include <chrono>
#include <ctime>

int GetCurrentYear() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm t{};
    localtime_r(&time, &t);
    return t.tm_year + 1900;
}

int GetCurrentMonth() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm t{};
    localtime_r(&time, &t);
    return t.tm_mon + 1;
}

int GetCurrentDay() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm t{};
    localtime_r(&time, &t);
    return t.tm_mday;
}

int GetCurrentHour() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm t{};
    localtime_r(&time, &t);
    return t.tm_hour;
}

int GetCurrentMinute() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm t{};
    localtime_r(&time, &t);
    return t.tm_min;
}

int GetCurrentSecond() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm t{};
    localtime_r(&time, &t);
    return t.tm_sec;
}

int GetCurrentMillisecond() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
    return static_cast<int>(milliseconds.count());
}
