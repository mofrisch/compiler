#include "log.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

// Redirect cout to stringstream for testing output
class LogTest : public ::testing::Test {
protected:
    std::streambuf* cout_buf;  // Original cout buffer
    std::stringstream output;  // Capture stream

    void SetUp() override {
        cout_buf = std::cout.rdbuf();  // Save original buffer
        std::cout.rdbuf(output.rdbuf());  // Redirect cout to stringstream
    }

    void TearDown() override {
        std::cout.rdbuf(cout_buf);  // Restore original buffer
    }
};

// Test Error log
TEST_F(LogTest, LogErrorTest) {
    log(LogLevel::Error, "Test error message");
    EXPECT_NE(output.str().find("ERROR"), std::string::npos);
    EXPECT_NE(output.str().find("Test error message"), std::string::npos);
}

// Test Warning log
TEST_F(LogTest, LogWarningTest) {
    log(LogLevel::Warning, "Test warning message");
    EXPECT_NE(output.str().find("WARNING"), std::string::npos);
    EXPECT_NE(output.str().find("Test warning message"), std::string::npos);
}

// Test Info log (only valid in Debug mode)
TEST_F(LogTest, LogInfoTest) {
#ifndef NDEBUG  // Only valid in Debug mode
    log(LogLevel::Info, "Test info message");
    EXPECT_NE(output.str().find("INFO"), std::string::npos);
    EXPECT_NE(output.str().find("Test info message"), std::string::npos);
#endif
}

// Test Debug log (only valid in Debug mode)
TEST_F(LogTest, LogDebugTest) {
#ifndef NDEBUG  // Only valid in Debug mode
    log(LogLevel::Debug, "Test debug message");
    EXPECT_NE(output.str().find("DEBUG"), std::string::npos);
    EXPECT_NE(output.str().find("Test debug message"), std::string::npos);
#endif
}
