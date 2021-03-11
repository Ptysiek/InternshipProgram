#pragma once

#include <sstream>
#include <string>


struct Event {
    enum Type {Created, Deleted, Edited, Renamed};
    
    const Type _type;
    const std::string _fileLocation;

    std::string log() const {
        std::stringstream result;
        result << "\"" << _fileLocation << "\"    [" << to_string(_type) << "]";
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

