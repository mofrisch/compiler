//
// Created by mo on 9/13/24.
//

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>



#ifndef LOG_H
#define LOG_H

// Enumeration for log levels
enum class LogLevel { Debug, Info, Warning, Error };

// Logging class definition
class Log {
public:
    static Log &instance();

    // Set the log level
    void setLogLevel(LogLevel level);

    // Log a message
    void log(LogLevel level, const std::string &message) const;

private:
    Log() = default; // Private constructor to prevent instantiation
    Log(const Log &) = delete; // Delete copy constructor
    LogLevel logLevel = LogLevel::Info; // Default log level
};

#endif //LOG_H
