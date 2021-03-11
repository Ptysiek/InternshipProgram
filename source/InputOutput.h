#pragma once

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Event.h"


class InputOutput {
    const std::string _outputLogTarget;
    

public:
    InputOutput(const std::string& outputLogTarget):
        _outputLogTarget(outputLogTarget)
    {}

    std::vector<std::string> readPaths(const std::string& path) const {
        DIR* directory;
        directory = opendir(path.c_str());
        if (!directory) {
            throw std::logic_error("InputOutput::readPaths: Cannot Read Given Path: " + path + ".\n");
        }

        std::vector<std::string> result;
        struct dirent* entry;
        while ((entry = readdir(directory)) != NULL) {
            const auto data = static_cast<std::string>(entry->d_name);
            if (data != "." && data != "..") {
                result.emplace_back(data);
            }
        }
        closedir(directory);
        return result;
    }

    std::string readFile(const std::string& path) const {
        std::ifstream read(path);        
        if (!read || !read.is_open()) {
            throw std::logic_error("InputOutput::readFile: Cannot Read Given Path: " + path + ".\n");
        }
        std::stringstream result; 
        std::copy(
            std::istreambuf_iterator<char>(read), 
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(result));
        read.close();
        return result.str();
    }

    void writeToLog(const Event& event) const {
        std::ofstream logOutput(_outputLogTarget, std::ios_base::app);
        if (!logOutput || !logOutput.is_open()) {
            throw std::logic_error("InputOutput::writeToLog: Cannot Write to Given Event: " + event.log() + ".\n");
        }
        logOutput << event.log() << "\n";
        logOutput.close();
    }

};

