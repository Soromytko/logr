#pragma once
#include <iostream>

namespace logx {
enum class Level {
    critical,
    error,
    warning,
    info,
    debug,
    trace,
};

enum class Color {
    def,
    black,
    darkRed,
    darkGreen,
    darkYellow,
    darkBlue,
    darkMagenta,
    darkCyan,
    darkWhite,
    gray,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
};
} // namespace logx

#define LOGX_FILE static_cast<const char *>(__FILE__)
#define LOGX_LINE static_cast<size_t>(__LINE__)
#if defined(_MSC_VER)
#define LOGX_FUNC static_cast<const char *>(__FUNCSIG__)
#elif defined(__GNUC__) || defined(__clang__)
#define LOGX_FUNC static_cast<const char *>(__PRETTY_FUNCTION__)
#else
#define LOGX_FUNC static_cast<const char *>(__func__)
#endif

#define LOGX_CLASS (typeid(*this).name())
