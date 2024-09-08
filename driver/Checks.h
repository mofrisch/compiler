#include <string>
#include <vector>


class Checks {
    public:
    void privileges();
    std::string filename(const std::string &filename);
    auto arguments(int argc, char *argv[]) -> std::vector<std::string>;
    };