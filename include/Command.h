//
// Created by mo on 9/12/24.
//

#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <stdexcept>

class CommandException : public std::runtime_error {
public:
    explicit CommandException(const std::string &message)
        : std::runtime_error(message) {
    }
};

class Command {
public:
    static int run(const std::string &name, const std::string &command, const std::vector<std::string> &args);

private:
    static std::string construct_full_command(const std::string &command, const std::vector<std::string> &args);

    static void execute_command(const std::string &command, const std::vector<std::string> &args);
};

#endif // COMMAND_H
