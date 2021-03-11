#pragma once

#include <experimental/filesystem>  
    // Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
    // Needs to be linked with static library: GCC[-lstdc++fs] or LLVM/Clang[-lc++fs].
    // Or You can use GCC(9.0 - 2020)
#include <string>


class File {
    using DirectoryEntry = std::experimental::filesystem::directory_entry;

    DirectoryEntry _iNode;
    std::string _latestData;
    bool _beenModified; 

public:
    explicit File(const std::string& path, const std::string& data):
        _iNode(path),
        _latestData(data),
        _beenModified(false)
    {}

    


};

