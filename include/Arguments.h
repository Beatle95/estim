#ifndef ARGUMENTS
#define ARGUMENTS
#include <list>
#include <string>
#include "Types.h"

class Arguments {
public:
    Arguments(int argc, char **argv);
    bool is_help() const { return is_help_; }
    bool is_recusive() const { return is_recursive_; }
    bool is_file_flag_used() const { return file_flag_used_; }
    bool is_path_flag_used() const { return path_flag_used_; }
    OutputLevel get_output_level() const { return output_level_; }
    const std::string& get_path() const { return work_path_; }
    const std::list<std::string>& get_extensions() const { return exts_; }
    const std::list<std::string>& get_ignore_list() const { return ignore_list_; }

private:
    OutputLevel output_level_ {OutputLevel::ErrorsOnly};
    std::list<std::string> exts_;
    std::list<std::string> ignore_list_;
    bool is_recursive_ {false};
    bool is_help_ {false};
    std::string work_path_;
    int file_flag_used_ {0};
    int path_flag_used_ {0};
};

#endif
