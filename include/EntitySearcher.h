//
// Created by Анастасия on 08.03.2026.
//

#pragma once

#include <vector>
#include <string>
#include <optional>

class Node;

class EntitySearcher {
public:
    static std::vector<std::vector<std::string>> findAllPathsToObject(const Node* root, const std::string& objectName);

    static bool objectExists(const Node* root, const std::string& objectName);

    static std::vector<std::string> getAllObjects(const Node* root);

    static std::optional<std::string> findObjectByAnswers(const Node* root, const std::vector<bool>& answers);

};

