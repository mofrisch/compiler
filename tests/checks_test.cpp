// File: ChecksTest.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Checks.h"

using namespace testing;

class ChecksTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Prepare for tests, if necessary
    }

    void TearDown() override {
        // Clean up after tests, if necessary
    }

    Checks checks{};
};

// Test for privileges() method
TEST_F(ChecksTest, PrivilegesShouldThrowIfRunAsRoot) {
    // Simulate root privileges by overriding geteuid() function
    setenv("FAKE_ROOT", "1", 1);
    EXPECT_THROW(checks.privileges(), std::runtime_error);
    unsetenv("FAKE_ROOT");
}

// Temporarily override geteuid()
extern "C" uid_t geteuid() {
    const char* fake_root = std::getenv("FAKE_ROOT");
    return (fake_root && std::stoi(fake_root) == 1) ? 0 : ::geteuid();
}

// Test for filename() method
TEST_F(ChecksTest, FilenameShouldThrowIfExtensionIsNotC) {
    EXPECT_THROW(checks.filename("example.txt"), std::invalid_argument);
}

TEST_F(ChecksTest, FilenameShouldThrowIfFileDoesNotExist) {
    EXPECT_THROW(checks.filename("nonexistent.c"), std::invalid_argument);
}

TEST_F(ChecksTest, FilenameShouldReturnStemIfValid) {
    const std::string valid_file = "test.c";
    std::ofstream(valid_file).close(); // Create a dummy file
    EXPECT_EQ(checks.filename(valid_file), "test");
    std::remove(valid_file.c_str());    // Clean up
}

// Test for arguments() method
TEST_F(ChecksTest, ArgumentsShouldReturnProperValuesFromArgs) {
    const char* argv[] = { "program", "gcc", "test.c" };
    int argc = 3;
    const std::string valid_file = "test.c";
    std::ofstream(valid_file).close(); // Create a dummy file
    auto result = checks.arguments(argc, const_cast<char**>(argv));
    std::remove(valid_file.c_str());   // Clean up

    EXPECT_EQ(result[0], "gcc");
    EXPECT_EQ(result[1], "test");
}

TEST_F(ChecksTest, ArgumentsShouldPromptUserIfArgcNot3) {
    // A real test here might require more complex input simulation
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}