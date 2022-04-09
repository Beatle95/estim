#ifndef DIR_PROCESSOR
#define DIR_PROCESSOR
#include <filesystem>
#include <list>
#include <vector>

enum class OutputLevel {
    Silent,
    ErrorsOnly,
    Max
};

class DirProcessor
{
public:
    DirProcessor(const std::filesystem::path& path) : main_path_(path) {}
    void set_recursive(bool recursive) { recursive_ = recursive; }
    void set_output(OutputLevel lvl) { output_lvl_ = lvl; }
    void set_extensions(const std::list<std::string>& exts);
    size_t process();

private:
    const std::filesystem::path main_path_;
    std::vector<std::string> extenstions_;
    bool recursive_ {false};
    // we can set printing output level
    OutputLevel output_lvl_ {OutputLevel::Max};
};


#endif
