#ifndef FILESYSTEMCOMPONENT_HPP
#define FILESYSTEMCOMPONENT_HPP

#include <iostream>
#include <memory>
#include <ctime>

class FileSystemComponent
{
protected:
    std::time_t creationTime = std::time(nullptr);

public:
    FileSystemComponent() { std::ctime(&creationTime); }
    virtual std::string getName() = 0;
    virtual std::string getComponentType() = 0;
    virtual std::time_t getTimestamp() = 0;
    virtual ~FileSystemComponent() {}
};

#endif