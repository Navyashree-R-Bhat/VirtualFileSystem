#include <iostream>
#include <memory>

#include "commandExecutor.hpp"

int main()
{
    CommandExecutor *commandExecutorObject = new CommandExecutor();
    commandExecutorObject->readCommandLine();
    
    delete commandExecutorObject;
}