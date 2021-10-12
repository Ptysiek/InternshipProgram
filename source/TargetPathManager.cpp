#include "TargetPathManager.h"

TargetPathManager::TargetPathManager(const InputOutput& inputOutput, const int argc, const char* const * const argv):
	_inputOutput(inputOutput),
	_targetEntry(argToDirectoryEntry(argc, argv))	
{}

std::string TargetPathManager::build() const {
	std::string result = _targetEntry.path().c_str();
	return (result.back() == '/')? result : result + '/';
}

bool TargetPathManager::validate() const {
	namespace fs = std::filesystem;

	if (_targetEntry == fs::directory_entry()) {
		_inputOutput.writeToUser("No argument was given.");
		return false;
	}
	const auto status = _targetEntry.status();
	const auto targetPath = build();
	if (!fs::status_known(status)) {
		throw std::logic_error("ERROR AT: Program::validateTarget: Unknown Status.");
	}
	if (!fs::exists(status)) {
		_inputOutput.writeToUser("Specified path does not exist. \"" + targetPath + "\""); 
		return false;
	}
	if (!fs::is_directory(status)) {
		_inputOutput.writeToUser("Specified path is not a directory. \"" + targetPath + "\"");  
		return false;
	}
	return true;
}

std::filesystem::directory_entry TargetPathManager::argToDirectoryEntry(const int argc, const char* const * const argv) const {
	if (argc > 1) {
		return std::filesystem::directory_entry(argv[1]);
	}
	return {};  
}
