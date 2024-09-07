#pragma once
#include <string>
#include <vector>

int run_command(const std::string &command,
                const std::vector<std::string> &args);
bool is_safe_filename(const std::string &filename);
void check_privileges();
bool validate_source_file(const std::string &source_file);
void run_stage(const std::string &stage_name, const std::string &command,
               const std::vector<std::string> &args);
std::string change_extension(const std::string &filename,
                             const std::string &new_ext);
std::string validate_and_get_basename(const std::string &filename);
void check_arguments(int argc, char *argv[]);
void preprocess(const std::string &basename);
void compile(const std::string &basename);
void assemble(const std::string &basename);
void link(const std::string &basename);