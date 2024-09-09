//
// Created by mo on 9/9/24.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "Stages.h"

namespace fs = std::filesystem;

// Helper function to clean up generated files
void cleanup_files(const std::string &base_name) {
    fs::remove(base_name + ".i");
    fs::remove(base_name + ".s");
    fs::remove(base_name + ".o");
    fs::remove(base_name); // The final executable
}

// Test fixture for Stages
class StagesTest : public ::testing::Test, public Stages {
protected:
    std::string test_file = "test.c";

    void SetUp() override {
        // Create a simple C file for testing
        std::ofstream file(test_file);
        file << "int main() { return 0; }" << std::endl;
        file.close();
    }

    void TearDown() override {
        // Clean up the test file and generated files
        fs::remove(test_file);
        cleanup_files("test");
    }
};

// Test command execution success
TEST_F(StagesTest, RunCommandSuccess) {
    int result = Stages::run_command("echo", {"Hello, World!"});
    EXPECT_EQ(result, 0); // Command should succeed
}

// Test command execution failure
TEST_F(StagesTest, RunCommandFailure) {
    int result = Stages::run_command("nonexistentcommand", {});
    EXPECT_NE(result, 0); // Command should fail
}

// Test successful preprocessing stage
TEST_F(StagesTest, PreprocessingSuccess) {
    EXPECT_NO_THROW(Stages::preprocess("test"));
    EXPECT_TRUE(fs::exists("test.i")); // Ensure the .i file is created
}

// Test preprocessing stage failure (invalid file)
TEST_F(StagesTest, PreprocessingFailure) {
    EXPECT_THROW(Stages::preprocess("invalid"), std::runtime_error);
}

// Test successful compilation stage
TEST_F(StagesTest, CompilationSuccess) {
    Stages::preprocess("test");
    EXPECT_NO_THROW(Stages::compile("gcc", "test"));
    EXPECT_TRUE(fs::exists("test.s")); // Ensure the .s file is created
}

// Test compilation stage failure (invalid file)
TEST_F(StagesTest, CompilationFailure) {
    EXPECT_THROW(Stages::compile("gcc", "invalid"), std::runtime_error);
}

// Test successful assembly stage
TEST_F(StagesTest, AssemblySuccess) {
    Stages::preprocess("test");
    Stages::compile("gcc", "test");
    EXPECT_NO_THROW(Stages::assemble("test"));
    EXPECT_TRUE(fs::exists("test.o")); // Ensure the .o file is created
}

// Test assembly stage failure (invalid file)
TEST_F(StagesTest, AssemblyFailure) {
    EXPECT_THROW(Stages::assemble("invalid"), std::runtime_error);
}

// Test successful linking stage
TEST_F(StagesTest, LinkingSuccess) {
    Stages::preprocess("test");
    Stages::compile("gcc", "test");
    Stages::assemble("test");
    EXPECT_NO_THROW(Stages::link("test"));
    EXPECT_TRUE(fs::exists("test")); // Ensure the executable is created
}

// Test linking stage failure (invalid file)
TEST_F(StagesTest, LinkingFailure) {
    EXPECT_THROW(Stages::link("invalid"), std::runtime_error);
}

// Test the full workflow (preprocess, compile, assemble, link)
TEST_F(StagesTest, FullWorkflowSuccess) {
    EXPECT_NO_THROW({
        Stages::preprocess("test");
        Stages::compile("gcc", "test");
        Stages::assemble("test");
        Stages::link("test");
        });
    EXPECT_TRUE(fs::exists("test.i")); // Check that all intermediate files exist
    EXPECT_TRUE(fs::exists("test.s"));
    EXPECT_TRUE(fs::exists("test.o"));
    EXPECT_TRUE(fs::exists("test")); // Check that the final executable exists
}

// Test the full workflow with a failure in the linking stage
TEST_F(StagesTest, FullWorkflowFailureLink) {
    EXPECT_NO_THROW({
        Stages::preprocess("test");
        Stages::compile("gcc", "test");
        Stages::assemble("test");
        });
    EXPECT_THROW(Stages::link("invalid"), std::runtime_error); // Should fail during linking
}

// Test logging for each stage (using a custom log capturing mechanism)
class LoggingStagesTest : public ::testing::Test {
protected:
    std::stringstream log_output;
    std::streambuf *old_cout_buf;

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
        EXPECT_THROW(Stages::preprocess("test"), std::runtime_error);
    } catch (std::exception &e) {
        std::string log = log_output.str();
        EXPECT_EQ(log.find("Running Preprocessing stage..."), std::string::npos);
        EXPECT_EQ(log.find("Running command: gcc -E -P test.c -o test.i"), std::string::npos);
    }
}

// Test logging during compilation stage
TEST_F(LoggingStagesTest, LoggingCompile) {
    Stages::preprocess("test");
    Stages::compile("gcc", "test");
    std::string log = log_output.str();
    EXPECT_NE(log.find("Running Compiling stage..."), std::string::npos);
    EXPECT_NE(log.find("Running command: gcc -S test.i -o test.s"), std::string::npos);
}

// Test logging during assembly stage
TEST_F(LoggingStagesTest, LoggingAssemble) {
    Stages::preprocess("test");
    Stages::compile("gcc", "test");
    Stages::assemble("test");
    std::string log = log_output.str();
    EXPECT_NE(log.find("Running Assembling stage..."), std::string::npos);
    EXPECT_NE(log.find("Running command: as test.s -o test.o"), std::string::npos);
}

// Test logging during linking stage
TEST_F(LoggingStagesTest, LoggingLink) {
    Stages::preprocess("test");
    Stages::compile("gcc", "test");
    Stages::assemble("test");
    Stages::link("test");
    std::string log = log_output.str();
    EXPECT_NE(log.find("Running Linking stage..."), std::string::npos);
    EXPECT_NE(log.find("Running command: gcc test.o -o test"), std::string::npos);
}


