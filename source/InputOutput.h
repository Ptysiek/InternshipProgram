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
    InputOutput(const std::string& logTarget);
    
    std::vector<std::string> readPaths(const std::string& path) const;
    std::string readFile(const std::string& path) const;

    void writeToLog(const Event& event) const;
    void writeToLog(const std::string& log) const;
    void writeToLog(const std::string& log, const std::string& logType) const;

    void writeToUser(const Event& event) const;
    void writeToUser(const std::string& log) const;

    void writeToLogAndUser(const Event& event) const;
    void waitForUserInput() const;

private:
    void streamToLog(
        const std::string& log, 
        const std::string& logTarget,
        const std::string& logType = std::string()) const;
};
