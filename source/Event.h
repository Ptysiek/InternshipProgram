#pragma once
#include <string>

struct Event {
    enum class Type {
        Created, 
        Deleted, 
        Edited, 
        Renamed
    };

private: 
    const Type _type;
    const std::string _fileLocation;
    const std::string _date;
    const std::string _time;

public:
    explicit Event(const Type& type, const std::string& fileLocation);

    std::string fullLog() const;
    std::string log() const;

private:
    std::string initDateTimeFormat(const std::string& format);
    std::string to_string(const Type type) const;
};
