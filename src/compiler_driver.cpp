#include <sys/wait.h>  // For waitpid
#include <unistd.h>    // For fork and execvp

#include <cstring>  // For strerror
#include <iostream>

// Function to run a command securely using fork and exec
int run_command(const char* command, const char* const argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        execvp(command, const_cast<char* const*>(argv));
        // If execvp fails, print error and exit
        std::cerr << "Error executing command: " << strerror(errno)
                  << std::endl;
        exit(1);
    } else if (pid > 0) {
        // Parent process: wait for child to finish
        int status;
        waitpid(pid, &status, 0);    // Wait for the child process to complete
        return WEXITSTATUS(status);  // Return the exit status of the child
    } else {
        // Fork failed
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
        return -1;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ${argv} <source_file.c>" << std::endl;
        return 1;
    }

    std::string source_file = argv[1];
    std::string assembly_file = "output.s";
    std::string object_file = "output.o";
    std::string executable_file = "a.out";

    // Step 1: Run your compiler to generate assembly
    const char* const compile_args[] = {"./my_compiler", source_file.c_str(),
                                        "-o", assembly_file.c_str(), nullptr};
    std::cout << "Running compiler..." << std::endl;
    if (run_command("./my_compiler", compile_args) != 0) {
        std::cerr << "Error: Compilation failed" << std::endl;
        return 1;
    }

    // Step 2: Run the assembler to generate the object file
    const char* const assemble_args[] = {"as", assembly_file.c_str(), "-o",
                                         object_file.c_str(), nullptr};
    std::cout << "Running assembler..." << std::endl;
    if (run_command("as", assemble_args) != 0) {
        std::cerr << "Error: Assembly failed" << std::endl;
        return 1;
    }

    // Step 3: Run the linker to generate the final executable
    const char* const link_args[] = {"ld", object_file.c_str(), "-o",
                                     executable_file.c_str(), nullptr};
    std::cout << "Running linker..." << std::endl;
    if (run_command("ld", link_args) != 0) {
        std::cerr << "Error: Linking failed" << std::endl;
        return 1;
    }

    std::cout << "Compilation successful! Executable: " << executable_file
              << std::endl;
    return 0;
}
