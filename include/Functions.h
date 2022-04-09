#ifndef FUNCTIONS
#define FUNCTIONS
#include <filesystem>

/*
    Utility functions file
*/

namespace estim {
    // process single file
    size_t process_file(const std::filesystem::path& path);
}

#endif
