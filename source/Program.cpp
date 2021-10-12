#include "Program.h"
#include "TargetPathManager.h"

Program::Program(const int argc, const char* const * const argv):
    _filesState(),
    _inputOutput("logs.txt"),
    _targetManager(_inputOutput, argc, argv),
    _targetPath(_targetManager.build())
{}

void Program::execute() {
    if (!_targetManager.validate()) {
        _inputOutput.waitForUserInput();
        return;
    }
    _inputOutput.writeToLog("\nProgram Init. Target Folder: \"" + _targetPath + "\"");
    _filesState = captureFilesState();
    mainLogicLoop();
    return;
}

void Program::mainLogicLoop() {
    while (true) {
        auto latestFilesState = tryCaptureFilesState();
        if (latestFilesState.empty()) {
            continue;
        }
        auto newFiles = enlistCreatedFiles(latestFilesState);
        for (auto& [fileName, file] : _filesState) {
            const auto i = latestFilesState.find(fileName);
            if (i == latestFilesState.end()) {                
                processIfFileDeletedOrRenamed(newFiles, file);
            }
            else if (i->second.modificationTime() != file.modificationTime()) {
                _inputOutput.writeToLogAndUser(Event(Event::Type::Edited, file.name()));                
            }
        }
        for (const auto& file : newFiles) {
            _inputOutput.writeToLogAndUser(Event(Event::Type::Created, file.name()));
        }
        std::swap(_filesState, latestFilesState);
    }
}

void Program::processIfFileDeletedOrRenamed(std::vector<File>& newFiles, const File& file) {
    auto j = findFileWithEqualData(newFiles, file);
    if (j == newFiles.end()) {
        _inputOutput.writeToLogAndUser(Event(Event::Type::Deleted, file.name()));                    
    }
    else {
        _inputOutput.writeToLogAndUser(Event(Event::Type::Renamed, file.name()));
        *j = newFiles.back();
        newFiles.pop_back();
    }
}

Program::FilesState Program::captureFilesState() const {
    FilesState result;
    auto paths = _inputOutput.readPaths(_targetPath);
    for (const auto& path : paths) {
        const std::filesystem::directory_entry meta(_targetPath + path);
        const auto data = _inputOutput.readFile(_targetPath + path);
        result.insert({path, File(meta, data)}); 
    }
    return result;
}

Program::FilesState Program::tryCaptureFilesState() const {
    try {
        return captureFilesState();
    } 
    catch(const std::exception& e) {
        _inputOutput.writeToLog(e.what(), "Warning:  ");
    }
    return {};
}

std::vector<File>::iterator Program::findFileWithEqualData(std::vector<File>& newFiles, const File& file) const {
    auto compareFileData = [&file](const auto& newFile) { 
        return (newFile.data() == file.data());
    }; 
    return std::find_if(newFiles.begin(), newFiles.end(), compareFileData);
}

std::vector<File> Program::enlistCreatedFiles(const FilesState& latestFilesState) const {
    std::vector<File> result;
    for (auto& [key, file] : latestFilesState) {
        if (_filesState.find(key) == _filesState.end()) {
            result.emplace_back(file);
        }
    }
    return result;
}
