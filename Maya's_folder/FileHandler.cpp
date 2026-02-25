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

    std::optional<std::string> readFile(const std::string& filename)const {
        auto path = BuildPath(filename); // используем optional (контейнер)  потому что 2 состояния, возвращается либо строка или нихрена
        if (!pp::exists(path)) {
            return std::nullopt;
        }
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Error, file cant be opened:" + std::string(filename));
        }
        std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
                    );//читает всю строку до конца, можно получать доступ к элементам пока не достигнем конца
        return text;
    }
};