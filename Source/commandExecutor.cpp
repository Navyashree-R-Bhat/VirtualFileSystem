#include "commandExecutor.hpp"

void CommandExecutor::handlecd()
{
    fileSystemObject->changeDirectory(arguments);
}

void CommandExecutor::handlecat()
{
    fileSystemObject->displayFileContent(arguments);
}

void CommandExecutor::handlemkdir()
{
    fileSystemObject->createDirectory(arguments);
}

void CommandExecutor::handletouch()
{
    fileSystemObject->createFile(arguments);
}

void CommandExecutor::handleecho()
{
    fileSystemObject->writeToFile(arguments);
}

void CommandExecutor::handlels()
{
    fileSystemObject->displayFiles();
}

void CommandExecutor::handlermdir()
{
    fileSystemObject->removeDirectory(arguments);
}

void CommandExecutor::handlermfile()
{
    fileSystemObject->removeFile(arguments);
}

void CommandExecutor::handlefindFile()
{
    fileSystemObject->findFile(arguments);
}

bool CommandExecutor::isCommandName(const std::string &commandName)
{
    auto foundCommand = commandMap.find(commandName);
    if (foundCommand != commandMap.end())
    {
        return true;
    }
    return false;
}

void CommandExecutor::separateCommandLine(const std::string &readCommandLine)
{
    arguments = " ";
    std::istringstream commandLine(readCommandLine);
    commandLine >> command;
    if (!commandLine.fail())
    {
        std::getline(commandLine >> std::ws, arguments);
    }
}

void CommandExecutor::readCommandLine()
{
    std::cout << fileSystemObject->getPathOfWorkingDirectory() << "$ ";
    while (std::getline(std::cin, commandLine))
    {
        separateCommandLine(commandLine);
        if (command == "help")
        {
            for (auto &command : commandMap)
            {
                std::cout << command.first << std::endl;
            }
        }
        else if (command == "exit")
        {
            break;
        }
        else if (isCommandName(command))
        {
            auto foundCommand = commandMap.find(command);
            foundCommand->second();
        }
        else
        {
            std::cout << "Command not found" << std::endl;
        }
        std::cout << fileSystemObject->getPathOfWorkingDirectory() << "$ ";
    }
}