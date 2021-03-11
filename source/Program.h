#pragma once

#include <algorithm>
#include <exception>
#include <experimental/filesystem>  
    // Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
    // Needs to be linked with static library: GCC[-lstdc++fs] or LLVM/Clang[-lc++fs].
    // Or You can use GCC(9.0 - 2020)
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Event.h"
#include "File.h"
#include "InputOutput.h"


class Program {
    using DirectoryEntry = std::experimental::filesystem::directory_entry;
    using FilesState = std::map<std::string, File>;

    FilesState _filesState;
    const DirectoryEntry _target;
    const std::string _targetPath;
    const InputOutput _stream;


public:
    explicit Program(const int argc, const char* const * const argv):
        _filesState(),
        _target(initTarget(argc, argv)),
        _targetPath(initTargetPath()),
        _stream("logs.txt")
    {}
    
    void execute() {
        if (!validateTarget()) {
            waitForUserInput();
            return;
        }
        mainLogicLoop();
    }


private:
    //#######################################################################################################
    std::string initTarget(const int argc, const char* const * const argv) const {
        if (argc > 1) {
            return argv[1];
        }
        return std::string();   
    }
    
    std::string initTargetPath() const {
        std::string result = _target.path().c_str();
        return (result.back() == '/')? result : result + '/';
        
        if (result.back() != '/') {
            result += '/';
        }
        return result;
    }
    
    //#######################################################################################################
    bool validateTarget() const {
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

    void waitForUserInput() const {
        std::cerr << "Press [enter] to continue.   ";
        std::cin.get();
    }

    void mainLogicLoop() {
        _filesState = captureFilesState();
        while (true) {
            auto [latestFilesState, success] = tryCaptureFilesState();
            if (!success) {
                continue;
            }
            auto newFiles = enlistCreatedFiles(latestFilesState);
            for (auto& [key, file] : _filesState) {
                const auto i = latestFilesState.find(key);
                if (i == latestFilesState.end()) {
                    auto j = std::find_if(begin(newFiles), end(newFiles), 
                        [file](const auto& newFile){ return (newFile.data() == file.data()); });
                    
                    if (j == newFiles.end()) {
                        Event event {Event::Type::Deleted, file.name()};
                        _stream.writeToLog(event);
                    }
                    else {
                        Event event {Event::Type::Renamed, file.name()};
                        _stream.writeToLog(event);
                        *j = newFiles.back();
                        newFiles.pop_back();
                    }
                }
                else if (i->second.modificationTime() != file.modificationTime()) {
                    Event event {Event::Type::Edited, file.name()};
                    _stream.writeToLog(event);
                }
            }
            for (const auto& file : newFiles) {
                Event event {Event::Type::Created, file.name()};
                _stream.writeToLog(event);
            }
            std::swap(_filesState, latestFilesState);
        }
    }

    //#######################################################################################################
    std::pair<FilesState, bool> tryCaptureFilesState() const {
        using FileSystemError = std::experimental::filesystem::v1::__cxx11::filesystem_error;
        
        FilesState latestFilesState;
        bool success = true;
        try {
            latestFilesState = captureFilesState();
        } 
        catch(const FileSystemError& e) { 
            std::cerr << e.what() << "\n";
            success = false;
        }
        catch(const std::logic_error& e) {
            std::cerr << e.what() << "\n";
            success = false;
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << "\n";
            success = false;
        }
        return {latestFilesState, success};
    }
    
    FilesState captureFilesState() const {
        FilesState result;
        auto paths = _stream.readPaths(_targetPath);
        for (const auto& path : paths) {
            const DirectoryEntry meta(_targetPath + path);
            const auto data = _stream.readFile(_targetPath + path);
            result.insert({path, File(meta, data)}); 
        }
        return result;
    }
    
    std::vector<File> enlistCreatedFiles(const FilesState& latestFilesState) const {
        std::vector<File> result;
        result.reserve(latestFilesState.size() / 3);
        for (auto& [key, file] : latestFilesState) {
            if (_filesState.find(key) == _filesState.end()) {
                result.emplace_back(file);
            }
        }
        return result;
    }
    //#######################################################################################################
};

