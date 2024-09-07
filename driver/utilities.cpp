#include <cstring>
#include <string>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <filesystem>  // C++17 and later
#include <stdexcept>

bool is_safe_filename(const std::string& filename) {
    return std::all_of(filename.begin(), filename.end(), [](char c) {
        return std::isalnum(c) || c == '_' || c == '-' || c == '.';
    });
}

void check_privileges() {
    if (geteuid() == 0) {
        std::cerr << "Do not run this program with elevated privileges (root)." << std::endl;
        exit(1);
    }
}

bool validate_source_file(const std::string& source_file) {
    if (source_file.size() < 3 || source_file.substr(source_file.size() - 2) != ".c") {
        std::cerr << "Error: The source file must have a .c extension." << std::endl;
        return false;
    }
    return true;
}



std::string change_extension(const std::string& filename, const std::string& new_ext) {
    size_t dot_pos = filename.rfind('.');
    if (dot_pos == std::string::npos) {
        return filename + new_ext;
    } else {
        return filename.substr(0, dot_pos) + new_ext;
    }
}

std::string validate_and_get_basename(const std::string& filename) {
    // Use std::filesystem to handle path operations
    std::filesystem::path file_path(filename);

    // Check if file ends with ".c"
    if (!file_path.has_extension() || file_path.extension() != ".c") {
        throw std::invalid_argument("Error: The source file must have a .c extension.");
    }

    // Check if the file exists and is a regular file
    if (!std::filesystem::exists(file_path) || !std::filesystem::is_regular_file(file_path)) {
        throw std::invalid_argument("Error: The file does not exist or is not a regular file.");
    }

    // Get the base name without the .c extension
    return file_path.stem().string();  // stem() returns the filename without the extension
}

void check_arguments(int argc, char *argv[]) {
    if (argc != 3) {
        throw std::invalid_argument("Usage: <compiler> <source_file.c>");
    }

    // Check if argv[1] or argv[2] are nullptr
    if (argv[1] == nullptr || argv[2] == nullptr) {
        throw std::invalid_argument("Error: One or more arguments are null.");
    }
}

int run_command(const std::string& command, const std::vector<std::string>& args) {
    pid_t pid = fork();
    if (pid == 0) {
        std::vector<const char*> exec_args;
        exec_args.push_back(command.c_str());
        for (const auto& arg : args) {
            exec_args.push_back(arg.c_str());
        }
        exec_args.push_back(nullptr);

        execvp(command.c_str(), const_cast<char* const*>(exec_args.data()));
        std::cerr << "Error executing command: " << strerror(errno) << std::endl;
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    } else {
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
        return -1;
    }
}


void run_stage(const std::string& stage_name, const std::string& command, const std::string & source_file,const std::string & output_file) {
    std::cout << "Running " << stage_name << "..." << std::endl;
    auto command_to_execute = command + source_file + " -o " + output_file;
    system(command_to_execute.c_str());
}

void preprocess(const std::string& basename) {
    run_stage("Preprocessing...", "gcc -P -E ", basename + ".c", basename + ".i");
}

void compile(const std::string& basename) {
    run_stage("Compiling...", "gcc -S ", basename + ".i", basename + ".s");
}

void assemble(const std::string& basename) {
    run_stage("Assembling...", "as ", basename + ".s", basename + ".o");
}

void link(const std::string& basename) {
    run_stage("Linking...", "gcc ", basename + ".o", basename);
}