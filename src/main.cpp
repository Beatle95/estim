#include <iostream>
#include <cstring>
#include <list>
#include <filesystem>

/*
    -p <followed by dest> - sets specified path as searchpath
    -r                    - recursivly search in subdirectories
*/

namespace {
    const char *help_str {
        "This program is intended for counting number of lines of code in project\n\n"
        "Available flags:\n"
        "\t-h - show this help information\n"
        "\t-p - specify directory to work with\n"
        "\t-r - recursievly check subdirectories\n"
        "\t-f - process specified file\n"
    };

    const char *file_flag {"-f"};
    const char *recursive_flag {"-r"};
    const char *path_flag {"-p"};
    const char *help_flag {"-h"};
    const char *arguments_error {"Error in argument: "};
}

int main(int argc, char **argv) {
    std::list<std::string> exts;
    std::string work_path {std::filesystem::current_path().string()};
    bool is_recursive {false};

    int file_flag_used {0};
    int path_flag_used {0};

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], help_flag) == 0) {
                std::cout << help_str;
                return 0;

            } else if (strcmp(argv[i], path_flag) == 0) {
                // path
                ++path_flag_used;
                if (++i >= argc || argv[i][0] == '-') {
                    std::cout << arguments_error << path_flag << '\n';
                    return 1;
                } else {
                    std::error_code err;
                    work_path = std::filesystem::canonical(argv[i], err);
                    if (err) {
                        std::cout << "Specified path: \"" << argv[i] << "\" is invalid\n";
                        return 1;
                    }
                }

            } else if (strcmp(argv[i], recursive_flag) == 0) {
                // recursive
                is_recursive = true;

            } else if (strcmp(argv[i], file_flag) == 0) {
                // file
                ++file_flag_used;
                if (++i >= argc || argv[i][0] == '-') {
                    std::cout << arguments_error << file_flag << '\n';
                    return 1;
                } else {
                    std::error_code err;
                    work_path = std::filesystem::canonical(argv[i], err);
                    if (err) {
                        std::cout << "Specified path: \"" << argv[i] << "\" is invalid\n";
                        return 1;
                    }
                }
            } else {
                std::cout << arguments_error << "unrecognized argument\n";
            }
        } else {
            // this is not flag, so we assume that this is extension
            exts.push_back(argv[i]);
        }
    }

    if (file_flag_used && path_flag_used) {
        std::cout << "Only one of -p or -f may be used" << '\n';
        return 1;
    }
    if (file_flag_used > 1 || path_flag_used > 1) {
        std::cout << "Flags -p and -f may be used only once" << '\n';
        return 1;
    }
    if (!std::filesystem::exists(work_path)) {
        std::cout << "Specified path are not exist: " << work_path << '\n';
    }
    if (exts.empty() && file_flag_used == 0) {
        std::cout << "Error: no extensions specified" << '\n';
        return 1;
    }

    std::cout << "Working directory: \"" << work_path << "\"\n";
    std::cout << "Selected extensions: ";
    bool first {true};
    for (const auto& ext : exts) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << ext;
        first = false;
    }
    std::cout << std::endl;

    if (file_flag_used) {
        // TODO: process file
        return 0;
    } else {
        std::filesystem::current_path(work_path);
        // TODO: process directory
        return 0;
    }

    std::cout << "Unexpected error\n";
    return 1;
}