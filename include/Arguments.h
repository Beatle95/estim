#ifndef ARGUMENTS
#define ARGUMENTS
#include <list>
#include <string>
#include "Types.h"

namespace estim {

    class Arguments {
    public:
        Arguments(char **argv);
        bool is_help()                 const        { return is_help_; }
        bool is_recusive()             const   { return is_recursive_; }
        OutputLevel get_output_level() const   { return output_level_; } 
               
        const std::list<std::string>& get_ignore_list() const { return ignore_list_; }
        const std::list<std::string>& get_extensions()  const        { return exts_; }
        const std::string& get_path()                   const   { return work_path_; }

    private:
        OutputLevel output_level_ {OutputLevel::ErrorsOnly};
        std::list<std::string> exts_;
        std::list<std::string> ignore_list_;
        std::string work_path_;
        bool is_recursive_ {false};
        bool is_help_ {false};
    };

}

#endif
