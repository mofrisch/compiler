#pragma once
#include <string>

// Enumeration for log levels
enum class LogLevel { Debug, Info, Warning, Error };

// Logging class definition
class Logger {
public:
  static Logger &instance();

  // Set the log level
  void setLogLevel(LogLevel level);

  // Log a message
  void log(LogLevel level, const std::string &message) const;

private:
  Logger() = default; // Private constructor to prevent instantiation
  Logger(const Logger &) = delete;    // Delete copy constructor
  LogLevel logLevel = LogLevel::Info; // Default log level
};
