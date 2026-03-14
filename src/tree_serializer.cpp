#include "tree_serializer.h"
#include <fstream>
#include "KnowledgeTree.hpp"
#include <iostream>
#include <string>


using json = nlohmann::json;

// Вспомогательная рекурсивная функция для конвертации Node в JSON
json TreeSerializer::nodeToJsonHelper(const Node* node) {
    if (!node) return nullptr;

    json j;
    j[KEY_DATA] = node->getData();
    j[KEY_IS_ANSWER] = node->isAnswer();

    if (!node->isAnswer()) {
        // Это QuestionNode, у него есть ветки
        if (node->hasYesBranch()) {
            j[KEY_YES] = nodeToJsonHelper(node->getYesBranch());
        }
        if (node->hasNoBranch()) {
            j[KEY_NO] = nodeToJsonHelper(node->getNoBranch());
        }
    }

    return j;
}

// Публичные методы
std::string TreeSerializer::serialize(const KnowledgeTree& tree) {
    try {
        Node* rootNode = tree.getRoot();
        if (!rootNode) {
            throw std::runtime_error("Tree is empty");
        }

        json j;
        j["root"] = nodeToJsonHelper(rootNode);
        j["count"] = countNode(j["root"]);
        j["serializedAt"] = std::time(nullptr);

        return j.dump(JSON_INDENT);
    } catch (const std::exception& e) {
        std::cerr << "Error in serialization: " << e.what() << "\n";
        throw;
    }
}

std::unique_ptr<KnowledgeTree> TreeSerializer::deserialize(const std::string& jsonstr) {
    try {
        json j = json::parse(jsonstr);

        // Валидация
        if (!validate(jsonstr)) {
            throw std::runtime_error("Validation failed");
        }

        // Получаем корневой узел (может быть в поле "root" или прямо объект)
        json rootJson = j.contains("root") ? j["root"] : j;

        // Рекурсивно восстанавливаем дерево
        auto root = jsonTonode(rootJson);

        // Создаём дерево и устанавливаем корень
        auto tree = std::make_unique<KnowledgeTree>();
        tree->buildFromJson(std::move(root));

        return tree;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        std::cerr << "Deserialize error: " << e.what() << "\n";
        throw;
    }
}

bool TreeSerializer::validate(const std::string& jsonString) {
    try {
        json j = json::parse(jsonString);
        json rootJson = j.contains("root") ? j["root"] : j;
        return validateNode(rootJson);
    } catch (...) {
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
    } catch (...) {}
    return keys;
}

bool TreeSerializer::SaveTo(const KnowledgeTree& tree, const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file for writing: " << filename << std::endl;
            return false;
        }
        file << serialize(tree);
        std::cout << "Tree saved to: " << filename << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Save error: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<KnowledgeTree> TreeSerializer::LoadingFrom(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file for reading: " << filename << std::endl;
            return nullptr;
        }

        std::string jsonString((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());

        if (jsonString.empty()) {
            std::cerr << "File is empty: " << filename << std::endl;
            return nullptr;
        }

        return deserialize(jsonString);
    } catch (const std::exception& e) {
        std::cerr << "Loading error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<Node> TreeSerializer::jsonTonode(const nlohmann::json& j) {
    // ✅ ИСПРАВЛЕНО: добавлены ||
    if (!j.contains(KEY_DATA) || !j.contains(KEY_IS_ANSWER)) {
        throw std::runtime_error("jsonTonode: missing required fields");
    }

    std::string data = j.at(KEY_DATA).get<std::string>();
    bool isAnswer = j.at(KEY_IS_ANSWER).get<bool>();

    std::unique_ptr<Node> node;

    if (isAnswer) {
        node = std::make_unique<AnswerNode>(data);
    } else {
        node = std::make_unique<QuestionNode>(data);
    }

    // Рекурсивно восстанавливаем дерево
    if (!isAnswer) {
        QuestionNode* qnode = dynamic_cast<QuestionNode*>(node.get());
        if (!qnode) {
            throw std::runtime_error("Failed to cast to QuestionNode");
        }

        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            auto yesNode = jsonTonode(j[KEY_YES]);
            qnode->setYesBranch(std::move(yesNode));
        }

        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            auto noNode = jsonTonode(j[KEY_NO]);
            qnode->setNoBranch(std::move(noNode));
        }
    }

    return node;
}

bool TreeSerializer::validateNode(const json& j) {
    // ✅ ИСПРАВЛЕНО: добавлены || вместо пробелов
    if (!j.is_object() || !j.contains(KEY_DATA) || !j.contains(KEY_IS_ANSWER)) {
        return false;
    }

    bool isAnswer = j[KEY_IS_ANSWER].get<bool>();

    if (isAnswer) {
        // AnswerNode не должен иметь веток
        return !j.contains(KEY_YES) && !j.contains(KEY_NO);
    } else {
        // QuestionNode может иметь ветки, но они должны быть валидными
        bool yesValid = true;
        bool noValid = true;

        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            yesValid = validateNode(j[KEY_YES]);
        }

        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            noValid = validateNode(j[KEY_NO]);
        }

        return yesValid && noValid;
    }
}

size_t TreeSerializer::countNode(const json& j) {
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
