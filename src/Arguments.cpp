#include "Arguments.h"
#include <filesystem>
#include <cstring>
#include <sstream>

namespace {
    const char *file_flag {"-f"};
    const char *recursive_flag {"-r"};
    const char *path_flag {"-p"};
    const char *help_flag {"-h"};
    const char *ignore_flag {"-i"};
    const char output_flag_char {'o'};
    const char *arguments_error {"Error in argument: "};
}

Arguments::Arguments(int argc, char **argv) 
    : work_path_(std::filesystem::current_path().string()) {    

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            int arg_len = strlen(argv[i]);
            if (arg_len > 1 && argv[i][1] == output_flag_char) {
                // output level
                // argument cannot be 0 length, so this check cannot throw exception
                if (arg_len != 3) {
                    std::stringstream ss;
                    ss << arguments_error << "wrong -o flag";
                    throw ss.str();
                }

                int val;
                bool error {false};
                try {
                    val = std::stoi(&argv[i][2]);
                    if (val < static_cast<int>(OutputLevel::Silent) || val > static_cast<int>(OutputLevel::Max)) {
                        error = true;
                    }
                } catch (const std::invalid_argument& err) {
                    error = true;
                }
                if (error) {
                    std::stringstream ss;
                    ss << "Wrong value for -o flag";
                    throw ss.str();
                }
                output_level_ = static_cast<OutputLevel>(val);
            } else if (strcmp(argv[i], ignore_flag) == 0) {
                // path to ignore
                if (++i >= argc || argv[i][0] == '-') {
                    std::stringstream ss;
                    ss << arguments_error << ignore_flag;
                    throw ss.str();
                }
                ignore_list_.push_back(argv[i]);
            } else if (strcmp(argv[i], help_flag) == 0) {
                is_help_ = true;
                return;

            } else if (strcmp(argv[i], path_flag) == 0) {
                // path
                ++path_flag_used_;
                if (++i >= argc || argv[i][0] == '-') {
                    std::stringstream ss;
                    ss << arguments_error << path_flag;
                    throw ss.str();
                } else {
                    std::error_code err;
                    work_path_ = std::filesystem::canonical(argv[i], err);
                    if (err) {
                        std::stringstream ss;
                        ss << "Specified path: \"" << argv[i] << "\" is invalid";
                        throw ss.str();
                    }
                }

            } else if (strcmp(argv[i], recursive_flag) == 0) {
                // recursive
                is_recursive_ = true;

            } else if (strcmp(argv[i], file_flag) == 0) {
                // file
                ++file_flag_used_;
                if (++i >= argc || argv[i][0] == '-') {
                    std::stringstream ss;
                    ss << arguments_error << file_flag;
                    throw ss.str();
                } else {
                    std::error_code err;
                    work_path_ = std::filesystem::canonical(argv[i], err);
                    if (err) {
                        std::stringstream ss;
                        ss << "Specified path: \"" << argv[i] << "\" is invalid";
                        throw ss.str();
                    }
                }
            } else {
                std::stringstream ss;
                ss << arguments_error << "unrecognized argument";
                throw ss.str();
            }
        } else {
            // this is not flag, so we assume that this is extension
            exts_.push_back(argv[i]);
        }
    }


    if (file_flag_used_ && path_flag_used_) {
        std::stringstream ss;
        ss << "Only one of -p or -f may be used";
        throw ss.str();
    }
    if (file_flag_used_ > 1 || path_flag_used_ > 1) {
        std::stringstream ss;
        ss << "Flags -p and -f may be used only once";
        throw ss.str();
    }
    if (!std::filesystem::exists(work_path_)) {
        std::stringstream ss;
        ss << "Specified path are not exist: " << work_path_;
        throw ss.str();
    }
    if (exts_.empty() && file_flag_used_ == 0) {
        std::stringstream ss;
        ss << "Error: no extensions specified";
        throw ss.str();
    }
}
