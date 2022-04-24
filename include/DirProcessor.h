#ifndef DIR_PROCESSOR
#define DIR_PROCESSOR
#include <filesystem>
#include <list>
#include <set>
#include <utility>

namespace estim {
    enum class OutputLevel;

    class DirProcessor {
    public:
        DirProcessor(const std::filesystem::path& path);
        void set_recursive(bool recursive) { recursive_ = recursive; }
        void set_output(OutputLevel lvl) { output_lvl_ = lvl; }
        void set_extensions(const std::list<std::string>& exts);
        void set_ignore_list(const std::list<std::string>& ignore);
        size_t process();

    private:
        const std::filesystem::path main_path_;
        std::set<std::string> extenstions_;
        std::set<std::filesystem::path> ignored_;
        bool recursive_ {false};
        // we can set printing output level
        OutputLevel output_lvl_;

        bool is_ignored(const std::filesystem::path& path);
        void print_info() const;
    };
}

#endif
