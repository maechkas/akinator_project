//
// Created by Maya on 02.03.2026.
//

#ifndef AKINATOR_PROJECT_FILEHANDLER_H
#define AKINATOR_PROJECT_FILEHANDLER_H
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
    bool writeFile(const std::string& filename, const std::string& text) {
        auto path = BuildPath(filename);
        std::ofstream file(path);
        if (!file.is_open()) { // файл закрывать не нужно, тк он автоматически закроется когда вфйдет из области видимости. RAII освобождается автоматически  в деструкторе
            return false;
        }
        file << text;
        return true;
    }
    std::vector <std::string> listSaveFiles() const {
        std::vector <std::string> file_names;
        for (const auto& a : pp::directory_iterator(saveDirectory)) {
            if (a.is_regular_file()) {
                file_names.push_back(a.path().filename().string());
            }
        }
        return file_names;
    }
    bool deleteFile (const std::string& filename) const {
        auto path = BuildPath(filename);
        if (!pp::exists(path)) {
            return false;
        }
        return pp::remove(path);
    }
};

#endif //AKINATOR_PROJECT_FILEHANDLER_H