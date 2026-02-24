//
// Created by Maya on 25.02.2026.
//
#include <__filesystem/filesystem_error.h>


#include <iostream>
#include<filesystem>
#include<optional>
#include <fstream>
#include<vector>
namespace pp = std::filesystem;

class FileHandler {
private:
    std::string saveDirectory;
    std::string defaultFileName; // имя файла по умолчанию
    pp::path BuildPath(const std::string_view filename)const {
        if (filename.empty())
            return pp::path(saveDirectory) / defaultFileName;
        return pp::path(saveDirectory) / filename;
    }
public:
    FileHandler(const std::string& directoryPath, const std::string& defaultName = "save.txt") : saveDirectory(directoryPath), defaultFileName(defaultName) {
        if (!pp::exists(saveDirectory)) {
            pp::create_directories(saveDirectory);
        }
    }
    bool fileExists(const std::string& filename)  {
        return pp::exists(BuildPath(filename));
    }