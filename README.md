#VIRTUAL FILE SYSTEM (VFS) PROJECT

##Introduction

The Virtual File System (VFS) is a simple implementation of a virtual file system that allows users to perform file and directory operations through a command line interface.

##Features

File Operations:
    -Create and delete file
    -Write to and read from file

Directory Operations:
    -Create and remove directory
    -Change the current working directory and display files in directories

Search Operations:
    -Find by file name
    -Find by timestamp
    -Find by file content

To run the code

cd VFSProject
./output/main

To run test cases

cd build
./executeTest

How to execute commands

mkdir directory_name 
    -This create a new directory in the working directory, path can also be specified where the directories in the path must be present

touch file_name 
    -This creates a new file in the working directory, if path is specified file will be created in that directory.

cd path
    -This changes the present working directory to given path. If path is empty it will be changed to root directory and if ".." is given it moves the directory one level up.

echo file_name
    -This will allow to write the content to the file specified, if it exists.

cat file_name
    -This will display the contents in the file specified, if it exists.

ls
    -This will display the files and directories present in the working directory.

rm *.extension
    -This will delete all the files in the directory with extension.

rm file_name
    -This deletes the file that is specified.

rmdir directory_name
    -This will delete the specified directory if it is empty.

find -name file_name
    -This will find the files with given name.

find -time seconds
    -This will find the files that are created 'seconds' number of seconds before from current time.

find -content text
    -This will find the files that contain the text given.