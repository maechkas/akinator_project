#include "tree_serializer.h"
#include <fstream>

#include "KnowledgeTree.hpp"

#include <iostream>
#include <string>
using json = nlohmann::json;
//public methods
std::string TreeSerializer::serialize(const KnowledgeTree &tree) {
    try {
        Node* rootNode = tree.getRoot();
        if (!rootNode) {
            throw std::runtime_error("Tree is empty");
        }
        QuestionNode* root = dynamic_cast<QuestionNode*>(rootNode);
        if (!root) {
            throw std::runtime_error("Root is not QuestionNode");
        }
        nlohmann::json j;
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
            throw std::runtime_error("Validation failed");
        }
        json rootJson = j.contains("root") ? j["root"] : j;

        auto root = jsonTonode(rootJson);  // unique_ptr<Node>

        auto tree = std::make_unique<KnowledgeTree>();
        // ИСПРАВЛЕНО: передаём unique_ptr через std::move
        tree->buildFromJson(std::move(root));  // ✅ больше не release()!

        return tree;
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        std::cerr << "Deserialize error: " << e.what() << "\n";
        throw;
    }
}
bool TreeSerializer::validate(const std::string& jsonString) {
    try {
        json j = json::parse(jsonString);
        json rootJson = j.contains("root") ? j["root"] : j;
        return validateNode(rootJson);
    } catch (...) { return false; }
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
        if (!file.is_open()) return false;
        file << serialize(tree);
        std::cout << "Tree saved: " << filename << std::endl;
        return true;
    } catch (...) { return false; }
}

std::unique_ptr<KnowledgeTree> TreeSerializer::LoadingFrom(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) return nullptr;
        std::string jsonString((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
        return deserialize(jsonString);
    } catch (...) { return nullptr; }
}

std::unique_ptr<Node> TreeSerializer::jsonTonode(const nlohmann::json& j) {
    if (!j.contains("data") || !j.contains("isAnswer")) {
        throw std::runtime_error("jsonTonode: missing required fields");
    }

    std::string data = j.at("data").get<std::string>();
    bool isAnswer = j.at("isAnswer").get<bool>();

    std::unique_ptr<Node> node;

    if (isAnswer) {
        node = std::make_unique<AnswerNode>(data);
    } else {
        node = std::make_unique<QuestionNode>(data);
    }

    // Рекурсивно восстанавливаем дерево
    if (!isAnswer) {
        QuestionNode* qnode = static_cast<QuestionNode*>(node.get());

        if (j.contains("yes") && !j["yes"].is_null()) {
            auto yesNode = jsonTonode(j["yes"]);
            // ИСПРАВЛЕНО: передаём unique_ptr через std::move
            qnode->setYesBranch(std::move(yesNode));  // ✅ больше не release()!
        }
        if (j.contains("no") && !j["no"].is_null()) {
            auto noNode = jsonTonode(j["no"]);
            // ИСПРАВЛЕНО: передаём unique_ptr через std::move
            qnode->setNoBranch(std::move(noNode));    // ✅ больше не release()!
        }
    }

    return node;  // возвращаем unique_ptr (RVO сработает)
}

nlohmann::json TreeSerializer::nodeTojson(QuestionNode* node) {
    if (!node) throw std::runtime_error("nodeToJson: nullptr");
    json j;
    j["data"] = node->getData();
    j["isAnswer"] = node->isAnswer();

    if (!node->isAnswer()) {
        // ИСПРАВЛЕНО: проверяем и кастуем для рекурсивного вызова
        Node* yesBranch = node->getYesBranch();
        if (yesBranch) {
            QuestionNode* yes = dynamic_cast<QuestionNode*>(yesBranch);
            if (yes) {
                j["yes"] = nodeTojson(yes);
            }
        }

        Node* noBranch = node->getNoBranch();
        if (noBranch) {
            QuestionNode* no = dynamic_cast<QuestionNode*>(noBranch);
            if (no) {
                j["no"] = nodeTojson(no);
            }
        }
    }
    return j;
}

bool TreeSerializer::validateNode(const json& j) {
    if (!j.is_object() || !j.contains("data") || !j.contains("isAnswer"))
        return false;

    bool isAnswer = j["isAnswer"];

    // Создаём variant из двух возможных состояний
    std::variant<std::true_type, std::false_type> nodeType;

    if (isAnswer) {
        nodeType = std::true_type{};   // AnswerNode
    } else {
        nodeType = std::false_type{};  // QuestionNode
    }

    struct Validator {
        const json& j;
        TreeSerializer* serializer;

        bool operator()(std::true_type) const {  // AnswerNode
            return !j.contains("yes") && !j.contains("no");
        }

        bool operator()(std::false_type) const {  // QuestionNode
            bool yesValid = !j.contains("yes") || j["yes"].is_null() ||
                           serializer->validateNode(j["yes"]);
            bool noValid = !j.contains("no") || j["no"].is_null() ||
                          serializer->validateNode(j["no"]);
            return yesValid && noValid;
        }
    };

    // Используем std::visit с variant
    return std::visit(Validator{j, this}, nodeType);
}
size_t TreeSerializer::countNode(const json& j) {
    size_t count = 1;
    if (!j["isAnswer"]) {
        if (j.contains("yes")) count += countNode(j["yes"]);
        if (j.contains("no")) count += countNode(j["no"]);
    }
    return count;
}