#ifndef FILE_HPP
#define FILE_HPP

#include "fileSystemComponent.hpp"

class File : public FileSystemComponent
{
private:
    std::string content{};
    std::string fileName{};

public:
    File(std::string name) : fileName(name) {}
    virtual void setContent(const std::string &);
    std::string getContent() const;

    std::string getName() override { return fileName; }
    std::string getComponentType() override { return "File"; }
    std::time_t getTimestamp() override { return creationTime; }
};

#endif