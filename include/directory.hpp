#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <unordered_map>
#include <memory>
#include "fileSystemComponent.hpp"

class Directory : public FileSystemComponent
{
private:
    std::string directoryName;
    std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> children;

public:
    Directory(std::string name) : directoryName(name) {}

    std::string getName() override { return directoryName; }
    std::string getComponentType() override { return "Directory"; }
    virtual void addChild(const std::shared_ptr<FileSystemComponent> &child);
    std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> getSubDirectories() { return children; }
    virtual void removeChild(const std::string &directoryName);
    void displayChildren();
    std::time_t getTimestamp() override { return creationTime; }
};

#endif