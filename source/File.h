#pragma once

#include <experimental/filesystem>  
    // Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
    // Needs to be linked with static library: GCC[-lstdc++fs] or LLVM/Clang[-lc++fs].
    // Or You can use GCC(9.0 - 2020)
#include <string>


class File {
    using DirectoryEntry = std::experimental::filesystem::directory_entry;
    using ModificationTime = std::experimental::filesystem::file_time_type;

    ModificationTime _modificationTime;
    std::string _name;
    std::string _data;
    

public:
    explicit File(const DirectoryEntry& meta, const std::string& data):
        _modificationTime(std::experimental::filesystem::last_write_time(meta)),
        _name(meta.path().c_str()),
        _data(data)
    {}

    ModificationTime modificationTime() const { return _modificationTime; }
    std::string name() const { return _name; } 
    std::string data() const { return _data; }
};

