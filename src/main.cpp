#include <iostream>
#include "Arguments.h"
#include "Functions.h"
#include "DirProcessor.h"
#include "Types.h"
#include "Constants.h"

using namespace estim;
using namespace estim::constants;

namespace {
    bool process_single_file(const std::filesystem::path& canonical, const OutputLevel out_level) {
        std::cout << "Processing file: " << canonical << "\n";
        try {
            const auto lines = process_file(canonical);
            std::cout << "Result: " << lines << '\n';
            return true;
        } catch (const std::runtime_error& err) {
            if (out_level >= OutputLevel::ErrorsOnly) {
                std::cout << err.what() << '\n';
            }
            return false;
        }
    }

    void process_directory(const std::filesystem::path& canonical, const Arguments& args) {
        DirProcessor dp {canonical};
        dp.set_recursive(args.is_recusive());
        dp.set_extensions(args.get_extensions());
        dp.set_output(args.get_output_level());
        dp.set_ignore_list(args.get_ignore_list());
        const auto result = dp.process();
        std::cout << "Result: " << result << '\n';
    }
}

int main(int argc, char **argv) {
    try {
        Arguments args(argv);
        if (args.is_help()) {
            std::cout << help_str;
            return 0;
        }

        std::error_code err;
        const auto working_path {std::filesystem::canonical(args.get_path(), err)};
        if (err) {
            throw std::runtime_error {err.message()};
        }
        if (std::filesystem::is_directory(working_path)) {
            process_directory(working_path, args);
        } else {
            if (!process_single_file(working_path, args.get_output_level())) 
                return 1;
        }
    } catch (const std::runtime_error& err) {
        std::cout << err.what() << '\n';
        return 1;
    }
    return 0;
}
