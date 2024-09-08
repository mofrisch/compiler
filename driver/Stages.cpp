#include "Stages.h"
#include "Logger.h"

#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// Ensure Logger and LogLevel are declared
extern Logger &logger;
enum class LogLevel;

Logger &logger = Logger::instance();

int Stages::run_command(const std::string &command,
                        const std::vector<std::string> &args) {

  // Log the command and arguments at DEBUG level
  std::string full_command = command;
  for (const auto &arg : args) {
    full_command += " " + arg;
  }
  logger.log(LogLevel::Debug, "Running command: " + full_command);

  // Fork the current process
  pid_t pid = fork();

  if (pid == 0) {
    // Child process: run the command
    std::vector<const char *> exec_args;
    exec_args.push_back(
        command.c_str()); // First argument is the command itself

    for (const auto &arg : args) {
      exec_args.push_back(arg.c_str()); // Add each argument to the vector
    }
    exec_args.push_back(nullptr); // Null-terminate the argument list

    // Log the command execution
    logger.log(LogLevel::Debug, "Executing command: " + full_command);

    // Replace the current process with the new one
    if (execvp(command.c_str(), const_cast<char *const *>(exec_args.data())) ==
        -1) {
      logger.log(LogLevel::Error,
                 "Error executing command: " + std::string(strerror(errno)));
      exit(EXIT_FAILURE); // Terminate child process if execvp fails
    }
  } else if (pid > 0) {
    // Parent process: wait for the child to finish
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      int exit_code = WEXITSTATUS(status);
      logger.log(LogLevel::Debug,
                 "Command exited with code: " + std::to_string(exit_code));
      return exit_code; // Return the exit code of the child process
    } else {
      logger.log(LogLevel::Error, "Command did not exit normally.");
      throw std::runtime_error("Command did not exit normally.");
    }
  } else {
    // Fork failed
    logger.log(LogLevel::Error, "Fork failed: " + std::string(strerror(errno)));
    throw std::runtime_error("Fork failed: " + std::string(strerror(errno)));
  }
  return 0;
}

void Stages::execute_stage(const std::string &stage_name, const std::string &command, const std::vector<std::string> &args) {
  logger.log(LogLevel::Info, "Running " + stage_name + " stage...");

  // Log the command that is about to run
  std::string full_command = command;
  for (const auto &arg : args) {
    full_command += " " + arg;
  }
  logger.log(LogLevel::Debug, "Executing: " + full_command);

  // Execute the command with args
  int result = run_command(command, args);

  if (result != 0) {
    throw std::runtime_error("Error during " + stage_name + " stage.");
  }
}

void Stages::preprocess(const std::string &basename) {
  execute_stage("Preprocessing", "gcc", {"-E", "-P", basename + ".c", "-o", basename + ".i"});
}

void Stages::compile(const std::string &compiler,const std::string &basename) {
  execute_stage("Compiling", compiler, {"-S", basename + ".i", "-o", basename + ".s"});
}

void Stages::assemble(const std::string &basename) {
  execute_stage("Assembling", "as", {basename + ".s", "-o", basename + ".o"});
}

void Stages::link(const std::string &basename) {
  execute_stage("Linking", "gcc", {basename + ".o", "-o", basename});
}

void Stages::simple_test() {
  logger.log(LogLevel::Debug, "Simple test...");
  run_command("echo", {"Hello, World!"});
}
