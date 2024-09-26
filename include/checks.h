#pragma once

#include <string>
#include <vector>


class Checks {
public:
    static void privileges();

    static std::string filename(const std::string &filename);

    static auto arguments(int argc, char *argv[]) -> std::vector<std::string>;
};
