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
    int run(const std::string &name, const std::string &command, const std::vector<std::string> &args);

private:
    std::string construct_full_command(const std::string &command, const std::vector<std::string> &args) const;

    void execute_command(const std::string &command, const std::vector<std::string> &args) const;
};

#endif // COMMAND_H
