cmake_minimum_required(VERSION 3.10)

project(VirtualFileSystem)
# Add your source files to a library
add_library(vfsLibrary STATIC
Source/commandExecutor.cpp
Source/directory.cpp
Source/file.cpp
Source//fileSystem.cpp
Source/fileSystemComponent.cpp
)
target_include_directories(vfsLibrary PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


add_executable(executeCode mainFile.cpp)
target_link_libraries(executeCode vfsLibrary)

add_executable(executeTest Test/testvfs.cpp)
target_link_libraries(executeTest vfsLibrary gtest gmock gtest_main gmock_main)
