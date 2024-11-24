#include "stages.h"
#include "command.h"

#include <string>
#include <vector>

void Stages::execute_stage(const std::string &stage_name, const std::string &command_name,
                           const std::vector<std::string> &args) {
    Command::run(stage_name, command_name, args);
}


void Stages::preprocess(const std::string &basename) {
    execute_stage("Preprocessing", "gcc", {"-E", "-P", basename + ".c", "-o", basename + ".i"});
}

void Stages::compile(const std::string &basename) {
    execute_stage("Compiling", "./mcc", {basename + ".i", "-o", basename + ".s"});
}

void Stages::assemble(const std::string &basename) {
    execute_stage("Assembling", "as", {basename + ".s", "-o", basename + ".o"});
}

void Stages::link(const std::string &basename) { execute_stage("Linking", "ld", {basename + ".o", "-o", basename}); }
