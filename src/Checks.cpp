#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

#include "../include/Checks.h"

void Checks::privileges() {
  if (geteuid() == 0) {
    throw std::runtime_error(
      "Do not run this program with elevated privileges (root).");
  }
}

std::string Checks::filename(const std::string &filename) {
  const std::filesystem::path file_path(filename);

  if (!file_path.has_extension() || file_path.extension() != ".c") {
    throw std::invalid_argument(
      "Error: The source file must have a .c extension.");
  }

  if (!exists(file_path) ||
      !is_regular_file(file_path)) {
    throw std::invalid_argument(
      "Error: The file does not exist or is not a regular file.");
  }

  return file_path.stem().string();
}

auto Checks::arguments(const int argc, char *argv[]) -> std::vector<std::string> {
  std::string compiler_name;
  std::string file_name;
  if (argc == 3) {
    compiler_name = argv[1];
    file_name = argv[2];
  } else {
    std::cout << "compiler: ";
    std::cin >> compiler_name;
    std::cout << "file name: ";
    std::cin >> file_name;
  }
  const auto base_name = filename(file_name);
  std::vector arguments = {compiler_name, base_name};

  return arguments;
}
