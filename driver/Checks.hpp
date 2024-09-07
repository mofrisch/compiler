#include <string>

class Checks {
    public:
    void privileges();
    std::string filename(const std::string &filename);
    void arguments(int argc, char *argv[]);
    };