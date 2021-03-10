#pragma once

#include <exception>
#include <experimental/filesystem>  
    // Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
    // Needs to be linked with static library: 'libstdc++fs.a' [-lstdc++fs].
    // Or You can use GCC(9 - 2020)
#include <iostream>
#include <string>

#include "Event.h"
#include "InputOutput.h"


class Program {
    using Path = std::experimental::filesystem::directory_entry;

    const bool _isInitializable;
    const std::string _targetFolder;
    Path _path;


public:
    explicit Program(const int argc, const char* const * const argv):
        _isInitializable(argc > 1),
        _targetFolder(initTargetFolder(argv)),
        _path(_targetFolder)
    {}
    
    void execute() {
        if (!validateTargetFolder()) {
            waitForUserInput();
            return;
        }
        
        std::cout << _path << "\n\n";

    }


private:
    std::string initTargetFolder(const char* const * const argv) {
        if (_isInitializable) {
            return argv[1];
        }
        return std::string();   
    }

    bool validateTargetFolder() {
        using namespace std::experimental;
        
        if (!_isInitializable) {
            std::cerr << "No argument was given.\n";
            return false;
        }
        const auto& status = _path.status();

        if (!filesystem::status_known(status)) {
            throw std::logic_error("Program::validateTargetFolder: Unknown Status.");
        }
        if (!filesystem::exists(status)) {
            std::cerr << "Specified path does not exist. \'" << _targetFolder << "\'\n";
            return false;
        }
        if (!filesystem::is_directory(status)) {
            std::cerr << "Specified path is not a directory. \'" << _targetFolder << "\'\n";
            return false;
        }
        return true;
    }

    void waitForUserInput() {
        std::cerr << "Press [enter] to continue.   ";
        std::cin.get();
    }
};

