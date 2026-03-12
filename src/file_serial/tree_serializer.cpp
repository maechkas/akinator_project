#include "file_serial/tree_serializer.h"
#include <fstream>
#include <ctime>  // Добавлено для std::time
#include "KnowledgeTree.hpp"
#include <iomanip>
#include <iostream>
#include <string>
using json = nlohmann::json;

std::string TreeSerializer::serialize(const KnowledgeTree &tree) {
    try {
        Node* rootNode = tree.getRoot();
        if (!rootNode) {
            throw std::runtime_error("Tree is empty");
        }
        QuestionNode* root = dynamic_cast<QuestionNode*>(rootNode);  // Каст к QuestionNode*
        if (!root) {
            throw std::runtime_error("Root is not QuestionNode");
        }
        json j;
        j["root"] = nodeTojson(root);
        j["count"] = countNode(j["root"]);
        j["serializedAT"] = std::time(nullptr);
        return j.dump(4);
    } catch (const std::exception& e) {
        std::cerr << "Error in serialization: " << e.what() << "\n";
        throw;
    }
}


std::unique_ptr<KnowledgeTree> TreeSerializer::deserialize(const std::string &jsonstr) {
    try {
        json j = json::parse(jsonstr);
        if (!validate(jsonstr)) {
            throw std::runtime_error("No validation occurred");
        }
        json rootJson;
        if (j.contains("root")) {
            rootJson = j["root"];
        } else {
            rootJson = j;
        }

        auto root = jsonTonode(rootJson);  // jsonTonode → jsonToNode


        auto tree = std::make_unique<KnowledgeTree>();  // Конструктор по умолчанию
        tree->buildFromJson(root.get());  // Используем метод из KnowledgeTree.h
        return tree;
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("Error in parsing JSON: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        std::cerr << "Error in deserialize: " << e.what() << "\n";
        throw;
    }
}

bool TreeSerializer::validate(const std::string& jsonString) {
    try {
        json j = json::parse(jsonString);
        json rootJson;
        if (j.contains("root")) {
            rootJson = j["root"];
        } else {
            rootJson = j;
        }
        return validateNode(rootJson);
    }
    catch (const json::parse_error& e) {
        std::cerr << "Parse error in validation: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Validation error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> TreeSerializer::getKeys(const std::string& jsonString) {
    std::vector<std::string> keys;
    try {
        json j = json::parse(jsonString);
        for (auto it = j.begin(); it != j.end(); ++it) {
            keys.push_back(it.key());
        }
    }
    catch (...) {
        // Игнорируем ошибки
    }
    return keys;
}

bool TreeSerializer::SaveTo(const KnowledgeTree& tree, const std::string& filename) {
    try {
        std::string jsonString = serialize(tree);
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return false;
        }
        file << jsonString;
        if (!file.good()) {
            std::cerr << "Error writing to file: " << filename << std::endl;
            return false;
        }
        std::cout << "Tree saved to file: " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "File save error: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<KnowledgeTree> TreeSerializer::LoadingFrom(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for reading: " << filename << std::endl;
            return nullptr;
        }
        std::string jsonString((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
        auto tree = deserialize(jsonString);
        std::cout << "Tree loaded from file: " << filename << std::endl;
        return tree;
    }
    catch (const std::exception& e) {
        std::cerr << "File load error: " << e.what() << std::endl;
        return nullptr;
    }
}

// Private methods
json TreeSerializer::nodeTojson(QuestionNode* node) {  // nodeTojson → nodeToJson
    if (!node) {
        throw std::runtime_error("nodeToJson: passed nullptr");
    }
    json j;
    j[KEY_DATA] = node->getData();
    j[KEY_IS_ANSWER] = node->isAnswer();
    if (!node->isAnswer()) {
        if (node->hasYesBranch()) {  // hasYes → hasYesBranch
            QuestionNode* yesNode = static_cast<QuestionNode*>(node->getYesBranch());  // getYes → getYesBranch + cast
            if (yesNode) {
                j[KEY_YES] = nodeTojson(yesNode);
            }
        }
        if (node->hasNoBranch()) {  // hasNo → hasNoBranch
            QuestionNode* noNode = static_cast<QuestionNode*>(node->getNoBranch());  // getNo → getNoBranch + cast
            if (noNode) {
                j[KEY_NO] = nodeTojson(noNode);
            }
        }
    }
    return j;
}

std::unique_ptr<QuestionNode> TreeSerializer::jsonTonode(const json& j) {  // jsonTonode → jsonToNode
    if (!j.contains(KEY_DATA) || !j.contains(KEY_IS_ANSWER)) {
        throw std::runtime_error("jsonToNode: missing required fields");
    }

    // ИСПРАВЛЕНО ИЗ-ЗА ОТСУТСТВИЯ 2-аргументного конструктора QuestionNode
    std::string data = j.at(KEY_DATA).get<std::string>();
    auto node = std::make_unique<QuestionNode>(data);  // Только 1 аргумент!
    bool isAnswer = j.at(KEY_IS_ANSWER).get<bool>();
    // Нужен метод setIsAnswer в QuestionNode для установки флага после создания!

    if (!isAnswer) {
        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            auto yesNode = jsonTonode(j[KEY_YES]);
            yesNode->setParent(node.get());  // Предполагается метод setParent
            node->setYesBranch(std::move(yesNode));  // setYes → setYesBranch
        }
        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            auto noNode = jsonTonode(j[KEY_NO]);
            noNode->setParent(node.get());  // Предполагается метод setParent
            node->setNoBranch(std::move(noNode));  // setNo → setNoBranch
        }
    }
    return node;
}

bool TreeSerializer::validateNode(const json& j) {
    if (!j.is_object()) {
        std::cerr << "Error: node is not an object" << std::endl;
        return false;
    }
    if (!j.contains(KEY_DATA)) {
        std::cerr << "Error: missing 'data' field" << std::endl;
        return false;
    }
    if (!j.contains(KEY_IS_ANSWER)) {
        std::cerr << "Error: missing 'isAnswer' field" << std::endl;
        return false;
    }
    if (!j[KEY_DATA].is_string()) {
        std::cerr << "Error: 'data' field must be string" << std::endl;
        return false;
    }
    if (!j[KEY_IS_ANSWER].is_boolean()) {
        std::cerr << "Error: 'isAnswer' field must be boolean" << std::endl;
        return false;
    }

    bool isAnswer = j[KEY_IS_ANSWER].get<bool>();
    if (!isAnswer) {
        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            if (!j[KEY_YES].is_object()) {
                std::cerr << "Error: 'yes' branch must be object" << std::endl;
                return false;
            }
            if (!validateNode(j[KEY_YES])) {
                return false;
            }
        }
        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            if (!j[KEY_NO].is_object()) {
                std::cerr << "Error: 'no' branch must be object" << std::endl;
                return false;
            }
            if (!validateNode(j[KEY_NO])) {
                return false;
            }
        }
    }
    return true;
}

size_t TreeSerializer::countNode(const json& j) {  // Оставлено countNode (верно)
    size_t count = 1;
    if (j.contains(KEY_IS_ANSWER) && !j[KEY_IS_ANSWER].get<bool>()) {
        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            count += countNode(j[KEY_YES]);
        }
        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            count += countNode(j[KEY_NO]);
        }
    }
    return count;
}
