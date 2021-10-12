#pragma once

// <FILESYSTEM>
// Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
// Needs to be linked with static library: GCC[-lstdc++fs] or LLVM/Clang[-lc++fs].
// Or You can use GCC(9.0 - 2020)
#include <filesystem>  
#include <string>

class File {
    std::filesystem::file_time_type _modificationTime;
    std::string _name;
    std::string _data;
    
public:
    explicit File(const std::filesystem::directory_entry& meta, const std::string& data);

    std::filesystem::file_time_type modificationTime() const;
    std::string name() const;
    std::string data() const;
};
