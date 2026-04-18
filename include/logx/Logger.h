#pragma once
#include "defines.h"

#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <unordered_map>

namespace logx {
class Logger {
public:
    Logger(std::ostream &stream = std::cout, std::wostream &wstream = std::wcout);
    virtual ~Logger() = default;

    std::ostream &getStream() const;
    std::wostream &getWStream() const;
    const char *getLevelAnsiColor(Level level) const;

    const char *getLevelName(Level level) const;

    void setStream(std::ostream &stream);
    void setWStream(std::wostream &wstream);
    void setLevelColor(Level level, Color color);
    void setLevelColor(Level level, const char *ansiColor);

    void log(Level level, const std::string &message,
             const std::source_location &sourceLocation = std::source_location::current());
    void log(Level level, const std::wstring &message,
             const std::source_location &sourceLocation = std::source_location::current());

    void critical(const std::string &message,
                  const std::source_location &sourceLocation = std::source_location::current());
    void error(const std::string &message,
               const std::source_location &sourceLocation = std::source_location::current());
    void warning(const std::string &message,
                 const std::source_location &sourceLocation = std::source_location::current());
    void info(const std::string &message,
              const std::source_location &sourceLocation = std::source_location::current());
    void debug(const std::string &message,
               const std::source_location &sourceLocation = std::source_location::current());
    void trace(const std::string &message,
               const std::source_location &sourceLocation = std::source_location::current());

    void critical(const std::wstring &message,
                  const std::source_location &sourceLocation = std::source_location::current());
    void error(const std::wstring &message,
               const std::source_location &sourceLocation = std::source_location::current());
    void warning(const std::wstring &message,
                 const std::source_location &sourceLocation = std::source_location::current());
    void info(const std::wstring &message,
              const std::source_location &sourceLocation = std::source_location::current());
    void debug(const std::wstring &message,
               const std::source_location &sourceLocation = std::source_location::current());
    void trace(const std::wstring &message,
               const std::source_location &sourceLocation = std::source_location::current());

protected:
    virtual void log_Unsafe(Level level, const std::string &message,
                            const std::source_location &sourceLocation);
    virtual void log_Unsafe(Level level, const std::wstring &message,
                            const std::source_location &sourceLocation);
    virtual const char *getLevelName_Unsafe(Level level) const;

private:
    const char *getLevelAnsiColor_Unsafe(Level level) const;

    std::ostream *_stream;
    std::wostream *_wstream;
    mutable std::mutex _mutex{};

    std::unordered_map<Level, const char *> _levelColors;
};
} // namespace logx