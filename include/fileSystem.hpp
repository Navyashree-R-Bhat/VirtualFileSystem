#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <memory>
#include <sstream>
#include <vector>
#include <chrono>

#include "directory.hpp"
#include "file.hpp"

class FileSystem
{
private:
    std::shared_ptr<Directory> rootDirectory;
    std::string pathOfWorkingDirectory;
    std::shared_ptr<Directory> workingDirectory;

public:
    FileSystem()
    {
        rootDirectory = std::make_shared<Directory>("root");
        workingDirectory = rootDirectory;
        pathOfWorkingDirectory = "~";
    }
    virtual ~FileSystem() = default;

    virtual void createDirectory(const std::string &);
    virtual void changeDirectory(const std::string &path = " ");
    virtual void moveUpDirectory();
    virtual std::vector<std::string> splitPath(std::istringstream &path, const char &delimiter);
    virtual void createFile(const std::string &);
    virtual void writeToFile(const std::string &);
    virtual void displayFileContent(const std::string &);
    virtual std::string getPathOfWorkingDirectory() const { return pathOfWorkingDirectory; }
    virtual void displayFiles();
    virtual void removeDirectory(const std::string &);
    virtual void removeFile(const std::string &);
    virtual void findFile(const std::string &);
    virtual void fileFound(const std::string &, const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &, std::string);
    virtual void findByTime(const int &, const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &, std::string);
    virtual void findByContent(const std::string &, const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &, std::string);

    void setWorkingDirectory(const std::shared_ptr<Directory> &directory)
    {
        workingDirectory = directory;
    }
};

#endif