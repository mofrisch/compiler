
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "Checks.hpp"
#include "Stages.hpp"

int main(int argc, char *argv[]) {

  try {
    Checks checks;

    checks.privileges();
    checks.arguments(argc, argv);
    std::string compiler = argv[1];
    std::string basename = checks.filename(argv[2]);

    Stages stages;

    stages.preprocess(basename);
    stages.compile(basename);
    stages.assemble(basename);
    stages.link(basename);

    std::cout << "Compilation successful! Executable: " << basename
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } // end try-catch

  return 0;
}
