//
// Created by Maya on 02.03.2026.
//
//
// Created by Maya on 02.03.2026.
//

#ifndef AKINATOR_PROJECT_FILEHANDLER_H
#define AKINATOR_PROJECT_FILEHANDLER_H

#include <iostream>
#include <filesystem>
#include <optional>
#include <fstream>
#include <vector>
#include <string>

namespace pp = std::filesystem;

class FileHandler {
private:
    std::string saveDirectory;
    std::string defaultFileName; // имя файла по умолчанию

    pp::path BuildPath(const std::string_view filename) const;

public:
    FileHandler(const std::string& directoryPath, const std::string& defaultName = "save.txt");

    bool fileExists(const std::string& filename);

    std::optional<std::string> readFile(const std::string& filename) const;

    bool writeFile(const std::string& filename, const std::string& text);

    std::vector<std::string> listSaveFiles() const;

    bool deleteFile(const std::string& filename) const;
};

#endif //AKINATOR_PROJECT_FILEHANDLER_H