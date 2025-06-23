#include <logx/Logger.h>
#include <cassert>
#include <cstring>

#define ANSI_COLOR_DEFAULT "\033[0m"

#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>

static bool s_windows_is_ansi_enabled = false;

static void windows_enable_ansi()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}
#endif

namespace logx
{
	static const char* get_ansi_color_code(Color color)
	{
		switch (color)
		{
		case Color::def: return "\033[0m";
		case Color::black: return "\033[30m";
		case Color::darkRed: return "\033[31m";
		case Color::darkGreen: return "\033[32m";
		case Color::darkYellow: return "\033[33m";
		case Color::darkBlue: return "\033[34m";
		case Color::darkMagenta: return "\033[35m";
		case Color::darkCyan: return "\033[36m";
		case Color::darkWhite: return "\033[37m";
		case Color::gray: return "\033[90m";
		case Color::red: return "\033[91m";
		case Color::green: return "\033[92m";
		case Color::yellow: return "\033[93m";
		case Color::blue: return "\033[94m";
		case Color::magenta: return "\033[95m";
		case Color::cyan: return "\033[96m";
		case Color::white: return "\033[97m";
		}

		return ANSI_COLOR_DEFAULT;
	}

	Logger::Logger(std::ostream& stream, std::wostream& wstream) : _stream(&stream), _wstream(&wstream)
	{
#if defined(WIN32)
		if (!s_windows_is_ansi_enabled)
		{
			windows_enable_ansi();
			s_windows_is_ansi_enabled = true;
		}
#endif
		_levelColors =
		{
			{ Level::critical, get_ansi_color_code(Color::red) },
			{ Level::error, get_ansi_color_code(Color::darkRed) },
			{ Level::warning, get_ansi_color_code(Color::darkYellow) },
			{ Level::info, get_ansi_color_code(Color::darkCyan) },
			{ Level::debug, get_ansi_color_code(Color::darkGreen) },
			{ Level::trace, get_ansi_color_code(Color::gray) },
		};
	}

	std::ostream& Logger::getStream() const
	{
		return *_stream;
	}

	std::wostream& Logger::getWStream() const
	{
		return *_wstream;
	}

	const char* Logger::getLevelAnsiColor(Level level) const
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return getLevelAnsiColor_Unsafe(level);
	}

	const char* Logger::getLevelName(Level level) const
	{
		std::lock_guard<std::mutex> lock(_mutex);

		return getLevelName_Unsafe(level);
	}

	void Logger::setStream(std::ostream& stream)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_stream = &stream;
	}

	void Logger::setWStream(std::wostream& wstream)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_wstream = &wstream;
	}

	void Logger::setLevelColor(Level level, Color color)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		auto it = _levelColors.find(level);
		if (it == _levelColors.end())
		{
			return;
		}

		it->second = get_ansi_color_code(color);
	}

	void Logger::setLevelColor(Level level, const char* ansiColor)
	{
		// TODO: It would be good to check the correctness of the ANSI code.

		std::lock_guard<std::mutex> lock(_mutex);

		auto it = _levelColors.find(level);
		if (it == _levelColors.end())
		{
			return;
		}

		it->second = ansiColor;
	}

	void Logger::log(Level level, const std::string& message, const std::source_location& sourceLocation)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		log_Unsafe(level, message, sourceLocation);
	}

	void Logger::log(Level level, const std::wstring& message, const std::source_location& sourceLocation)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		log_Unsafe(level, message, sourceLocation);
	}

	void Logger::critical(const std::string& message, const std::source_location& sourceLocation)
	{
		log(Level::critical, message, sourceLocation);
	}

	void Logger::error(const std::string& message, const std::source_location& sourceLocation)
	{
		log(Level::error, message, sourceLocation);
	}

	void Logger::warning(const std::string& message, const std::source_location& sourceLocation)
	{
		log(Level::warning, message, sourceLocation);
	}

	void Logger::info(const std::string& message, const std::source_location& sourceLocation)
	{
		log(Level::info, message, sourceLocation);
	}

	void Logger::debug(const std::string& message, const std::source_location& sourceLocation)
	{
		log(Level::debug, message, sourceLocation);
	}

	void Logger::trace(const std::string& message, const std::source_location& sourceLocation)
	{
		log(Level::trace, message, sourceLocation);
	}

	void Logger::critical(const std::wstring& message, const std::source_location& sourceLocation)
	{
		log(Level::critical, message, sourceLocation);
	}

	void Logger::error(const std::wstring& message, const std::source_location& sourceLocation)
	{
		log(Level::error, message, sourceLocation);
	}

	void Logger::warning(const std::wstring& message, const std::source_location& sourceLocation)
	{
		log(Level::warning, message, sourceLocation);
	}

	void Logger::info(const std::wstring& message, const std::source_location& sourceLocation)
	{
		log(Level::info, message, sourceLocation);
	}

	void Logger::debug(const std::wstring& message, const std::source_location& sourceLocation)
	{
		log(Level::debug, message, sourceLocation);
	}

	void Logger::trace(const std::wstring& message, const std::source_location& sourceLocation)
	{
		log(Level::trace, message, sourceLocation);
	}

	static bool shouldPrintFunctionName(Level level)
	{
		return level == Level::critical || level == Level::error ||
			level == Level::debug || level == Level::trace;
	}

	void Logger::log_Unsafe(Level level, const std::string& message, const std::source_location& sourceLocation)
	{
		const std::string functionLine = shouldPrintFunctionName(level) ?
			std::format("[{}:{}] ", sourceLocation.function_name(), sourceLocation.line()) :
			std::string();

		const std::string formatedMessage = std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {} {}",
			std::chrono::system_clock::now(),
			getLevelName_Unsafe(level),
			message,
			functionLine
		);

		const char* ansiColor = getLevelAnsiColor_Unsafe(level);
		(*_stream) << ansiColor << formatedMessage << ANSI_COLOR_DEFAULT << std::endl;
	}

	void Logger::log_Unsafe(Level level, const std::wstring& message, const std::source_location& sourceLocation)
	{
		const std::wstring functionLine = [&]() -> std::wstring {
			if (shouldPrintFunctionName(level)) {
				const char* funcName = sourceLocation.function_name();
				return std::format(L"[{}:{}] ", std::wstring(funcName, funcName + strlen(funcName)), sourceLocation.line());
			}
			return std::wstring();
			}();

		const std::wstring formatedMessage = std::format(L"[{:%Y-%m-%d %H:%M:%S}] [{}] {} {}",
			std::chrono::system_clock::now(),
			std::bit_cast<const wchar_t*>(getLevelName_Unsafe(level)),
			message,
			functionLine
		);

		const char* ansiColor = getLevelAnsiColor_Unsafe(level);
		(*_wstream) << ansiColor << formatedMessage << ANSI_COLOR_DEFAULT << std::endl;
	}

	const char* Logger::getLevelName_Unsafe(Level level) const
	{
		static const std::unordered_map<Level, const char*> levelNames
		{
			{Level::critical, "CRITICAL"},
			{Level::error, "ERROR"},
			{Level::warning, "WARNING"},
			{Level::info, "INFO"},
			{Level::debug, "DEBUG"},
			{Level::trace, "TRACE"},
		};

		auto it = levelNames.find(level);
		if (it != levelNames.end())
		{
			return it->second;
		}

		return nullptr;
	}

	const char* Logger::getLevelAnsiColor_Unsafe(Level level) const
	{
		auto it = _levelColors.find(level);
		if (it == _levelColors.end())
		{
			return ANSI_COLOR_DEFAULT;
		}

		return it->second;
	}

}