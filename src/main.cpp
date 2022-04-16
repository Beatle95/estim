#include <iostream>
#include "Arguments.h"
#include "Functions.h"
#include "DirProcessor.h"
#include "Types.h"

using namespace estim;

namespace {
    const char *help_str {
        "This program is intended for counting number of lines of code in project\n\n"
        "Available flags:\n"
        "\t-h - show this help information\n"
        "\t-p - specify directory to work with\n"
        "\t-r - recursievly check subdirectories\n"
        "\t-f - process specified file\n"
        "\t-o<number> - set output level (default is 1)\n"
        "\t-i - specify directory or file to ignore while processing (do not accepts '..' in path)\n"
    };
}

int main(int argc, char **argv) {
    try {
        Arguments args(argc, argv);
        std::filesystem::current_path(args.get_path());

        if (args.is_help()) {
            std::cout << help_str;
            return 0;
        }

        if (args.is_file_flag_used()) {
            std::cout << "File: \"" << args.get_path() << "\"\n";
        } else {
            std::cout << "Directory: \"" << args.get_path() << "\"\n";
            std::cout << "Extensions: ";
            bool first {true};
            for (const auto& ext : args.get_extensions()) {
                if (!first) {
                    std::cout << ", ";
                }
                std::cout << ext;
                first = false;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        if (args.is_file_flag_used()) {
            try {
                if (std::filesystem::is_directory(args.get_path())) {
                    throw std::runtime_error("Specified file is directory");
                }
                const auto lines = process_file(args.get_path());
                std::cout << "Result: " << lines << '\n';
                return 0;
            } catch (const std::runtime_error& err) {
                if (args.get_output_level() >= OutputLevel::ErrorsOnly) {
                    std::cout << err.what() << '\n';
                    return 1;
                }
            }
        } else {
            if (!std::filesystem::is_directory(args.get_path())) {
                std::cout << "Specified path is not directory\n";
                return 1;
            }
            DirProcessor dp {args.get_path()};
            dp.set_recursive(args.is_recusive());
            dp.set_extensions(args.get_extensions());
            dp.set_output(args.get_output_level());
            dp.set_ignore_list(args.get_ignore_list());
            const auto result = dp.process();
            std::cout << "Result: " << result << '\n';
            return 0;
        }
    } catch (const std::string& err) {
        std::cout << err << '\n';
        return 1;
    }

    std::cout << "Unexpected error\n";
    return 1;
}
