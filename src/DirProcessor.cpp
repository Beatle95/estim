#include "DirProcessor.h"
#include <algorithm>
#include <cctype>
#include <stack>
#include <iostream>
#include "Functions.h"
#include "Types.h"

namespace fs = std::filesystem;

namespace estim {

bool is_path_contains(const fs::path& base_path, const fs::path& check_path) {
    fs::path::iterator base_it = --base_path.end();
    fs::path::iterator check_it = --check_path.end();

    // check_path was entered by user in the cli arguments, so there can be '/' at the end
    if (check_it->empty())
        --check_it;
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

DirProcessor::DirProcessor(const std::filesystem::path& path) 
    : main_path_(path) {
}

void DirProcessor::print_info() const {
    std::cout << "Directory: " << main_path_ << "\n";
    std::cout << "Extensions: ";
    bool first {true};
    for (const auto& ext : extenstions_) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << ext;
        first = false;
    }
    std::cout << "\n\n";
}

size_t DirProcessor::process() {
    print_info();

    size_t result = 0;
    // I want to avoid recursion
    std::stack<fs::path> dirs;
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
                if (extenstions_.find(entry.path().extension().string()) == extenstions_.end()
                    || is_ignored(entry.path())) {

                    if (output_lvl_ >= OutputLevel::Max) {
                        std::cout << "Skipped: " << entry.path() << "\n";
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
    return result;
}

void DirProcessor::set_extensions(const std::list<std::string>& exts) {
    for (const auto& ext : exts) {
        extenstions_.insert(std::string(".") + ext);
    }
}

void DirProcessor::set_ignore_list(const std::list<std::string>& ignore) {
    for (const auto& item : ignore) {
        const fs::path p {item};
        if (p.empty())
            continue;
            
        if (p.is_absolute() || *(p.begin()) == "." || *(p.begin()) == "..") {
            ignored_.insert(fs::weakly_canonical(p));
        } else {
            ignored_.insert(p);
        }
    }
}

bool DirProcessor::is_ignored(const std::filesystem::path& path) {
    for (const auto& item : ignored_) {
        if (item.is_absolute()) {
            if (fs::equivalent(item, path))
                return true;
        } else {
            if (is_path_contains(path, item))
                return true;
        }
    }
    return false;
}

} // namespace estim
