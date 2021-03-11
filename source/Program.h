#pragma once

#include <exception>
#include <experimental/filesystem>  
    // Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
    // Needs to be linked with static library: GCC[-lstdc++fs] or LLVM/Clang[-lc++fs].
    // Or You can use GCC(9.0 - 2020)
#include <iostream>
#include <string>
#include <vector>

#include "Event.h"
#include "InputOutput.h"


class Program {
    using Path = std::experimental::filesystem::directory_entry;

    const bool _isInitializable;
    const std::string _targetFolder;
    Path _path;

    InputOutput _stream;

public:
    explicit Program(const int argc, const char* const * const argv):
        _isInitializable(argc > 1),
        _targetFolder(initTargetFolder(argv)),
        _path(_targetFolder),
        _stream()
    {}
    
    void execute() {
        if (!validateTargetFolder()) {
            waitForUserInput();
            return;
        }
        
        auto vctr = _stream.readPaths(_path.path().c_str());

        for (const auto& path : vctr) {
            std::cout << path << "\n";
        }
        std::cout << "\n\n";

        auto test = _stream.readFile("../source/main.cpp");

        std::cout << test;
        
        //using namespace std::experimental;
        //std::cout << _path.last_write_time() << "\n\n";
        //std::cout << filesystem::last_write_time(_path) << "\n\n";
        //auto time = filesystem::last_write_time(_path);
        //auto timeb = filesystem::last_write_time(_path);
        
        //std::cout << (time == timeb);
        //std::cout << _path << "\n";
        //std::cout << _path.path() << "\n";
        //std::cout << _path.path().c_str() << "\n";
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
        const auto status = _path.status();

        if (!filesystem::status_known(status)) {
            throw std::logic_error("Program::validateTargetFolder: Unknown Status.\n");
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

