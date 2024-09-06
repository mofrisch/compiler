#include <gtest/gtest.h>

#include <fstream>
#include <string>

// Example test case
TEST(CompilerTest, BasicCompilation) {
    const char* source_file = "test1.c";
    const char* expected_output = "Expected output for test1";

    // Run the compiler
    int result = system(("./my_compiler " + std::string(source_file)).c_str());

    // Check if compilation was successful
    ASSERT_EQ(result, 0);

    // Verify the output
    std::ifstream output_file("output.txt");
    std::string actual_output((std::istreambuf_iterator<char>(output_file)),
                              std::istreambuf_iterator<char>());

    ASSERT_EQ(actual_output,
              expected_output);  // Use the expected_output variable
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}