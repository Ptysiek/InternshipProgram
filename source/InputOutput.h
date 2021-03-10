#pragma once

#include <string>
#include <vector>

#include <fstream>
#include <dirent.h>



struct InputOutput {
    std::vector<std::string> readPaths(const std::string& targetPath) {
        std::vector<std::string> result;
        DIR* directory;
        directory = opendir(targetPath.c_str());

        if (!directory) {
            return result;
        }
        struct dirent* entry;
        while ((entry = readdir(directory)) != NULL) {
            const std::string data = entry->d_name;
            if (data == "." || data == "..") {
                continue;
            }
            result.push_back(data);
        }
        closedir(directory);
        return result;
    }

    std::string readFile(const std::string& path) {
        std::ifstream readFile(path);        
        if (!readFile || !readFile.is_open()) {
            throw std::logic_error("Cannot read given file path: " + path + "\n");
        }

        //std::string result(readFile.rdbuf());
        
        std::string result(readFile.beg, readFile.end);
        //std::string record;
        //while (std::getline(readFile, record)) {
        //    result.push_back(record);
        //}
        readFile.close();
        return result;
    }
};

