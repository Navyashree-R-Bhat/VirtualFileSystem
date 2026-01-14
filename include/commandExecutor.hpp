#ifndef COMMANDEXECUTOR_HPP
#define COMMANDEXECUTOR_HPP

#include <algorithm>

#include "fileSystem.hpp"

class CommandExecutor
{
public:
    using CommandFunction = std::function<void(void)>;
    CommandExecutor()
    {
        commandMap["cd"] = std::bind(&CommandExecutor::handlecd, this);
        commandMap["mkdir"] = std::bind(&CommandExecutor::handlemkdir, this);
        commandMap["cat"] = std::bind(&CommandExecutor::handlecat, this);
        commandMap["touch"] = std::bind(&CommandExecutor::handletouch, this);
        commandMap["echo"] = std::bind(&CommandExecutor::handleecho, this);
        commandMap["ls"] = std::bind(&CommandExecutor::handlels, this);
        commandMap["rmdir"] = std::bind(&CommandExecutor::handlermdir, this);
        commandMap["rm"] = std::bind(&CommandExecutor::handlermfile, this);
        commandMap["find"] = std::bind(&CommandExecutor::handlefindFile, this);
    }
    CommandExecutor(std::shared_ptr<FileSystem> fileSystem)
    {
        fileSystemObject = fileSystem;
        commandMap["cd"] = std::bind(&CommandExecutor::handlecd, this);
        commandMap["mkdir"] = std::bind(&CommandExecutor::handlemkdir, this);
        commandMap["cat"] = std::bind(&CommandExecutor::handlecat, this);
        commandMap["touch"] = std::bind(&CommandExecutor::handletouch, this);
        commandMap["echo"] = std::bind(&CommandExecutor::handleecho, this);
        commandMap["ls"] = std::bind(&CommandExecutor::handlels, this);
        commandMap["rmdir"] = std::bind(&CommandExecutor::handlermdir, this);
        commandMap["rm"] = std::bind(&CommandExecutor::handlermfile, this);
        commandMap["find"] = std::bind(&CommandExecutor::handlefindFile, this);
    }
    ~CommandExecutor() {}

    virtual bool isCommandName(const std::string &commandName);
    virtual void separateCommandLine(const std::string &readCommandLine);
    void readCommandLine();
    virtual void handlecd();
    virtual void handlemkdir();
    virtual void handlecat();
    virtual void handletouch();
    virtual void handleecho();
    virtual void handlels();
    virtual void handlermdir();
    virtual void handlermfile();
    virtual void handlefindFile();

private:
    std::shared_ptr<FileSystem> fileSystemObject = std::make_shared<FileSystem>();
    std::vector<std::string> listOfCommands;
    std::string command;
    std::string arguments;
    std::string commandLine;
    std::unordered_map<std::string, CommandFunction> commandMap;
};

#endif