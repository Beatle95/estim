#include "DirProcessor.h"
#include <algorithm>
#include <stack>
#include <iostream>
#include "Functions.h"

using namespace estim;
namespace fs = std::filesystem;

void DirProcessor::set_extensions(const std::list<std::string>& exts) {
    extenstions_.reserve(exts.size());
    for (const auto& ext : exts) {
        extenstions_.push_back(std::string(".") + ext);
    }
}

size_t DirProcessor::process() {
    size_t result = 0;
    // I want to avoid recursion
    std::stack<fs::path> dirs;
    std::list<std::string> skipped_list;
    dirs.push(main_path_);

    while (!dirs.empty()) {
        const auto current_dir {dirs.top()};
        dirs.pop();

        for (const auto& entry : fs::directory_iterator(current_dir)) {
            if (entry.is_directory()) {
                if (recursive_) {
                    dirs.push(entry.path());
                }
            } else {
                // this is file
                if (std::find(extenstions_.begin(), extenstions_.end(), entry.path().extension()) == extenstions_.end()) {
                    if (output_lvl_ >= OutputLevel::Max) {
                        skipped_list.push_back(entry.path());
                    }
                    continue;
                }

                try {
                    const auto cur_result = process_file(entry.path());
                    if (output_lvl_ >= OutputLevel::Max) {
                        std::cout << "Found: "
                                  << std::setw(6)
                                  << cur_result
                                  << " in file "
                                  << entry.path()
                                  << "\n";
                    }
                    result += cur_result;
                } catch (const std::runtime_error& err) {
                    if (output_lvl_ >= OutputLevel::ErrorsOnly) {
                        std::cout << err.what() << '\n';
                    }
                }
            }
        }
    }

    if (output_lvl_ >= OutputLevel::Max) {
        for (const auto& p : skipped_list) {
            std::cout << "Skipped: \"" << p << "\"\n";
        }
    }
    return result;
}
