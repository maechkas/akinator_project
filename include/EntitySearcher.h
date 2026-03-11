//
// Created by Анастасия on 08.03.2026.
//

#pragma once

#include <vector>
#include <string>
#include <optional>

class Node;

class EntitySearcher{
public:
    std::vector<std::vector<std::string>> findAllPathsToObject(const Node* root, const std::string& objectName);

    bool objectExists(const Node* root, const std::string& objectName);

    std::vector<std::string> getAllObjects(const Node* root);

    int getObjectDepth(const Node* root, const std::string& objectName);

    std::optional<std::string> findObjectByQuestions(const Node* root, const std::vector<bool>& answers);

};

