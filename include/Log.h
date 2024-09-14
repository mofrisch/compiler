//
// Created by mo on 9/13/24.
//
#pragma once

#include <string>

// Enumeration for log levels
enum class LogLevel { Debug, Info, Warning, Error };

// Logging class definition
class Log {
public:
    Log(const Log &) = delete; // Delete copy constructor
    static Log &instance();

    void setLogLevel(const LogLevel level) { _logLevel = level; }
    void showTime(const bool enable) { _displayTime = enable; }
    void displayLevel(const bool enable) { _displayLevel = enable; }

    // Log a message
    void log(LogLevel level, const std::string &message) const;

private:
    Log() = default; // Private constructor to prevent instantiation

    LogLevel _logLevel = LogLevel::Info; // Default log level
    bool _displayTime = true; // Display time in log messages
    bool _displayLevel = true; // Display log level in log messages
};


