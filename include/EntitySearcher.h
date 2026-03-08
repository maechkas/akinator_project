//
// Created by Анастасия on 08.03.2026.
//

#pragma once

#include<vector>
#include<string>

class QuestionNode;

class EntitySearcher{
public:
    std::vector<std::vector<std::string>> findAllPathsToObject(QuestionNode* root, const std::string& objectName);

    bool objectExists(QuestionNode* root, const std::string& objectName);

    std::vector<std::string> getAllObjects(QuestionNode* root);

    int getObjectDepth(QuestionNode* root, const std::string& objectName);

    std::string findObjectByQuestions(QuestionNode* root, const std::vector<bool>& answers);

};