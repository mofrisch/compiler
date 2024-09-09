
#include <iostream>
#include <string>

#include "Checks.h"
#include "Logger.h"
#include "Stages.h"

int main(const int argc, char *argv[]) {
  Logger &logger = Logger::instance();
  logger.setLogLevel(LogLevel::Debug);

  try {
    Checks::privileges();
    const auto arguments = Checks::arguments(argc, argv);
    const std::string &compiler = arguments[0];
    const std::string &basename = arguments[1];

    Stages::preprocess(basename);
    Stages::compile(compiler, basename);
    Stages::assemble(basename);
    Stages::link(basename);

    logger.log(LogLevel::Info,
               "Compilation successful! Executable: " + basename);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } // end try-catch
}
