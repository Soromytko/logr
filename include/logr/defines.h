#pragma once
#include <iostream>

namespace logr {
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
} // namespace logr

#define LOGR_FILE static_cast<const char *>(__FILE__)
#define LOGR_LINE static_cast<size_t>(__LINE__)
#if defined(_MSC_VER)
#define LOGR_FUNC static_cast<const char *>(__FUNCSIG__)
#elif defined(__GNUC__) || defined(__clang__)
#define LOGR_FUNC static_cast<const char *>(__PRETTY_FUNCTION__)
#else
#define LOGR_FUNC static_cast<const char *>(__func__)
#endif

#define LOGR_CLASS (typeid(*this).name())
