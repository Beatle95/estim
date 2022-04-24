#include "Arguments.h"
#include <filesystem>
#include "argh.h"
#include "Constants.h"

using namespace estim::constants;

namespace estim {

std::list<std::string> split(const std::string& str, char delim) {
    std::list<std::string> ret_val;
    auto start = 0U;
    auto end = str.find(delim);
    while (end != std::string::npos) {
        ret_val.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    ret_val.push_back(str.substr(start, end));
    return ret_val;
}

Arguments::Arguments(char **argv) 
    : work_path_(std::filesystem::current_path().string()) {

    argh::parser cmdl;
    cmdl.add_params({ext_param.c_str(), ignore_param.c_str(), output_param.c_str()});
    cmdl.parse(argv);

    for (const auto& flag : cmdl.flags()) {
        if (flag == recursive_flag) {
            is_recursive_ = true;
        } else if (flag == help_flag) {
            is_help_ = true;
        } else {
            throw std::runtime_error {std::string {"Unknown flag: "} + flag};
        }
    }

    for (const auto& param : cmdl.params()) {
        if (param.first == ext_param) {
            exts_ = split(param.second, ',');
        } else if (param.first == output_param) {
            int num = -1;
            try {
                num = std::stoi(param.second);
            } catch (...) {}
            if (num < min_output_level || num > max_output_level) {
                throw std::runtime_error {"Unexpected value for --o parameter"};
            }
            output_level_ = static_cast<OutputLevel>(num);
        } else if (param.first == ignore_param) {
            ignore_list_.push_back(param.second);
        } else {
            throw std::runtime_error {"Unknown parameter: " + param.first};
        }
    }
    
    for (auto it = ++cmdl.pos_args().begin(); it != cmdl.pos_args().end(); ++it) {
        work_path_ = *it;
    }
}

} // namespace estim
