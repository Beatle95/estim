#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace estim {
    namespace constants {
        const std::string help_flag         {"h"};
        const std::string recursive_flag    {"r"};
        const std::string ext_param       {"ext"};
        const std::string ignore_param {"ignore"};
        const std::string output_param      {"o"};
        const int min_output_level {0};
        const int max_output_level {3};

        const std::string help_str {
            "This program is intended for counting number of lines of code in project\n\n"
            "Available flags:\n"
            "\t-h - show this help information\n"
            "\t-r - recursievly check subdirectories\n"
            "Parameteres:\n"
            "\t--ext <comma separated exts>   - extensions to work with\n"
            "\t--ignore [<path>, <file_name>] - specify directory or file to ignore while processing (do not accepts '..' in path)\n"
            "\t--o=<number>                   - set output level (default is 1)\n"
            "And positional argument as path, which directory to process."
        };
    }
}

#endif // CONSTANTS_H
