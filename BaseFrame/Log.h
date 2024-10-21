#pragma once

#include <iostream>
#include <format>

int GetCurrentYear();
int GetCurrentMonth();
int GetCurrentDay();
int GetCurrentHour();
int GetCurrentMinute();
int GetCurrentSecond();
int GetCurrentMillisecond();

template <class T>
requires std::same_as<T, const char *>
static constexpr T SplitFileName(T fileName) {
    std::size_t length = 0;
    while(fileName[length] != '\0') {
        ++length;
    }

    for(int i = static_cast<int>(length) - 1; i >= 0; i--) {
        if(fileName[i] == '/') {
            return &fileName[i + 1];
        }
    }

    return fileName;
}

// color
#define COLOR_RESET        "\033[0m"   // 重置颜色
#define COLOR_DEFAULT      COLOR_RESET
#define RED                "\033[31m"  // 红色
#define GREEN              "\033[32m"  // 绿色
#define YELLOW             "\033[33m"  // 黄色
#define BLUE               "\033[34m"  // 蓝色
#define MAGENTA            "\033[35m"  // 品红
#define CYAN               "\033[36m"  // 青色
#define WHITE              "\033[37m"  // 白色
#define BACKGROUND_BLACK   "\033[40m"  // 黑色背景
#define BACKGROUND_RED     "\033[41m"  // 红色背景
#define BACKGROUND_GREEN   "\033[42m"  // 绿色背景
#define BACKGROUND_YELLOW  "\033[43m"  // 黄色背景
#define BACKGROUND_BLUE    "\033[44m"  // 蓝色背景
#define BACKGROUND_MAGENTA "\033[45m"  // 紫色背景
#define BACKGROUND_CYAN    "\033[46m"  // 青色背景
#define BACKGROUND_WHITE   "\033[47m"  // 白色背景

#define COLOR(__text, __color) __color __text COLOR_RESET
#define BTCOLOR(__text, __background_color, __text_color) __background_color __text_color __text COLOR_RESET

#define LOG_ERROR_IMPL(__fmt, ...)                  \
do {                                                \
    std::cout << std::format(__fmt, ##__VA_ARGS__); \
    std::cout.flush();                              \
} while(false)

#define WHERE_LOG_HOLDER "[{}:{}:{}]"
#define WHERE_LOG_VALUE SplitFileName(__FILE__), __func__, __LINE__

#define WHEN_LOG_HOLDER "{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}"
#define WHEN_LOG_VALUE GetCurrentYear(), GetCurrentMonth(), GetCurrentDay(), GetCurrentHour(), GetCurrentMinute(), GetCurrentSecond(), GetCurrentMillisecond()

#define LOG_ERROR(__log_type, __fmt, ...) LOG_ERROR_IMPL(__log_type " " WHEN_LOG_HOLDER " " WHERE_LOG_HOLDER " " __fmt "\n", WHEN_LOG_VALUE, WHERE_LOG_VALUE, ##__VA_ARGS__)

#define RERROR(__fmt, ...) LOG_ERROR(COLOR("BaseFrame", YELLOW), __fmt, ##__VA_ARGS__)
#define WERROR(__fmt, ...) LOG_ERROR(COLOR("Biz", CYAN), __fmt, ##__VA_ARGS__)
#define UERROR(__fmt, ...) LOG_ERROR(COLOR("Unexpected", RED), __fmt, ##__VA_ARGS__)
