
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include "utilities.h" // Adjust the path as necessary



int main(int argc, char *argv[]) {

  #ifndef __OPTIMIZE__
  std::cout << "This is a debug build." << std::endl;
  std::cout << "Number of arguments: " << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }
  #endif
    
  try {
    
    check_privileges();
    check_arguments(argc, argv);
    std::string compiler = argv[1];
    std::string basename = validate_and_get_basename(argv[2]);
    

    // std::string basename = "first";
    
    preprocess(basename);
    compile(basename);
    assemble(basename);
    link(basename);

    std::cout << "Compilation successful! Executable: " << basename
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } // end try-catch
  
    return 0;
}
