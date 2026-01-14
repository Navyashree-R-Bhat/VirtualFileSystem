#include "directory.hpp"

void Directory::addChild(const std::shared_ptr<FileSystemComponent> &child)
{
    children[child->getName()] = child;
}

void Directory::displayChildren()
{
    for (auto &child : children)
    {
        std::cout << child.first << std::endl;
    }
}

void Directory::removeChild(const std::string &directoryName)
{
    children.erase(directoryName);
}