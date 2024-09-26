//
// Created by mo on 9/9/24.
//

#include "stages.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace fs = std::filesystem;

// Helper function to clean up generated files
void cleanup_files(const std::string &base_name) {
    fs::remove(base_name + ".i");
    fs::remove(base_name + ".s");
    fs::remove(base_name + ".o");
    fs::remove(base_name); // The final executable
}

// Test fixture for Stages
class ChecksTest : public testing::Test {
protected:
    std::string test_file = "tests.c";

    void SetUp() override {
        // Create a simple C file for testing
        std::ofstream file(test_file);
        file << "int main() { return 0; }" << std::endl;
        file.close();
    }

    void TearDown() override {
        // Clean up the tests file and generated files
        fs::remove(test_file);
        cleanup_files("tests");
    }
};


// Test successful preprocessing stage
TEST_F(ChecksTest, PreprocessingSuccess) {
    EXPECT_NO_THROW(Stages::preprocess("tests"));
    EXPECT_TRUE(fs::exists("tests.i")); // Ensure the .i file is created
}

// Test preprocessing stage failure (invalid file)
TEST_F(ChecksTest, PreprocessingFailure) { EXPECT_THROW(Stages::preprocess("invalid"), std::runtime_error); }


// Test logging for each stage (using a custom log capturing mechanism)
class LoggingStagesTest : public testing::Test {
protected:
    std::stringstream log_output;
    std::streambuf *old_cout_buf{};

    void SetUp() override {
        old_cout_buf = std::cout.rdbuf(); // Save old buffer
        std::cout.rdbuf(log_output.rdbuf()); // Redirect cout to log_output
    }

    void TearDown() override {
        std::cout.rdbuf(old_cout_buf); // Restore old buffer
    }
};

// Test logging during preprocessing stage
TEST_F(LoggingStagesTest, LoggingPreprocess) {
    try {
        EXPECT_THROW(Stages::preprocess("tests"), std::runtime_error);
    } catch (std::exception &) {
        std::string log = log_output.str();
        EXPECT_EQ(log.find("Running Preprocessing stage..."), std::string::npos);
        EXPECT_EQ(log.find("Running command: gcc -E -P tests.c -o tests.i"), std::string::npos);
    }
}

// Test logging during compilation stage
TEST_F(LoggingStagesTest, LoggingCompile) {
    try {
        EXPECT_THROW(Stages::compile("tests"), std::runtime_error);
    } catch (std::exception &) {
        std::string log = log_output.str();
        EXPECT_NE(log.find("Running Compilation stage..."), std::string::npos);
        EXPECT_NE(log.find("Running command: gcc -S tests.i -o tests.s"), std::string::npos);
    }
}


// Test logging during assembly stage
TEST_F(LoggingStagesTest, LoggingAssemble) {
    try {
        EXPECT_THROW(Stages::assemble("tests"), std::runtime_error);
    } catch (std::exception &) {
        std::string log = log_output.str();
        EXPECT_NE(log.find("Running Assembly stage..."), std::string::npos);
        EXPECT_NE(log.find("Running command: gcc -c tests.s -o tests.o"), std::string::npos);
    }
}

// Test logging during linking stage
TEST_F(LoggingStagesTest, LoggingLink) {
    try {
        EXPECT_THROW(Stages::link("tests"), std::runtime_error);
    } catch (std::exception &) {
        std::string log = log_output.str();
        EXPECT_NE(log.find("Running Linking stage..."), std::string::npos);
        EXPECT_NE(log.find("Running command: gcc tests.o -o tests"), std::string::npos);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
