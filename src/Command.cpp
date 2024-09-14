//
// Created by mo on 9/12/24.
//

#include "Command.h"
#include "Log.h"

#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

Log &logger = Log::instance();

std::string Command::construct_full_command(const std::string &command, const std::vector<std::string> &args) {
    std::string full_command = command;
    for (const auto &arg: args) {
        full_command += " " + arg;
    }
    return full_command;
}

void Command::execute_command(const std::string &command, const std::vector<std::string> &args) {
    std::vector<const char *> exec_args;
    exec_args.reserve(args.size() + 2);
    exec_args.push_back(command.c_str());
    for (const auto &arg: args) {
        exec_args.push_back(arg.c_str());
    }
    exec_args.push_back(nullptr);

    if (execvp(command.c_str(), const_cast<char *const *>(exec_args.data())) == -1) {
        throw std::runtime_error("Error executing command: " + std::string(strerror(errno)));
    }
}

// TODO: Refactor this function to return a CommandStatus enum
//  instead of throwing exceptions and to make it a bit cleaner
int Command::run(const std::string &name, const std::string &command, const std::vector<std::string> &args) {
    try {
        logger.log(LogLevel::Info, "Running " + name + " stage.");
        const std::string full_command = construct_full_command(command, args);
        logger.log(LogLevel::Debug, "Running command: " + full_command);

        const pid_t pid = fork();
        if (pid == 0) {
            logger.log(LogLevel::Debug, "Executing command: " + full_command);
            execute_command(command, args);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                const int exit_code = WEXITSTATUS(status);
                logger.log(LogLevel::Debug, "Command exited with code: " + std::to_string(exit_code));
                if (exit_code != 0) {
                    throw std::runtime_error("Command failed with exit code: " + std::to_string(exit_code));
                }
                return exit_code;
            } else if (WIFSIGNALED(status)) {
                throw std::runtime_error("Command terminated by signal: " + std::to_string(WTERMSIG(status)));
            } else {
                throw std::runtime_error("Command did not exit normally.");
            }
        } else {
            throw std::runtime_error("Fork failed: " + std::string(strerror(errno)));
        }
    } catch (const std::exception &e) {
        logger.log(LogLevel::Error, e.what());
        throw CommandException(e.what());
    }
    return -1; // Ideally, code should never reach here
}
