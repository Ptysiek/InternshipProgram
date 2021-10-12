#pragma once

#include <exception>
// <FILESYSTEM>
// Neither GCC(7.5 - 2019) nor Clang(6.0.0 - 2018) has a <filesystem> yet.
// Needs to be linked with static library: GCC[-lstdc++fs] or LLVM/Clang[-lc++fs].
// Or You can use GCC(9.0 - 2020)
#include <filesystem>  
#include <string>

#include "InputOutput.h"

class TargetPathManager {
    const InputOutput& _inputOutput;
	const std::filesystem::directory_entry _targetEntry;

public:
	explicit TargetPathManager(const InputOutput& inputOutput, const int argc, const char* const * const argv);

	std::string build() const;
	bool validate() const;

private:
	std::filesystem::directory_entry argToDirectoryEntry(const int argc, const char* const * const argv) const;
};
