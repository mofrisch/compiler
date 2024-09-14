
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <regex>

#include "Log.h"

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

// Test fixture for Log
class LogTest : public ::testing::Test {
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

    void redirect_on(bool redirect) {
        if (redirect) {
            output.str("");
            output.clear();
            this->redirect = std::make_unique<CoutRedirect>(output);
        } else {
            this->redirect.reset();
        }
    }

    std::stringstream output;
    std::unique_ptr<CoutRedirect> redirect;
};

// Test singleton instance retrieval
TEST_F(LogTest, SingletonInstance) {
    Log &instance1 = Log::instance();
    Log &instance2 = Log::instance();
    EXPECT_EQ(&instance1, &instance2); // Check if both instances are the same
}

// Test that the log level can be set and messages are logged at or above the level
TEST_F(LogTest, SetLogLevelAndLogMessages) {
    Log &logger = Log::instance();

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
TEST_F(LogTest, LogLevelFiltering) {
    Log &logger = Log::instance();

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
TEST_F(LogTest, LogOutputFormat) {
    Log &logger = Log::instance();
    logger.setLogLevel(LogLevel::Info);
    logger.log(LogLevel::Info, "Formatted message");

    std::string log_output = output.str();
    EXPECT_NE(log_output.find("INFO"), std::string::npos); // Ensure "INFO" appears in output

    // Check if timestamp format (YYYY-MM-DD HH:MM:SS) is present in the output
    std::regex time_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    EXPECT_TRUE(std::regex_search(log_output, time_regex)) << "Timestamp not found in log output";
}

// Test color codes for different log levels
TEST_F(LogTest, LogColorCodes) {
    Log &logger = Log::instance();

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
TEST_F(LogTest, ResetColorAfterLog) {
    Log &logger = Log::instance();
    logger.setLogLevel(LogLevel::Info);
    logger.log(LogLevel::Info, "Info message");

    // Check if color reset code \033[0m is at the end
    EXPECT_NE(output.str().find("\033[0m"), std::string::npos); // Reset color code
}

TEST_F(LogTest, WriteColoredLines) {
    Log &logger = Log::instance();
    redirect_on(false);

    // Set log level to Debug
    logger.setLogLevel(LogLevel::Debug);

    // Test Debug (Blue)
    logger.log(LogLevel::Debug, "Debug message");

    logger.log(LogLevel::Info, "Info message");

    logger.log(LogLevel::Warning, "Warning message");

    logger.log(LogLevel::Error, "Error message");

    redirect_on(true);
}

// Test logging below the current log level
TEST_F(LogTest, LoggingBelowCurrentLevel) {
    Log &logger = Log::instance();

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
