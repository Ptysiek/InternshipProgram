#pragma once

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Event.h"


class InputOutput {
    const std::string _logTarget;
    const std::string _badLogTarget;
    

public:
    InputOutput(const std::string& logTarget):
        _logTarget(logTarget),
        _badLogTarget("bad_" + logTarget)
    {}

    //#######################################################################################################
    std::vector<std::string> readPaths(const std::string& path) const {
        DIR* directory;
        directory = opendir(path.c_str());
        if (!directory) {
            throw std::logic_error("InputOutput::readPaths: Cannot Read Given Path:  [" + path + "]");
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
            throw std::logic_error("InputOutput::readFile: Cannot Read Given Path:  [" + path + "]");
        }
        std::stringstream result; 
        std::copy(
            std::istreambuf_iterator<char>(read), 
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(result));
        read.close();
        return result.str();
    }

    //#######################################################################################################
    void writeToLog(const Event& event) const {
        streamToLog(event.log(), _logTarget); 
    }

    void writeToLog(const std::string& log) const {
        streamToLog(log, _logTarget); 
    }

    void writeToLog(const std::string& log, const std::string& logType) const {
        streamToLog(log, _badLogTarget, logType); 
    }

    //#######################################################################################################
    void waitForUserInput() const {
        writeToUser("Press [enter] to continue.   ");
        std::cin.get();
    }

    void writeToUser(const Event& event) const {
        writeToUser("  " + event.log());
    }

    void writeToUser(const std::string& log) const {
        std::cout << log << "\n";
    }


private:
    //#######################################################################################################
    void streamToLog(
        const std::string& log, 
        const std::string& logTarget,
        const std::string& logType = std::string()
    ) const {
        std::ofstream logOutput(logTarget, std::ios_base::app);
        if (!logOutput || !logOutput.is_open()) {
            throw std::logic_error("InputOutput::writeToLog: Cannot Write Given Log:  [" + log + "]");
        }
        logOutput << logType << log << "\n";
        logOutput.close();
    }
};

