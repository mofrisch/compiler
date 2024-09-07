#include <unistd.h>
#include <stdexcept>
#include <filesystem>

#include "Checks.hpp"


  void Checks::privileges() {
    if (geteuid() == 0) {
      throw std::runtime_error("Do not run this program with elevated privileges (root).");
    }
  }

std::string Checks::filename(const std::string &filename) {
        std::filesystem::path file_path(filename);
    
        if (!file_path.has_extension() || file_path.extension() != ".c") {
        throw std::invalid_argument("Error: The source file must have a .c extension.");
        }
    
        if (!std::filesystem::exists(file_path) || !std::filesystem::is_regular_file(file_path)) {
        throw std::invalid_argument("Error: The file does not exist or is not a regular file.");
        }
    
        return file_path.stem().string();
    }

    void Checks::arguments(int argc, char *argv[]) {
        if (argc != 3) {
        throw std::invalid_argument("Usage: <compiler> <source_file.c>");
        }
    
        if (argv[1] == nullptr || argv[2] == nullptr) {
        throw std::invalid_argument("Error: One or more arguments are null.");
        }
    }

    


    