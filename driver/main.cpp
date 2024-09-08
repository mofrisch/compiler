
#include <iostream>
#include <string>

#include "Checks.h"
#include "Logger.h"
#include "Stages.h"

int main(int argc, char *argv[]) {
  Logger &logger = Logger::instance();
  logger.setLogLevel(LogLevel::Debug);

  try {
    Checks checks;
    Stages stages;
    checks.privileges();
    auto arguments = checks.arguments(argc, argv);
    std::string compiler = arguments[0];
    const std::string& basename = arguments[1];

    stages.preprocess(basename);
    stages.compile(compiler,basename);
    stages.assemble(basename);
    stages.link(basename);

    logger.log(LogLevel::Info,
               "Compilation successful! Executable: " + basename);

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } // end try-catch


}
