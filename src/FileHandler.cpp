//
// Created by Ангелина Радаева on 12.03.2026.
//
//
// Created by Maya on 02.03.2026.
//

#include "FileHandler.h"

pp::path FileHandler::BuildPath(const std::string_view filename) const {
    if (filename.empty())
        return pp::path(saveDirectory) / defaultFileName;
    return pp::path(saveDirectory) / filename;
}

FileHandler::FileHandler(const std::string& directoryPath, const std::string& defaultName)
    : saveDirectory(directoryPath), defaultFileName(defaultName) {
    if (!pp::exists(saveDirectory)) {
        pp::create_directories(saveDirectory);
    }
}

bool FileHandler::fileExists(const std::string& filename) {
    return pp::exists(BuildPath(filename));
}

std::optional<std::string> FileHandler::readFile(const std::string& filename) const {
    auto path = BuildPath(filename);
    if (!pp::exists(path)) {
        return std::nullopt;
    }
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Error, file cant be opened: " + std::string(filename));
    }
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return text;
}

bool FileHandler::writeFile(const std::string& filename, const std::string& text) {
    auto path = BuildPath(filename);
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << text;
    return true;
}

std::vector<std::string> FileHandler::listSaveFiles() const {
    std::vector<std::string> file_names;
    for (const auto& a : pp::directory_iterator(saveDirectory)) {
        if (a.is_regular_file()) {
            file_names.push_back(a.path().filename().string());
        }
    }
    return file_names;
}

bool FileHandler::deleteFile(const std::string& filename) const {
    auto path = BuildPath(filename);
    if (!pp::exists(path)) {
        return false;
    }
    return pp::remove(path);
}