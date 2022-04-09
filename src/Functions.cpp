#include "Functions.h"
#include <fstream>
#include <stdexcept>
#include <stack>
#include <iostream>

namespace fs = std::filesystem;

namespace estim {
    size_t process_file(const std::filesystem::path& path) {
        std::ifstream file;
        file.open(path, std::ifstream::in);
        if (!file.good()) {
            throw std::runtime_error(std::string("Unable to open file: ") + path.string());
        }

        char c;
        size_t lines = 1;
        while (file.get(c)) {
            if (c == '\n') {
                ++lines;
            }
        }
        return lines;
    }
}
