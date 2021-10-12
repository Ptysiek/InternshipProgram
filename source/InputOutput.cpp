#include "InputOutput.h"

InputOutput::InputOutput(const std::string& logTarget):
    _logTarget(logTarget),
    _badLogTarget("bad_" + logTarget)
{}

std::vector<std::string> InputOutput::readPaths(const std::string& path) const {
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

std::string InputOutput::readFile(const std::string& path) const {
    std::ifstream read(path);        
    if (!read || !read.is_open()) {
        throw std::logic_error("InputOutput::readFile: Cannot Read Given Path:  [" + path + "]");
    }
    std::string result; 
    while (read) {
        read >> result;
    }
    read.close();
    return result;
}

void InputOutput::writeToLog(const Event& event) const {
    streamToLog(event.fullLog(), _logTarget); 
}

void InputOutput::writeToLog(const std::string& log) const {
    streamToLog(log, _logTarget); 
}

void InputOutput::writeToLog(const std::string& log, const std::string& logType) const {
    streamToLog(log, _badLogTarget, logType); 
}

void InputOutput::writeToUser(const Event& event) const {
    writeToUser("  " + event.log());
}

void InputOutput::writeToUser(const std::string& log) const {
    std::cerr << log << "\n";
}

void InputOutput::writeToLogAndUser(const Event& event) const {
    writeToLog(event);
    writeToUser(event);
}

void InputOutput::waitForUserInput() const {
    writeToUser("Press [enter] to continue.   ");
    std::cin.get();
}

void InputOutput::streamToLog(
    const std::string& log, 
    const std::string& logTarget,
    const std::string& logType) const 
{
    std::ofstream logOutput(logTarget, std::ios_base::app);
    if (!logOutput || !logOutput.is_open()) {
        throw std::logic_error("InputOutput::writeToLog: Cannot Write Given Log:  [" + log + "]");
    }
    logOutput << logType << log << "\n";
    logOutput.close();
}
