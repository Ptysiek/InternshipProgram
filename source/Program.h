#pragma once

#include <iostream>
#include <string>


#include "Event.h"


class Program {
    const bool _isNotInitializable;
    const std::string _targetFolder;


public:
    Program(const int argc, const char* const * const argv):
        _isNotInitializable(argc < 2),
        _targetFolder(initTargetFolder(argv))
    {}
    
    void execute() {
        if (_isNotInitializable) {
            std::cout << "No argument was given.\n";
            waitForUserInput();
        }
        
    }


private:
    std::string initTargetFolder(const char* const * const argv) {
        if (_isNotInitializable) {
            return std::string();   
        }
        return argv[1];
    }

    void waitForUserInput() {
        std::cout << "Press [enter] to continue.";
        std::cin.get();
    }
};

