#pragma once
#include <format>
#include <string_view>

namespace Physics
{
void LogInfoImpl(std::string_view msg);
void LogDebugImpl(std::string_view msg);
void LogErrorImpl(std::string_view msg);

// Public API: compile-time-checked format string + args.
template <typename... Args>
void LogInfo(std::format_string<Args...> fmt, Args&&... args)
{
	LogInfoImpl(std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void LogDebug(std::format_string<Args...> fmt, Args&&... args)
{
	LogDebugImpl(std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args)
{
	LogErrorImpl(std::format(fmt, std::forward<Args>(args)...));
}
} // namespace Physics
