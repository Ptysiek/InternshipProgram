#pragma once

#include <string>

#include "Event.h"


class Program {
    const bool _isInitializable;
    const std::string _targetFolder;


public:
    Program(const int argc, const char* const * const argv):
        _isInitializable(argc > 1),
        _targetFolder(initTargetFolder(argv))
    {}
    


private:
    std::string initTargetFolder(const char* const * const argv) {
        if (_isInitializable) {
            return argv[1];
        }
        return std::string();   
    }


};

