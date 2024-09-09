#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip> // For std::put_time
#include <iostream>
#include <sstream> // For std::ostringstream

// Helper function to get the current time as a string
namespace {
  std::string currentTime() {
    const auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm{};
#ifdef _MSC_VER
  localtime_s(&local_tm, &now_time); // For MSVC
#else
    localtime_r(&now_time, &local_tm); // For POSIX
#endif

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
  }
} // namespace

// Get the singleton instance
Logger &Logger::instance() {
  static Logger
      instance; // Guaranteed to be destroyed, instantiated on first use
  return instance;
}

// Set the log level
void Logger::setLogLevel(const LogLevel level) { logLevel = level; }

// Log a message
void Logger::log(LogLevel level, const std::string &message) const {
  if (static_cast<int>(level) >= static_cast<int>(logLevel)) {
    std::string levelStr;
    std::string colorCode;

    switch (level) {
      case LogLevel::Debug:
        levelStr = "DEBUG";
        colorCode = "\033[34m"; // Blue
        break;
      case LogLevel::Info:
        levelStr = "INFO";
        colorCode = "\033[32m"; // Green
        break;
      case LogLevel::Warning:
        levelStr = "WARNING";
        colorCode = "\033[33m"; // Yellow
        break;
      case LogLevel::Error:
        levelStr = "ERROR";
        colorCode = "\033[31m"; // Red
        break;
    }

    // Print the log message with timestamp, log level, and color
    std::cout << colorCode << currentTime() << " [" << levelStr << "] "
        << message << "\033[0m" << std::endl;
  }
}
