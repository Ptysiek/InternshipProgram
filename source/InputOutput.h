#pragma once

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


struct InputOutput {
    std::vector<std::string> readPaths(const std::string& path) const {
        DIR* directory;
        directory = opendir(path.c_str());

        if (!directory) {
            throw std::logic_error("InputOutput::readPaths: Cannot read given folder path: " + path + ".\n");
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
            throw std::logic_error("InputOutput::readFile: Cannot read given file path: " + path + ".\n");
        }
        std::stringstream result; 
        
        std::copy(
            std::istreambuf_iterator<char>(read), 
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(result));
        
        read.close();
        return result.str();
    }
};

