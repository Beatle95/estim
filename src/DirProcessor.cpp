#include "DirProcessor.h"
#include <algorithm>
#include <stack>
#include <iostream>
#include "Functions.h"
#include "Types.h"

using namespace estim;
namespace fs = std::filesystem;

namespace {
    bool is_path_contains(const fs::path& base_path, const fs::path& check_path) {
        fs::path::iterator base_it = --base_path.end();
        fs::path::iterator check_it = --check_path.end();
        while (true) {
            if (*check_it != *base_it) {
                return false;
            }
            if (check_it == check_path.begin()) {
                return true;
            }
            if (base_it == base_path.begin()) {
                break;
            }
            --check_it;
            --base_it;
        }
        return false;
    }
}

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
            if (!ignored_.empty() && is_ignored(entry.path())) {
                continue;
            }

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
                    if (output_lvl_ >= OutputLevel::WithFilesStat) {
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

void DirProcessor::set_ignore_list(const std::list<std::string>& ignore) {
    ignored_.reserve(ignore.size());
    for (const auto& item : ignore) {
        if (item.empty()) {
            continue;
        }
        if (item[0] == '.') {
            ignored_.push_back(fs::canonical(item));
        } else {
            ignored_.push_back(item);
        }
    }
}

bool DirProcessor::is_ignored(const std::filesystem::path& path) {
    for (const auto& item : ignored_) {
        if (is_path_contains(path, item)) {
            return true;
        }
    }
    return false;
}
