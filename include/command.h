//
// Created by mo on 9/12/24.
//

#pragma once


#include <stdexcept>
#include <string>
#include <vector>

class CommandException : public std::runtime_error {
public:
    explicit CommandException(const std::string &message) : std::runtime_error(message) {}
};

class Command {
public:
    static int run(const std::string &name, const std::string &command, const std::vector<std::string> &args);
    friend class CommandTest;

protected:
    static std::string construct_full_command(const std::string &command, const std::vector<std::string> &args);

    static void execute_command(const std::string &command, const std::vector<std::string> &args);
};
