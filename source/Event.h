#pragma once

#include <sstream>
#include <string>


struct Event {
    enum Type {Created, Deleted, Edited, Renamed};
    
    const Type _type;
    const std::string _fileLocation;
    const bool _isFolder;

    std::string log() const {
        std::stringstream result;
        result << ((_isFolder)? "Folder " : "File ")
            << "\"" << _fileLocation << "\" "
            << "is " << to_string(_type) << ".";
        return result.str();
    }


private:
    std::string to_string(const Type type) const {
        switch(type) {
        case Created:
            return "Created";
        case Deleted:
            return "Deleted";
        case Edited:
            return "Edited";
        default: 
            return "Renamed";
        }
    }
};

