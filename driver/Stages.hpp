#include <string>
#include <vector>

enum class CommandStatus { SUCCESS, FORK_FAILED, EXEC_FAILED };

class Stages {
public:
  void preprocess(const std::string &basename);
  void compile(const std::string &basename);
  void assemble(const std::string &basename);
  void link(const std::string &basename);

private:
  // TODO CommandStatus
  int run_command(const std::string &command,
                            const std::vector<std::string> &args);

  void run_stage(const std::string &stage_name, const std::string &command,
                  const std::string &source_file,
                  const std::string &output_file);
};
