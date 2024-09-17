


#include <chrono>
#include <iostream>
#include <string>


#include "Log.h"

std::string currentTime() {
    using namespace std::chrono;

    // Get the current time_point
    const auto now = system_clock::now();

    // Format the time directly from the time_point
    return std::format("{:%Y-%m-%d %H:%M:%S}", floor<seconds>(now));
}

class LogLevelHelper {
public:
    // Constructor to initialize the log levels with corresponding color and string
    LogLevelHelper() {
        logLevels = {
                {"[DEBUG]", "\033[34m"}, // Blue
                {"[INFO]", "\033[32m"}, // Green
                {"[WARNING]", "\033[33m"}, // Yellow
                {"[ERROR]", "\033[31m"} // Red
        };
    }

    // Method to get log level string and color based on log level enum
    [[nodiscard]] std::pair<std::string, std::string> getLevelInfo(LogLevel level) const {
        return logLevels[static_cast<int>(level)];
    }

    // Reset color constant
    static const std::string RESET;

private:
    std::vector<std::pair<std::string, std::string>> logLevels; // Pair of log level string and color code
};

// Initialize the RESET color constant
const std::string LogLevelHelper::RESET = "\033[0m";
const LogLevelHelper logLevelHelper;

Log &Log::instance() {
    static Log instance; // Guaranteed to be destroyed, instantiated on first use
    return instance;
}

void Log::log(const LogLevel level, const std::string &message) const {
    if (level >= _logLevel) {
        auto [levelStr, colorCode] = logLevelHelper.getLevelInfo(level);
        std::cout << colorCode << currentTime() << " " << levelStr << " " << message << LogLevelHelper::RESET
                  << std::endl;
    }
}
