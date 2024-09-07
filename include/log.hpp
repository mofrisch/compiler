#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <source_location>

// Enum class for log levels
enum class LogLevel {
    Error,
    Warning,
    Info,
    Debug
};

// Convert log level to string with fixed width and colors
std::string log_level_to_string(LogLevel level);

// Get current time as string (for timestamping logs)
std::string current_time();

// Log function with optional source location and color
void log(LogLevel level, const std::string& message,
         const std::source_location location = std::source_location::current());

#endif  // LOG_HPP
