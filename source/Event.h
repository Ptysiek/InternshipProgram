#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>


struct Event {
    enum Type {Created, Deleted, Edited, Renamed};


private: 
    const Type _type;
    const std::string _fileLocation;
    const std::string _date;
    const std::string _time;


public:
    Event(const Type& type, const std::string& fileLocation):
        _type(type),
        _fileLocation(fileLocation),
        _date(initDateTimeFormat("%d.%m.%Y")),
        _time(initDateTimeFormat("%H:%M:%S"))
    {}
    
    std::string fullLog() const {
        std::stringstream result;
        result << "Date: [" << _date << "]   "
            << "Time: [" << _time << "]   "
            << log(); 
        return result.str();
    }

    std::string log() const {
        std::stringstream result;
        result << "[" << to_string(_type) << "]\t" 
            << "\"" << _fileLocation << "\""; 
        return result.str();
    }


private:
    std::string initDateTimeFormat(const std::string& format) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, format.c_str());
        return oss.str();
    }

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

