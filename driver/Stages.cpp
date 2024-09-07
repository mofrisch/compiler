#include "Stages.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void Stages::preprocess(const std::string &basename) {
  run_stage("Preprocessing", "gcc -P -E", basename + ".c", basename + ".i");
}

void Stages::compile(const std::string &basename) {
  run_stage("Compiling...", "gcc -S ", basename + ".i", basename + ".s");
}

void Stages::assemble(const std::string &basename) {
  run_stage("Assembling...", "as ", basename + ".s", basename + ".o");
}

void Stages::link(const std::string &basename) {
  run_stage("Linking...", "gcc ", basename + ".o", basename);
}

// TODO CommandStatus
int Stages::run_command(const std::string &command,
                                  const std::vector<std::string> &args) {
  pid_t pid = fork();
  if (pid == 0) {
    std::vector<const char *> exec_args;
    exec_args.push_back(command.c_str());
    for (const auto &arg : args) {
      exec_args.push_back(arg.c_str());
    }
    exec_args.push_back(nullptr);

    execvp(command.c_str(), const_cast<char *const *>(exec_args.data()));
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

void Stages::run_stage(const std::string &stage_name,
                       const std::string &command,
                       const std::string &source_file,
                       const std::string &output_file) {
  std::cout << "Running " << stage_name << "..." << std::endl;
  std::vector<std::string> args = {source_file, "-o", output_file};
  int result = run_command(command, args);
  if (result != 0) {
    throw std::runtime_error("Error during " + stage_name + " stage.");
  }
}