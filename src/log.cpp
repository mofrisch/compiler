#include "log.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

// Convert log level to string with fixed width and colors
std::string log_level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::Error:   return "\033[31mERROR  \033[0m";   // Red for Error
        case LogLevel::Warning: return "\033[33mWARNING\033[0m";   // Yellow for Warning
        case LogLevel::Info:    return "\033[32mINFO   \033[0m";   // Green for Info
        case LogLevel::Debug:   return "\033[34mDEBUG  \033[0m";   // Blue for Debug
        default:                return "UNKNOWN";
    }
}

// Get current time as string (optional: for timestamping logs)
std::string current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm* ptm = std::localtime(&time_now);

    std::stringstream ss;
    ss << std::put_time(ptm, "%Y-%m-%d %H:%M:%S");  // Format: Year-Month-Day Hour:Minute:Second
    return ss.str();
}

// Log function with optional source location and color
void log(LogLevel level, const std::string& message,
         const std::source_location location) {
#ifndef NDEBUG  // Only log Info and Debug messages in Debug builds
    if (level == LogLevel::Debug || level == LogLevel::Info) {
        std::cout << "[" << current_time() << "] "
                  << "[" << log_level_to_string(level) << "] "
                  << location.file_name() << ":" << location.line()
                  << " (" << location.function_name() << "): " << message << std::endl;
    }
#endif
    if (level == LogLevel::Error || level == LogLevel::Warning) {
        std::cout << "[" << current_time() << "] "
                  << "[" << log_level_to_string(level) << "] "
                  << location.file_name() << ":" << location.line()
                  << " (" << location.function_name() << "): " << message << std::endl;
    }
}
