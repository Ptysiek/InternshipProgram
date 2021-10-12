#pragma once

#include <algorithm>
#include <exception>
// <FILESYSTEM>
// Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
// Needs to be linked with static library: GCC[-lstdc++fs] or LLVM/Clang[-lc++fs].
// Or You can use GCC(9.0 - 2020)
#include <filesystem>  
#include <map>
#include <string>
#include <vector>

#include "Event.h"
#include "File.h"
#include "InputOutput.h"

#include "TargetPathManager.h"

class Program {
    using FilesState = std::map<std::string, File>;

    FilesState _filesState;
    const InputOutput _inputOutput;
    const TargetPathManager _targetManager;
    const std::string _targetPath;

public:
    explicit Program(const int argc, const char* const * const argv);
    
    void execute();

private:
    void mainLogicLoop();
    void processIfFileDeletedOrRenamed(std::vector<File>& newFiles, const File& file);
    FilesState captureFilesState() const;    
    FilesState tryCaptureFilesState() const;    
    std::vector<File>::iterator findFileWithEqualData(std::vector<File>& newFiles, const File& file) const;
    std::vector<File> enlistCreatedFiles(const FilesState& latestFilesState) const;
};

