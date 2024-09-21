
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

#include "checks.h"

// Test fixture for Stages
class ChecksTest : public testing::Test {
protected:
    std::string test_file = "test.c";

    void SetUp() override {}

    void TearDown() override {}
};

// Test for privileges function
TEST(ChecksTest, Privileges_NoRoot) {
    Checks checks;
    EXPECT_NO_THROW(checks.privileges());
}

// Test for filename function with valid .c file
TEST(ChecksTest, Filename_ValidFile) {
    std::filesystem::path currentPath = std::filesystem::current_path();

    std::cout << "Current working directory: " << currentPath << std::endl;
    Checks checks;
    std::string valid_filename = "test.c";
    std::ofstream file(valid_filename);
    EXPECT_NO_THROW(checks.filename(valid_filename));
}

// Test for filename function with invalid extension
TEST(ChecksTest, Filename_InvalidExtension) {
    Checks checks;
    const std::string invalid_filename = "test.txt";
    EXPECT_THROW(checks.filename(invalid_filename), std::invalid_argument);
}

// Test for filename function with non-existing file
TEST(ChecksTest, Filename_NonExistingFile) {
    Checks checks;
    const std::string non_existing_filename = "nonexistent.c";
    EXPECT_THROW(checks.filename(non_existing_filename), std::invalid_argument);
}

/* TODO: Fix this test
 // Test for arguments function with valid input
TEST(ChecksTest, Arguments_ValidInput) {
    Checks checks;
    const char *argv[] = {"compiler", "test.c"};
    int argc = 2;
    EXPECT_NO_THROW(checks.arguments(argc, const_cast<char**>(argv)));
}

// Test for arguments function with invalid input (no .c extension)
TEST(ChecksTest, Arguments_InvalidFileExtension) {
    Checks checks;
    const char *argv[] = {"compiler", "test.txt"};
    int argc = 2;
    EXPECT_THROW(checks.arguments(argc, const_cast<char**>(argv)), std::invalid_argument);
}

// Test for arguments function with fewer arguments
TEST(ChecksTest, Arguments_InsufficientArguments) {
    Checks checks;
    const char *argv[] = {"compiler"};
    int argc = 1;
    EXPECT_THROW(checks.arguments(argc, const_cast<char**>(argv)), std::runtime_error);
}
*/


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
