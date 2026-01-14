#include "fileSystem.hpp"

void FileSystem::createDirectory(const std::string &path)
{
    std::shared_ptr<Directory> presentWorkingDirectory = workingDirectory;
    std::istringstream pathStream(path);
    std::vector<std::string> directories;
    directories = splitPath(pathStream, '/');
    for (auto &directory : directories)
    {
        std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
        subDirectories = workingDirectory->getSubDirectories();
        auto foundDirectory = subDirectories.find(directory);
        if (foundDirectory != subDirectories.end())
        {
            workingDirectory = std::dynamic_pointer_cast<Directory>(foundDirectory->second);
        }
        else
        {
            std::shared_ptr<Directory> newDirectory = std::make_shared<Directory>(directory);
            workingDirectory->addChild(newDirectory);
            workingDirectory = newDirectory;
        }
    }
    workingDirectory = presentWorkingDirectory;
}

std::vector<std::string> FileSystem::splitPath(std::istringstream &path, const char &delimiter)
{
    std::vector<std::string> directories;
    std::string eachDirectory;
    while (std::getline(path, eachDirectory, delimiter))
    {
        directories.push_back(eachDirectory);
    }
    return directories;
}

void FileSystem::changeDirectory(const std::string &path)
{
    if (path == " ")
    {
        workingDirectory = rootDirectory;
        pathOfWorkingDirectory = "~";
        return;
    }
    else
    {
        std::istringstream pathStream(path);
        std::vector<std::string> directoriesToTraverse = splitPath(pathStream, '/');
        for (auto &directory : directoriesToTraverse)
        {
            if (directory == "..")
            {
                if (workingDirectory == rootDirectory)
                {
                    break;
                }
                moveUpDirectory();
                continue;
            }
            std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
            subDirectories = workingDirectory->getSubDirectories();
            auto foundDirectory = subDirectories.find(directory);
            if (foundDirectory != subDirectories.end() && (foundDirectory->second)->getComponentType() == "Directory")
            {
                workingDirectory = std::dynamic_pointer_cast<Directory>(foundDirectory->second);
                pathOfWorkingDirectory += ("/" + foundDirectory->first);
            }
            else if (foundDirectory == subDirectories.end() || (foundDirectory->second)->getComponentType() == "File")
            {
                std::cout << "Directory not found: " << directory << std::endl;
                break;
            }
        }
    }
}

void FileSystem::moveUpDirectory()
{
    if (workingDirectory != rootDirectory)
    {
        std::istringstream path(pathOfWorkingDirectory);
        std::vector<std::string> parentDirectories = splitPath(path, '/');
        if (!parentDirectories.empty())
        {
            parentDirectories.pop_back();
        }
        if (!parentDirectories.empty())
        {
            parentDirectories.erase(parentDirectories.begin());
        }
        auto parentDirectory = rootDirectory;
        pathOfWorkingDirectory = "~";
        if (!parentDirectories.empty())
        {
            std::string parentDirectoryName = parentDirectories.back();
            for (auto &directory : parentDirectories)
            {
                std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
                subDirectories = parentDirectory->getSubDirectories();

                auto findDirectory = subDirectories.find(directory);

                parentDirectory = std::dynamic_pointer_cast<Directory>(findDirectory->second);
                pathOfWorkingDirectory += ("/" + directory);
            }
        }
        workingDirectory = parentDirectory;
    }
}

void FileSystem::createFile(const std::string &path)
{
    std::shared_ptr<Directory> presentWorkingDirectory = workingDirectory;
    std::istringstream pathStream(path);
    std::vector<std::string> directories;
    directories = splitPath(pathStream, '/');
    std::string name;
    if (!directories.empty())
    {
        name = directories.back();
        directories.erase(directories.end());
    }
    if (!directories.empty())
    {
        for (auto &directory : directories)
        {
            std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
            subDirectories = workingDirectory->getSubDirectories();
            auto foundDirectory = subDirectories.find(directory);
            if (foundDirectory != subDirectories.end())
            {
                workingDirectory = std::dynamic_pointer_cast<Directory>(foundDirectory->second);
            }
            else
            {
                std::cout << "Directory not found:" << directory << std::endl;
                return;
            }
        }
    }
    std::shared_ptr<File> file = std::make_shared<File>(name);
    workingDirectory->addChild(file);
    workingDirectory = presentWorkingDirectory;
}

void FileSystem::writeToFile(const std::string &path)
{
    std::shared_ptr<Directory> presentWorkingDirectory = workingDirectory;
    std::istringstream pathStream(path);
    std::vector<std::string> directories;
    directories = splitPath(pathStream, '/');
    std::string fileName = directories.back();
    directories.erase(directories.end());
    if (!directories.empty())
    {
        for (auto &directory : directories)
        {
            std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
            subDirectories = workingDirectory->getSubDirectories();
            auto foundDirectory = subDirectories.find(directory);
            if (foundDirectory != subDirectories.end())
            {
                workingDirectory = std::dynamic_pointer_cast<Directory>(foundDirectory->second);
            }
            else
            {
                std::cout << "Directory not found:" << directory << std::endl;
                return;
            }
        }
    }
    std::cout << "\033[s";
    std::cout << "\033[?1049h";
    std::cout << "\033[H";
    std::cout << "\033[J";
    std::cout << "fileoperation" << std::endl;
    std::ostringstream contentsToInsertToFile;
    std::string line{};
    while (std::getline(std::cin, line) && line != "exit")
    {
        contentsToInsertToFile << line << "\n";
    }
    std::cout << "\033[?1049l";
    std::cout << "\033[u";

    std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
    subDirectories = workingDirectory->getSubDirectories();
    auto findFile = subDirectories.find(fileName);
    if (findFile != subDirectories.end() && (findFile->second)->getComponentType() == "File")
    {
        std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(findFile->second);
        file->setContent(contentsToInsertToFile.str());
    }
    else
    {
        std::cout << "File not found" << std::endl;
        return;
    }

    workingDirectory = presentWorkingDirectory;
}

void FileSystem::displayFileContent(const std::string &path)
{
    std::shared_ptr<Directory> presentWorkingDirectory = workingDirectory;
    std::istringstream pathStream(path);
    std::vector<std::string> directories;
    directories = splitPath(pathStream, '/');
    std::string fileName = directories.back();
    directories.erase(directories.end());
    if (!directories.empty())
    {
        for (auto &directory : directories)
        {
            std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
            subDirectories = workingDirectory->getSubDirectories();
            auto foundDirectory = subDirectories.find(directory);
            if (foundDirectory != subDirectories.end())
            {
                workingDirectory = std::dynamic_pointer_cast<Directory>(foundDirectory->second);
            }
            else
            {
                std::cout << "Directory not found:" << directory << std::endl;
                return;
            }
        }
    }
    std::unordered_map<std::string, std::shared_ptr<FileSystemComponent>> subDirectories;
    subDirectories = workingDirectory->getSubDirectories();
    auto findFile = subDirectories.find(fileName);
    if (findFile != subDirectories.end() && (findFile->second)->getComponentType() == "File")
    {
        std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(findFile->second);
        std::string fileContent = file->getContent();
        std::cout << "File Content\n"
                  << fileContent << std::endl;
    }
    else
    {
        std::cout << "File not found" << std::endl;
        return;
    }
    workingDirectory = presentWorkingDirectory;
}

void FileSystem::displayFiles()
{
    workingDirectory->displayChildren();
}

void FileSystem::removeDirectory(const std::string &directoryName)
{
    auto subDirectories = workingDirectory->getSubDirectories();
    auto findDirectory = subDirectories.find(directoryName);
    if (findDirectory != subDirectories.end())
    {
        auto directory = std::dynamic_pointer_cast<Directory>(findDirectory->second);
        if (directory->getSubDirectories().empty() && directory->getComponentType() == "Directory")
        {
            workingDirectory->removeChild(findDirectory->first);
        }
        else
        {
            std::cout << "Directory not empty" << std::endl;
        }
    }
    else
    {
        std::cout << "Directory not found" << std::endl;
    }
}

void FileSystem::removeFile(const std::string &fileName)
{
    auto files = workingDirectory->getSubDirectories();
    if (fileName.find("*.") != std::string::npos)
    {
        std::string extension = fileName;
        extension = extension.substr(1);
        for (auto &file : files)
        {

            if (file.second->getComponentType() == "File" && ((file.first).find(extension) != std::string::npos))
            {
                workingDirectory->removeChild(file.first);
            }
        }
    }
    else
    {
        auto findFile = files.find(fileName);
        if (findFile != files.end() && (findFile->second)->getComponentType() == "File")
        {
            auto file = std::dynamic_pointer_cast<File>(findFile->second);
            workingDirectory->removeChild(fileName);
        }
        else
        {
            std::cout << "File not found" << std::endl;
        }
    }
}

void FileSystem::fileFound(const std::string &fileName, const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &fileComponent, std::string currentPath)
{
    if (fileComponent.second->getComponentType() == "File" && fileComponent.first == fileName)
    {
        std::cout << currentPath << "/" << fileComponent.first << std::endl;
        return;
    }
    else if (fileComponent.second->getComponentType() == "Directory")
    {
        std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(fileComponent.second);
        auto files = directory->getSubDirectories();
        for (auto &file : files)
        {
            std::string updatedPath = currentPath + "/" + directory->getName();
            fileFound(fileName, std::make_pair(file.first, file.second), updatedPath);
        }
    }
}

void FileSystem::findByTime(const int &range, const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &fileComponent, std::string currentPath)
{
    std::time_t timestamp = (fileComponent.second)->getTimestamp();
    auto currentTimestamp = std::chrono::system_clock::now();
    auto fileTimestamp = std::chrono::system_clock::from_time_t(timestamp);

    if (fileComponent.second->getComponentType() == "File")
    {
        auto difference = std::chrono::duration_cast<std::chrono::seconds>(currentTimestamp - fileTimestamp);
        int differenceInSeconds = difference.count();
        if (differenceInSeconds <= range)
        {
            std::cout << currentPath << "/" << fileComponent.first << std::endl;
            return;
        }
    }
    else if (fileComponent.second->getComponentType() == "Directory")
    {
        std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(fileComponent.second);
        auto files = directory->getSubDirectories();
        for (auto &file : files)
        {
            std::string updatedPath = currentPath + "/" + directory->getName();
            findByTime(range, std::make_pair(file.first, file.second), updatedPath);
        }
    }
}

void FileSystem::findByContent(const std::string &content, const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &fileComponent, std::string currentPath)
{
    if (fileComponent.second->getComponentType() == "File")
    {
        std::shared_ptr<File> file = std::dynamic_pointer_cast<File>(fileComponent.second);
        if ((file->getContent()).find(content) != std::string::npos)
        {
            std::cout << currentPath << "/" << fileComponent.first << std::endl;
            return;
        }
    }
    else if (fileComponent.second->getComponentType() == "Directory")
    {
        std::shared_ptr<Directory> directory = std::dynamic_pointer_cast<Directory>(fileComponent.second);
        auto files = directory->getSubDirectories();
        for (auto &file : files)
        {
            std::string updatedPath = currentPath + "/" + directory->getName();
            findByContent(content, std::make_pair(file.first, file.second), updatedPath);
        }
    }
}

void FileSystem::findFile(const std::string &arguments)
{
    std::string path;
    std::string argument;
    std::string option;
    std::istringstream argumentStream(arguments);
    argumentStream >> option;
    argumentStream >> path;
    if (argumentStream.peek() != EOF)
    {
        argumentStream >> argument;
    }
    else
    {
        argument = path;
        path = " ";
    }
    if (path != " ")
    {
        changeDirectory(path);
    }
    auto files = workingDirectory->getSubDirectories();
    if (option == "-file")
    {
        for (auto &file : files)
        {
            fileFound(argument, std::make_pair(file.first, file.second), pathOfWorkingDirectory);
        }
    }
    else if (option == "-time")
    {
        for (auto &file : files)
        {
            int range = std::stoi(argument);
            findByTime(range, std::make_pair(file.first, file.second), pathOfWorkingDirectory);
        }
    }
    else if (option == "-content")
    {
        for (auto &file : files)
        {
            findByContent(argument, std::make_pair(file.first, file.second), pathOfWorkingDirectory);
        }
    }
}
