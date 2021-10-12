#include "Event.h"

#include <ctime>
#include <iomanip>
#include <sstream>

Event::Event(const Type& type, const std::string& fileLocation):
    _type(type),
    _fileLocation(fileLocation),
    _date(initDateTimeFormat("%d.%m.%Y")),
    _time(initDateTimeFormat("%H:%M:%S"))
{}

std::string Event::fullLog() const {
    std::stringstream result;
    result << "Date: [" << _date << "]   "
        << "Time: [" << _time << "]   "
        << log(); 
    return result.str();
}

std::string Event::log() const {
    std::stringstream result;
    result << "[" << to_string(_type) << "]\t" 
        << "\"" << _fileLocation << "\""; 
    return result.str();
}

std::string Event::initDateTimeFormat(const std::string& format) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

std::string Event::to_string(const Type type) const {
    switch(type) {
    case Type::Created:
        return "Created";
    case Type::Deleted:
        return "Deleted";
    case Type::Edited:
        return "Edited";
    default: 
        return "Renamed";
    }
}
