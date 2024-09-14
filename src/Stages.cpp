#include "Stages.h"
#include "Command.h"

#include <string>
#include <vector>

// extern Log &logger;

//extern Command &command;;


void Stages::execute_stage(const std::string &stage_name, const std::string &command_name,
                           const std::vector<std::string> &args) {
    Command::run(stage_name, command_name, args);
}


void Stages::preprocess(const std::string &basename) {
    execute_stage("Preprocessing", "gcc", {"-E", "-P", basename + ".c", "-o", basename + ".i"});
}

void Stages::compile(const std::string &compiler, const std::string &basename) {
    execute_stage("Compiling", compiler, {"-S", basename + ".i", "-o", basename + ".s"});
}

void Stages::assemble(const std::string &basename) {
    execute_stage("Assembling", "as", {basename + ".s", "-o", basename + ".o"});
}

void Stages::link(const std::string &basename) {
    execute_stage("Linking", "gcc", {basename + ".o", "-o", basename});
}


