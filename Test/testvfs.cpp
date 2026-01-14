#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

#include "commandExecutor.hpp"

class TestFileClass : public ::testing::Test
{
public:
    File *file;

    void SetUp() override
    {
        file = new File("sampleFile");
    }
    void TearDown() override
    {
        delete file;
    }
};

TEST_F(TestFileClass, getNameFunctionTest)
{
    EXPECT_EQ("sampleFile", file->getName());
}

TEST_F(TestFileClass, setContentFunction)
{
    std::string inputContent = "This is a sample example. \nLine is inserted for testing.\n";
    file->setContent(inputContent);
    EXPECT_EQ(inputContent, file->getContent());
}

TEST_F(TestFileClass, emptyContent)
{
    EXPECT_EQ("", file->getContent());
}

TEST_F(TestFileClass, concatenatingContent)
{
    std::string inputContent = "This is a sample example. \nLine is inserted for testing.\n";
    file->setContent(inputContent);
    std::string content = inputContent;
    inputContent = "This is another line to check concatenation. \nThis is concatenated line. \n";
    file->setContent(inputContent);
    content += inputContent;
    EXPECT_EQ(content, file->getContent());
}

TEST_F(TestFileClass, getComponentTypeFunctionTest)
{
    EXPECT_EQ("File", file->getComponentType());
}

TEST_F(TestFileClass, getTimestampFunctionTest)
{
    auto currentTime = std::chrono::system_clock::now();
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(currentTime.time_since_epoch());
    auto minutesCount = minutes.count();

    auto timestamp = std::chrono::system_clock::from_time_t(file->getTimestamp());
    auto fileMinutes = std::chrono::duration_cast<std::chrono::minutes>(timestamp.time_since_epoch());
    auto fileminutesCount = fileMinutes.count();

    EXPECT_EQ(minutesCount, fileminutesCount);
}

class TestDirectoryClass : public ::testing::Test
{
public:
    Directory *directory;

    void SetUp() override
    {
        directory = new Directory("sampleDirectory");
    }

    void TearDown() override
    {
        delete directory;
    }
};

TEST_F(TestDirectoryClass, getNameFunctionTest)
{
    EXPECT_EQ("sampleDirectory", directory->getName());
}

TEST_F(TestDirectoryClass, getComponentTypeFunctionTest)
{
    EXPECT_EQ("Directory", directory->getComponentType());
}

TEST_F(TestDirectoryClass, checkEmptyChildrenMap)
{
    EXPECT_TRUE(directory->getSubDirectories().empty());
}

TEST_F(TestDirectoryClass, addSubDirectoryFunctionTest)
{
    std::shared_ptr<Directory> subDirectory = std::make_shared<Directory>("subDir");
    directory->addChild(subDirectory);
    EXPECT_FALSE(directory->getSubDirectories().empty());
}

TEST_F(TestDirectoryClass, addFileFunctionTest)
{
    std::shared_ptr<File> file = std::make_shared<File>("sampleFile");
    directory->addChild(file);
    EXPECT_FALSE(directory->getSubDirectories().empty());
}

TEST_F(TestDirectoryClass, removeChildFunctionTest)
{
    std::shared_ptr<Directory> subDirectory = std::make_shared<Directory>("subDir");
    directory->addChild(subDirectory);
    ASSERT_FALSE(directory->getSubDirectories().empty());
    directory->removeChild("subDir");
    EXPECT_TRUE(directory->getSubDirectories().empty());
}

TEST_F(TestDirectoryClass, removeEmptyChildFunctionTest)
{
    directory->removeChild("subDir");
    EXPECT_TRUE(directory->getSubDirectories().empty());
}

class TestFileSystem : public ::testing::Test
{
public:
    FileSystem *fileSystemObject;
    void SetUp() override
    {
        fileSystemObject = new FileSystem();
    }
    void TearDown() override
    {
        delete fileSystemObject;
    }
};

TEST_F(TestFileSystem, initializationCheck)
{
    ASSERT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~");
}

class MockFileSystemCreatingDirectory : public FileSystem
{
public:
    MockFileSystemCreatingDirectory() : FileSystem() {}
    MOCK_METHOD(std::vector<std::string>, splitPath, (std::istringstream &, const char &), (override));
    // MOCK_METHOD(void, moveUpDirectory, (), (override));
};

TEST_F(TestFileSystem, creatingDirectoryFunctionTest)
{
    MockFileSystemCreatingDirectory mockdirectory;

    EXPECT_CALL(mockdirectory, splitPath(_, _)).Times(1);
    mockdirectory.createDirectory("subDirectory");
}

TEST_F(TestFileSystem, creatingDirectoryWithPathFunctionTest)
{
    MockFileSystemCreatingDirectory mockdirectory;

    EXPECT_CALL(mockdirectory, splitPath(_, _)).Times(AtLeast(1));
    mockdirectory.createDirectory("subDirectory/sampleDirectory");
}

class TestFileSystemChangeDirectory : public ::testing::Test
{
public:
    FileSystem *fileSystemObject;
    std::stringstream mockcout;
    std::streambuf *realcout;
    void SetUp() override
    {
        fileSystemObject = new FileSystem();
        fileSystemObject->createDirectory("sub1");
        fileSystemObject->createDirectory("sub1/sub2");
        realcout = std::cout.rdbuf(mockcout.rdbuf());
    }
    void TearDown() override
    {
        std::cout.rdbuf(realcout);
        delete fileSystemObject;
    }
};

TEST_F(TestFileSystemChangeDirectory, changeDirectoryFunctionWithEmptyPath)
{
    fileSystemObject->changeDirectory("sub1/sub2");
    fileSystemObject->changeDirectory(" ");
    EXPECT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~");
}

TEST_F(TestFileSystemChangeDirectory, changeDirectoryFunctionWithOneDirectory)
{
    fileSystemObject->changeDirectory("sub1");
    EXPECT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~/sub1");
}

TEST_F(TestFileSystemChangeDirectory, changeDirectoryOneDirectoryPathCheckCallToSplitPath)
{
    MockFileSystemCreatingDirectory mockDirectory;
    mockDirectory.createDirectory("sub1");

    EXPECT_CALL(mockDirectory, splitPath(_, _)).Times(1);
    mockDirectory.changeDirectory("sub1");
}

TEST_F(TestFileSystemChangeDirectory, changeDirectoryFunctionWIthMultipleDirectories)
{
    fileSystemObject->changeDirectory("sub1/sub2");
    EXPECT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~/sub1/sub2");
}

TEST_F(TestFileSystemChangeDirectory, changeDirectoryFunctionNoDirectoryFound)
{
    fileSystemObject->changeDirectory("sub1/sub3");
    ASSERT_TRUE(mockcout.str().find("Directory not found: ") != std::string::npos);
}

TEST_F(TestFileSystemChangeDirectory, changeDirectoryMoveOneDirectoryUp)
{
    fileSystemObject->changeDirectory("sub1/sub2");
    fileSystemObject->changeDirectory("..");
    EXPECT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~/sub1");
}

TEST_F(TestFileSystemChangeDirectory, changeDirectoryPathMoveUpDirectory)
{
    fileSystemObject->changeDirectory("sub1/sub2");
    fileSystemObject->createDirectory("sub3");
    fileSystemObject->changeDirectory("sub3");
    fileSystemObject->changeDirectory("../..");
    EXPECT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~/sub1");
}

TEST_F(TestFileSystemChangeDirectory, moveUpOneDirectoryFunctionTest)
{
    fileSystemObject->changeDirectory("sub1/sub2");
    fileSystemObject->moveUpDirectory();
    EXPECT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~/sub1");
}

TEST_F(TestFileSystemChangeDirectory, moveUpFromRootDirectoryNoEffect)
{
    fileSystemObject->changeDirectory(" ");
    fileSystemObject->moveUpDirectory();
    EXPECT_EQ(fileSystemObject->getPathOfWorkingDirectory(), "~");
}

class TestFileSystemFile : public ::testing::Test
{
public:
    FileSystem *fileSystemObject;
    std::stringstream mockcin;
    std::stringstream mockcout;
    std::streambuf *realcin;
    std::streambuf *realcout;

    void SetUp() override
    {
        realcin = std::cin.rdbuf(mockcin.rdbuf());
        realcout = std::cout.rdbuf(mockcout.rdbuf());
        fileSystemObject = new FileSystem();
    }
    void TearDown() override
    {
        std::cin.rdbuf(realcin);
        std::cout.rdbuf(realcout);
        delete fileSystemObject;
    }
};

class MockFileSystemCreatingFile : public FileSystem
{
public:
    MockFileSystemCreatingFile() : FileSystem() {}
    MOCK_METHOD(std::vector<std::string>, splitPath, (std::istringstream &, const char &), (override));
};

TEST_F(TestFileSystemFile, creatingFileCallToSplitPath)
{
    MockFileSystemCreatingFile creatingFile;

    EXPECT_CALL(creatingFile, splitPath(_, _)).Times(1);
    creatingFile.createFile("file1");
}

TEST_F(TestFileSystemFile, creatingFileWithinSubDirectory)
{
    MockFileSystemCreatingFile creatingFile;
    EXPECT_CALL(creatingFile, splitPath(_, _)).Times(1);
    creatingFile.createFile("sub1/file1");
}

TEST_F(TestFileSystemFile, creatingFileInUnknownDirectory)
{

    fileSystemObject->createFile("randomDirectory/file1");

    ASSERT_TRUE(mockcout.str().find("Directory not found:") != std::string::npos);
}

class MockDirectory : public Directory
{
public:
    MockDirectory(std::string name) : Directory(name) {}
    MOCK_METHOD(void, addChild, (const std::shared_ptr<FileSystemComponent> &), (override));
    MOCK_METHOD(void, removeChild, (const std::string &), (override));
};

TEST_F(TestFileSystemFile, callToChildFunction)
{
    std::shared_ptr<MockDirectory> mockDirectory = std::make_shared<MockDirectory>("root");
    fileSystemObject->setWorkingDirectory(mockDirectory);

    EXPECT_CALL(*mockDirectory, addChild(_)).Times(AtLeast(1));

    fileSystemObject->createFile("file1");
}

TEST_F(TestFileSystem, callToChildFunctionForDirectory)
{
    std::shared_ptr<MockDirectory> mockDirectory = std::make_shared<MockDirectory>("root");
    fileSystemObject->setWorkingDirectory(mockDirectory);

    EXPECT_CALL(*mockDirectory, addChild(_)).Times(AtLeast(1));

    fileSystemObject->createDirectory("sub1");
}

TEST_F(TestFileSystemFile, setContentFunctionTest)
{
    fileSystemObject->createFile("example.txt");
    fileSystemObject->writeToFile("example.txt");
    mockcin << "This is a sample testing statement.\nexit\n";
    fileSystemObject->displayFileContent("example.txt");
    EXPECT_TRUE(mockcout.str().find("File Content") != std::string::npos);
}

TEST_F(TestFileSystemFile, setContentForNonExistingFile)
{
    fileSystemObject->writeToFile("example.txt");
    mockcin << "This is a sample testing statement.\nexit\n";
    EXPECT_TRUE(mockcout.str().find("File not found") != std::string::npos);
}

TEST_F(TestFileSystemFile, displayContentForNonExistingFile)
{
    fileSystemObject->displayFileContent("example.txt");
    EXPECT_TRUE(mockcout.str().find("File not found") != std::string::npos);
}

TEST_F(TestFileSystemFile, displaySubDirectories)
{
    fileSystemObject->createDirectory("dir1");
    fileSystemObject->createFile("file1");
    fileSystemObject->createDirectory("dir2");

    fileSystemObject->displayFiles();
    EXPECT_TRUE(mockcout.str().find("dir2\nfile1\ndir1\n") != std::string::npos);
}

TEST_F(TestFileSystemFile, callToRemoveChildFunction)
{
    fileSystemObject->setWorkingDirectory(std::make_shared<Directory>("someDirectory"));

    fileSystemObject->createDirectory("sub1");
    fileSystemObject->createDirectory("sub2");

    fileSystemObject->removeDirectory("sub1");

    fileSystemObject->displayFiles();
    EXPECT_FALSE(mockcout.str().find("sub1") != std::string::npos);
}

TEST_F(TestFileSystemFile, callToRemoveNonExistingDirectory)
{
    fileSystemObject->setWorkingDirectory(std::make_shared<Directory>("someDirectory"));

    fileSystemObject->createDirectory("sub1");

    fileSystemObject->removeDirectory("sub2");

    fileSystemObject->displayFiles();
    EXPECT_TRUE(mockcout.str().find("Directory not found") != std::string::npos);
}

TEST_F(TestFileSystemFile, callToRemoveNonEmptyFile)
{
    fileSystemObject->setWorkingDirectory(std::make_shared<Directory>("someDirectory"));

    fileSystemObject->createDirectory("sub1");
    fileSystemObject->createDirectory("sub1/sub2");

    fileSystemObject->removeDirectory("sub1");

    fileSystemObject->displayFiles();
    EXPECT_TRUE(mockcout.str().find("Directory not empty") != std::string::npos);
}

TEST_F(TestFileSystemFile, callToRemoveAllInputFileExtension)
{
    fileSystemObject->createFile("file1.txt");
    fileSystemObject->createFile("file2.txt");
    fileSystemObject->createFile("file3");

    fileSystemObject->removeFile("*.txt");
    fileSystemObject->displayFiles();
    ASSERT_FALSE(mockcout.str().find("file1.txt") != std::string::npos);
    ASSERT_FALSE(mockcout.str().find("file2.txt") != std::string::npos);
}

TEST_F(TestFileSystemFile, callToRemoveParticularFile)
{
    fileSystemObject->createFile("file1.txt");
    fileSystemObject->createFile("file2.txt");
    fileSystemObject->createDirectory("dir1");

    fileSystemObject->removeFile("file1.txt");
    fileSystemObject->displayFiles();
    ASSERT_FALSE(mockcout.str().find("file1.txt") != std::string::npos);
}

TEST_F(TestFileSystemFile, callToRemoveNonExistingFile)
{
    fileSystemObject->createFile("file1.txt");
    fileSystemObject->createDirectory("dir1");
    fileSystemObject->removeFile("file2.txt");
    fileSystemObject->displayFiles();
    ASSERT_FALSE(mockcout.str().find("file2.txt") != std::string::npos);
}

class MockFileSystemFindingFile : public FileSystem
{
public:
    MOCK_METHOD(void, fileFound, (const std::string &, (const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &), std::string), (override));
    MOCK_METHOD(void, findByTime, (const int &, (const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &), std::string), (override));
    MOCK_METHOD(void, findByContent, (const std::string &, (const std::pair<std::string, std::shared_ptr<FileSystemComponent>> &), std::string), (override));
};

TEST_F(TestFileSystemFile, findFileByFileName)
{
    MockFileSystemFindingFile findingFile;
    findingFile.createFile("file1");
    findingFile.createFile("file2");

    EXPECT_CALL(findingFile, fileFound(_, _, _)).Times(AtLeast(1));

    findingFile.findFile("-file file1");
}

TEST_F(TestFileSystemFile, findFileByTimestamp)
{
    MockFileSystemFindingFile findingFile;
    findingFile.createFile("file1");
    findingFile.createFile("file2");

    EXPECT_CALL(findingFile, findByTime(_, _, _)).Times(AtLeast(1));

    findingFile.findFile("-time 100");
}

TEST_F(TestFileSystemFile, findFileByContent)
{
    MockFileSystemFindingFile findingFile;
    findingFile.createFile("file1");
    findingFile.createFile("file2");

    EXPECT_CALL(findingFile, findByContent(_, _, _)).Times(AtLeast(1));

    findingFile.findFile("-content Sample File Testing");
}

class MockFileSystemFunctionCalls : public FileSystem
{
public:
    MOCK_METHOD(void, changeDirectory, (const std::string &), (override));
    MOCK_METHOD(void, createDirectory, (const std::string &), (override));
    MOCK_METHOD(void, displayFileContent, (const std::string &), (override));
    MOCK_METHOD(void, createFile, (const std::string &), (override));
    MOCK_METHOD(void, writeToFile, (const std::string &), (override));
    MOCK_METHOD(void, displayFiles, (), (override));
    MOCK_METHOD(void, removeDirectory, (const std::string &), (override));
    MOCK_METHOD(void, removeFile, (const std::string &), (override));
    MOCK_METHOD(void, findFile, (const std::string &), (override));
};

class TestCommandExecutorClass : public ::testing::Test
{
public:
    CommandExecutor *commandExecutor;
    std::shared_ptr<MockFileSystemFunctionCalls> mockFileSystem;

    void SetUp() override
    {
        mockFileSystem = std::make_shared<MockFileSystemFunctionCalls>();
        commandExecutor = new CommandExecutor(mockFileSystem);
    }
    void TearDown() override
    {
        if (commandExecutor != nullptr)
            delete commandExecutor;
    }
};

TEST_F(TestCommandExecutorClass, TestChangeDirectoryCalls)
{
    EXPECT_CALL(*mockFileSystem, changeDirectory(_)).Times(1);
    commandExecutor->handlecd();
}

TEST_F(TestCommandExecutorClass, TestCreateDirectoryCalls)
{
    EXPECT_CALL(*mockFileSystem, createDirectory(_)).Times(1);
    commandExecutor->handlemkdir();
}

TEST_F(TestCommandExecutorClass, TestcatCommand)
{
    EXPECT_CALL(*mockFileSystem, displayFileContent(_)).Times(1);
    commandExecutor->handlecat();
}

TEST_F(TestCommandExecutorClass, TesttouchCommand)
{

    EXPECT_CALL(*mockFileSystem, createFile(_)).Times(1);
    commandExecutor->handletouch();
}

TEST_F(TestCommandExecutorClass, TestechoCommand)
{
    EXPECT_CALL(*mockFileSystem, writeToFile(_)).Times(1);
    commandExecutor->handleecho();
}

TEST_F(TestCommandExecutorClass, TestlsCommand)
{
    EXPECT_CALL(*mockFileSystem, displayFiles()).Times(1);
    commandExecutor->handlels();
}

TEST_F(TestCommandExecutorClass, TestrmdirCommand)
{
    EXPECT_CALL(*mockFileSystem, removeDirectory(_)).Times(1);
    commandExecutor->handlermdir();
}

TEST_F(TestCommandExecutorClass, TestrmfileCommand)
{
    EXPECT_CALL(*mockFileSystem, removeFile(_)).Times(1);
    commandExecutor->handlermfile();
}

TEST_F(TestCommandExecutorClass, TestfindCommand)
{
    EXPECT_CALL(*mockFileSystem, findFile(_)).Times(1);
    commandExecutor->handlefindFile();
}

class MockCommandParser : public CommandExecutor
{
public:
    MOCK_METHOD(void, separateCommandLine, (const std::string &), (override));
    MOCK_METHOD(bool, isCommandName, (const std::string &), (override));
};

class TestCommandExecutorParsing : public ::testing::Test
{
public:
    CommandExecutor *commandExecutor;
    MockCommandParser mockCommand;
    std::stringstream mockcin;
    std::stringstream mockcout;
    std::streambuf *realcin;
    std::streambuf *realcout;

    void SetUp() override
    {
        realcin = std::cin.rdbuf(mockcin.rdbuf());
        realcout = std::cout.rdbuf(mockcout.rdbuf());
        commandExecutor = new CommandExecutor();
    }

    void TearDown()
    {
        std::cin.rdbuf(realcin);
        std::cout.rdbuf(realcout);
        delete commandExecutor;
    }
};

TEST_F(TestCommandExecutorParsing, readingCommandLine)
{
    mockcin << "cd ..\nexit\n";
    EXPECT_CALL(mockCommand, separateCommandLine(_)).Times(AtLeast(1));
    mockCommand.readCommandLine();
}

TEST_F(TestCommandExecutorParsing, readingCommandLineCallisCommandName)
{
    mockcin << "cd ..\nexit\n";
    EXPECT_CALL(mockCommand, isCommandName(_)).Times(AtLeast(1));
    mockCommand.readCommandLine();
}

TEST_F(TestCommandExecutorParsing, readingCommandLineWillCallIsCommandNameForInvalidCommand)
{
    mockcin << "somecmd\nexit\n";
    EXPECT_CALL(mockCommand, isCommandName(_)).Times(AtLeast(1));
    mockCommand.readCommandLine();
}

TEST_F(TestCommandExecutorParsing, readingCommandLineInvalidCommand)
{
    mockcin << "somecmd\nexit\n";
    mockCommand.readCommandLine();
    ASSERT_TRUE(mockcout.str().find("Command not found") != std::string::npos);
}

TEST_F(TestCommandExecutorParsing, isCommandNameValidCommand)
{
    EXPECT_TRUE(commandExecutor->isCommandName("ls"));
}

TEST_F(TestCommandExecutorParsing, isCommandNameInvalidCommand)
{
    EXPECT_FALSE(commandExecutor->isCommandName("someCommand"));
}