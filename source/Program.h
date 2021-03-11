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
#include "File.h"
#include "InputOutput.h"


class Program {
    using DirectoryEntry = std::experimental::filesystem::directory_entry;

    const DirectoryEntry _target;
    const std::string _targetPath;
    const InputOutput _stream;


public:
    explicit Program(const int argc, const char* const * const argv):
        _target(initTarget(argc, argv)),
        _targetPath(initTargetPath()),
        _stream()
    {}
    
    void execute() {
        if (!validateTarget()) {
            waitForUserInput();
            return;
        }
        
        auto fileState = captureFilesState();

        
        for (const auto& file : fileState) {
            std::cout << file.beenModified() << " " << file.name() << "\n";
        }   
        
        //auto a = std::system("clear");
        //std::cout << a;
        //if (const auto errorCode = std::system("clear"); errorCode != 0) {
        //    throw std::logic_error("Program::execute: system returned error: " + std::to_string(errorCode));
        //}


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
    std::string initTarget(const int argc, const char* const * const argv) {
        if (argc > 1) {
            return argv[1];
        }
        return std::string();   
    }
    
    std::string initTargetPath() {
        std::string result = _target.path().c_str();
        return (result.back() == '/')? result : result + '/';
        
        if (result.back() != '/') {
            result += '/';
        }
        return result;
    }
    
    void waitForUserInput() {
        std::cerr << "Press [enter] to continue.   ";
        std::cin.get();
    }

    bool validateTarget() {
        using namespace std::experimental;
        
        if (_target == std::string()) {
            std::cerr << "No argument was given.\n";
            return false;
        }
        const auto status = _target.status();

        if (!filesystem::status_known(status)) {
            throw std::logic_error("Program::validateTarget: Unknown Status.\n");
        }
        if (!filesystem::exists(status)) {
            std::cerr << "Specified path does not exist. " << _target << "\n";
            return false;
        }
        if (!filesystem::is_directory(status)) {
            std::cerr << "Specified path is not a directory. " << _target << "\n";
            return false;
        }
        return true;
    }
    
    std::vector<File> captureFilesState() {
        using namespace std::experimental;
        
        std::vector<File> result;
        auto paths = _stream.readPaths(_targetPath);
        for (const auto& path : paths) {
            DirectoryEntry meta(_targetPath + path);
            if (filesystem::is_directory(meta.status())) {
                continue;
            }
            auto data = _stream.readFile(_targetPath + path);
            result.emplace_back(File(meta, data));
        }
        return result;
    }
};

