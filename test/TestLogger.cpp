#include "Logger.h"
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <regex>

// Helper class to redirect std::cout for testing
class CoutRedirect {
public:
    CoutRedirect(std::ostream &new_stream)
        : old(std::cout.rdbuf(new_stream.rdbuf())) {
    }

    ~CoutRedirect() { std::cout.rdbuf(old); }

private:
    std::streambuf *old;
};

// Test fixture for Logger
class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect cout to a stringstream to capture the logger output
        output.str("");
        output.clear();
        redirect = std::make_unique<CoutRedirect>(output);
    }

    void TearDown() override {
        redirect.reset(); // Stop redirecting cout
    }

    std::stringstream output;
    std::unique_ptr<CoutRedirect> redirect;
};

// Test singleton instance retrieval
TEST_F(LoggerTest, SingletonInstance) {
    Logger &instance1 = Logger::instance();
    Logger &instance2 = Logger::instance();
    EXPECT_EQ(&instance1, &instance2); // Check if both instances are the same
}

// Test that the log level can be set and messages are logged at or above the level
TEST_F(LoggerTest, SetLogLevelAndLogMessages) {
    Logger &logger = Logger::instance();

    // Set log level to Debug and log a Debug message
    logger.setLogLevel(LogLevel::Debug);
    logger.log(LogLevel::Debug, "Debug message");
    EXPECT_NE(output.str().find("DEBUG"), std::string::npos); // Ensure "DEBUG" is in output

    // Clear the stream and set log level to Info
    output.str("");
    output.clear();
    logger.setLogLevel(LogLevel::Info);
    logger.log(LogLevel::Debug, "Debug message");
    logger.log(LogLevel::Info, "Info message");
    EXPECT_EQ(output.str().find("Debug message"), std::string::npos); // Debug should not appear
    EXPECT_NE(output.str().find("Info message"), std::string::npos); // Info should appear
}

// Test log level filtering
TEST_F(LoggerTest, LogLevelFiltering) {
    Logger &logger = Logger::instance();

    // Set log level to Warning
    logger.setLogLevel(LogLevel::Warning);
    logger.log(LogLevel::Debug, "Debug message");
    logger.log(LogLevel::Info, "Info message");
    logger.log(LogLevel::Warning, "Warning message");

    // Only the warning message should appear in the output
    EXPECT_EQ(output.str().find("Debug message"), std::string::npos);
    EXPECT_EQ(output.str().find("Info message"), std::string::npos);
    EXPECT_NE(output.str().find("Warning message"), std::string::npos);
}

// Test log output formatting (timestamp and log level)
TEST_F(LoggerTest, LogOutputFormat) {
    Logger &logger = Logger::instance();
    logger.setLogLevel(LogLevel::Info);
    logger.log(LogLevel::Info, "Formatted message");

    std::string log_output = output.str();
    EXPECT_NE(log_output.find("INFO"), std::string::npos); // Ensure "INFO" appears in output

    // Check if timestamp format (YYYY-MM-DD HH:MM:SS) is present in the output
    std::regex time_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    EXPECT_TRUE(std::regex_search(log_output, time_regex)) << "Timestamp not found in log output";
}

// Test color codes for different log levels
TEST_F(LoggerTest, LogColorCodes) {
    Logger &logger = Logger::instance();

    // Test Debug (Blue)
    logger.setLogLevel(LogLevel::Debug);
    logger.log(LogLevel::Debug, "Debug message");
    EXPECT_NE(output.str().find("\033[34m"), std::string::npos); // Blue color code

    // Test Info (Green)
    output.str("");
    output.clear();
    logger.log(LogLevel::Info, "Info message");
    EXPECT_NE(output.str().find("\033[32m"), std::string::npos); // Green color code

    // Test Warning (Yellow)
    output.str("");
    output.clear();
    logger.log(LogLevel::Warning, "Warning message");
    EXPECT_NE(output.str().find("\033[33m"), std::string::npos); // Yellow color code

    // Test Error (Red)
    output.str("");
    output.clear();
    logger.log(LogLevel::Error, "Error message");
    EXPECT_NE(output.str().find("\033[31m"), std::string::npos); // Red color code
}

// Test that color is reset after log message
TEST_F(LoggerTest, ResetColorAfterLog) {
    Logger &logger = Logger::instance();
    logger.setLogLevel(LogLevel::Info);
    logger.log(LogLevel::Info, "Info message");

    // Check if color reset code \033[0m is at the end
    EXPECT_NE(output.str().find("\033[0m"), std::string::npos); // Reset color code
}

// Test logging below the current log level
TEST_F(LoggerTest, LoggingBelowCurrentLevel) {
    Logger &logger = Logger::instance();

    // Set log level to Error
    logger.setLogLevel(LogLevel::Error);
    logger.log(LogLevel::Debug, "Debug message");
    logger.log(LogLevel::Info, "Info message");
    logger.log(LogLevel::Warning, "Warning message");
    logger.log(LogLevel::Error, "Error message");

    // Only the Error message should appear
    EXPECT_EQ(output.str().find("Debug message"), std::string::npos);
    EXPECT_EQ(output.str().find("Info message"), std::string::npos);
    EXPECT_EQ(output.str().find("Warning message"), std::string::npos);
    EXPECT_NE(output.str().find("Error message"), std::string::npos);
}
