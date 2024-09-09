#pragma once

#include <string>
#include <vector>

enum class CommandStatus { SUCCESS, FORK_FAILED, EXEC_FAILED };

class Stages {
public:
    static void preprocess(const std::string &basename);

    static void compile(const std::string &compiler, const std::string &basename);

    static void assemble(const std::string &basename);

    static void link(const std::string &basename);

    static void simple_test();

private:
    // TODO CommandStatus
    static auto run_command(const std::string &command,
                            const std::vector<std::string> &args) -> int;

    static void execute_stage(const std::string &stage_name, const std::string &command,
                              const std::vector<std::string> &args);
};
