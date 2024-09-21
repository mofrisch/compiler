#include "command.h"
#include <gtest/gtest.h>

// Fixture for setting up necessary objects
class CommandTest : public ::testing::Test, public Command {
protected:
    void SetUp() override {
        // Initialize code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test case for the run method
TEST_F(CommandTest, Run_ValidCommand_ReturnsSuccess) {
    std::vector<std::string> args = {"arg1", "arg2"};
    int result = Command::run("TestCommand", "echo", args);
    EXPECT_EQ(result, 0); // Assuming 0 indicates success
}

// Test case for the run method throwing CommandException
TEST_F(CommandTest, ExecuteCommand_InvalidCommand_ThrowsException) {
    std::vector<std::string> args = {"invalid_arg"};
    EXPECT_THROW(Command::execute_command("invalid_command", args),
                 CommandException); // Assuming invalid commands throw CommandException
}

// Test case for the construct_full_command method
TEST_F(CommandTest, ConstructFullCommand_ValidArgs_ReturnsExpectedString) {
    std::vector<std::string> args = {"arg1", "arg2"};
    std::string full_command = Command::construct_full_command("echo", args);
    EXPECT_EQ(full_command, "echo arg1 arg2");
}

// Add more test cases for execute_command. As it's private, you may need to test it indirectly through run.

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
