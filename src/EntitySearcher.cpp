#include "Node.hpp"
#include "EntitySearcher.h"
#include <vector>
#include <string>
#include <cstddef>

// вспомогательная рекурсивная функция, которая ищет путь от узла, до нужного объекта
void findPathsHelper(const Node* node, const std::string& target,
                     std::vector<std::string>& currentPath,
                     std::vector<std::vector<std::string>>& allPaths){
    if (!node){
        return;
    }

    currentPath.push_back(node->getData());

    if (node->isAnswer()){
        if (node->getData()==target){
            allPaths.push_back(currentPath);
        }
    }
    else{

        if (node->hasYesBranch()){
            currentPath.push_back("YES");
            findPathsHelper(node->getYesBranch(), target, currentPath, allPaths);
            currentPath.pop_back();
        }

        if (node->hasNoBranch()){
            currentPath.push_back("NO");
            findPathsHelper(node->getNoBranch(), target, currentPath, allPaths);
            currentPath.pop_back();
        }
    }
    currentPath.pop_back();

}

// метод findAllPathsToObject (находит все пути к объекту)
std::vector<std::vector<std::string>> EntitySearcher::findAllPathsToObject(const Node* root, const std::string& objectName){
    std::vector<std::string> currentPath;
    std::vector<std::vector<std::string>> allPaths;
    findPathsHelper(root, objectName,currentPath,allPaths);
    return allPaths;
}

// метод objectExists (проверяет существование объекта)
bool EntitySearcher::objectExists(const Node* root, const std::string& objectName){
    std::vector<std::string> currentPath;
    std::vector<std::vector<std::string>> allPaths;
    findPathsHelper(root, objectName,currentPath,allPaths);
    return !allPaths.empty();
}

// вспомогательная рекурсивная функция для getAllObjects
void collectObjectsHelper(const Node* node, std::vector<std::string>& objects){
    if (!node){
        return;
    }
    if (node->isAnswer()){
        objects.push_back(node->getData());
    }

    else{
        if (node->hasYesBranch()){
            collectObjectsHelper(node->getYesBranch(), objects);
        }
        if (node->hasNoBranch()){
            collectObjectsHelper(node->getNoBranch(), objects);
        }
    }

}

// метод getAllObjects (собирает все объекты-листья)
std::vector<std::string> EntitySearcher::getAllObjects(const Node* root){
    std::vector<std::string> objects;
    collectObjectsHelper(root,objects);
    return objects;
}

// метод findObjectByAnswers (ищет объект по ответам пользователя)
std::optional<std::string> EntitySearcher::findObjectByAnswers(const Node* root, const std::vector<bool>& answers){
    if (!root){
        return std::nullopt;
    }
    const Node* current = root;
    for (size_t i=0; i<answers.size(); ++i){
        const bool answer=answers[i];

        if (current->isAnswer()){
            return std::nullopt;
        }

        if (answer){
            if (current->hasYesBranch()){
                current=current->getYesBranch();
            }
            else{
                return std::nullopt;
            }
        }

        else{
            if (current->hasNoBranch()){
                current=current->getNoBranch();
            }
            else{
                return std::nullopt;
            }
        }
    }
    if (current->isAnswer()){
        return current->getData();
    }
    else{
        return std::nullopt;
    }
}
