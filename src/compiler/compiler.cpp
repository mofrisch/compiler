#include <fstream>
#include <iostream>
#include <string>

#include "log.h"

Log &logger = Log::instance();

int main(const int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./cc <input_file> -o <output_file>" << std::endl;
        return 1;
    }

    const std::string output_file = argv[3]; // Get the output file from the command line argument

    // Open the output file for writing
    std::ofstream asm_file(output_file);
    if (!asm_file) {
        std::cerr << "Error: Could not open " << output_file << " for writing." << std::endl;
        return 1;
    }


    // Write a simple hardcoded assembly program (dummy output)
    asm_file << "    .section .text\n"; // Ensure we are in the text section
    asm_file << "    .global _start\n"; // Make _start global so linker can find it
    asm_file << "_start:\n";
    asm_file << "    mov $60, %rax        # system call for exit (64-bit)\n";
    asm_file << "    mov $42, %rdi        # exit code 42\n";
    asm_file << "    syscall             # invoke system call\n";

    asm_file.close();
    logger.log(LogLevel::Info, "Dummy compilation successful! Assembly output written to " + output_file);

    return 0;
}
