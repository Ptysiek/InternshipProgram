#include "File.h"

File::File(const std::filesystem::directory_entry& meta, const std::string& data):
    _modificationTime(std::filesystem::last_write_time(meta)),
    _name(meta.path().c_str()),
    _data(data)
{}

std::filesystem::file_time_type File::modificationTime() const { 
    return _modificationTime; 
}

std::string File::name() const { 
    return _name; 
} 

std::string File::data() const { 
    return _data; 
}
