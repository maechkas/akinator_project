#include "file_serial/tree_serializer.h"
#include <fstream>
#include "KnowledgeTree.hpp"
#include <iomanip>
#include <iostream>
#include <string>
using json = nlohmann::json;
std::string Treeserializer::serialize(const KnowledgeTree &tree) {
    try {
        QuestionNode* root = tree.getRoot();
        if (!root) {
            trew std::runtime_error("Tree is empty");
        }
        json j;
        j["root"] = nodeTojson(root);
        j["count"] = countNode(j["root"]);
        j["serializedAT"]=std::time(nullptr);
        return j.dump(JSON_INDENT);
    }// отступы 4
        catch (const std::exception& e) {
            std::cerr<< "Error in serialization" << e.what()<< "\n";
            throw;
        }
}
std::unique_ptr<KnowledgeTree> Treeserializer::desertilize(const std::string &jsonstr) {
    try {
        json j= json::parse(jsonstr);
        if (!validate(jsonstr)) {
            trow std::runtime_error("No validation occured");
        }
        json rootJson;
        if (j.contains(("root"))){
            rootJson=j["root"];
        } else {
            rootJson=j;
        }
        auto root=jsonTonode((rootJson));
        auto tree = std::make_unique<KnowledgeTree>(std::move(root));
        std::cout <<"Nodes Downloaded: " << countNode(rootJson)<< "\n";
        return tree;
    }
    catch (const json::parse_error& e){
        throws std::runtime_error("Error in parsing JSON" + std::string(e.what()));
}
    catch (const std::exception& e) {
        std::cerr << "error in deserialize" << e.what() << "\n";
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
        std::cerr << "Ошибка парсинга при валидации: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка валидации: " << e.what() << std::endl;
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
        // Игнорируем ошибки, возвращаем пустой вектор
    }

    return keys;
}

bool TreeSerializer::saveToFile(const KnowledgeTree& tree, const std::string& filename) {
    try {
        std::string jsonString = serialize(tree);

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл для записи: "
                      << filename << std::endl;
            return false;
        }

        file << jsonString;

        if (!file.good()) {
            std::cerr << "Ошибка при записи в файл: "
                      << filename << std::endl;
            return false;
        }

        std::cout << "Дерево сохранено в файл: " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения в файл: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<KnowledgeTree> TreeSerializer::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл для чтения: "
                      << filename << std::endl;
            return nullptr;
        }

        std::string jsonString((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());

        auto tree = deserialize(jsonString);

        std::cout << "Дерево загружено из файла: " << filename << std::endl;
        return tree;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки из файла: " << e.what() << std::endl;
        return nullptr;
    }
}
//private methods
json TreeSerializer::nodeToJson(QuestionNode* node) {
    if (!node) {
        throw std::runtime_error("nodeToJson: передан nullptr");
    }

    json j;

    j[KEY_DATA] = node->getData();
    j[KEY_IS_ANSWER] = node->isAnswer();

    if (!node->isAnswer()) {
        if (node->hasYes()) {
            QuestionNode* yesNode = node->getYes();
            if (yesNode) {
                j[KEY_YES] = nodeToJson(yesNode);
            }
        }

        if (node->hasNo()) {
            QuestionNode* noNode = node->getNo();
            if (noNode) {
                j[KEY_NO] = nodeToJson(noNode);
            }
        }
    }

    return j;
}

std::unique_ptr<QuestionNode> TreeSerializer::jsonToNode(const json& j) {
    if (!j.contains(KEY_DATA) || !j.contains(KEY_IS_ANSWER)) {
        throw std::runtime_error("jsonToNode: отсутствуют обязательные поля");
    }

    std::string data;
    bool isAnswer;

    try {
        data = j.at(KEY_DATA).get<std::string>();
        isAnswer = j.at(KEY_IS_ANSWER).get<bool>();
    }
    catch (const json::type_error& e) {
        throw std::runtime_error("jsonToNode: неверный тип поля: " + std::string(e.what()));
    }

    auto node = std::make_unique<QuestionNode>(data, isAnswer);

    if (!isAnswer) {
        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            auto yesNode = jsonToNode(j[KEY_YES]);
            yesNode->setParent(node.get());
            node->setYes(std::move(yesNode));
        }

        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            auto noNode = jsonToNode(j[KEY_NO]);
            noNode->setParent(node.get());
            node->setNo(std::move(noNode));
        }
    }

    return node;
}

bool TreeSerializer::validateNode(const json& j) {
    if (!j.is_object()) {
        std::cerr << "Ошибка: узел не является объектом" << std::endl;
        return false;
    }

    if (!j.contains(KEY_DATA)) {
        std::cerr << "Ошибка: отсутствует поле 'data'" << std::endl;
        return false;
    }
    if (!j.contains(KEY_IS_ANSWER)) {
        std::cerr << " Ошибка: отсутствует поле 'isAnswer'" << std::endl;
        return false;
    }

    if (!j[KEY_DATA].is_string()) {
        std::cerr << "Ошибка: поле 'data' должно быть строкой" << std::endl;
        return false;
    }
    if (!j[KEY_IS_ANSWER].is_boolean()) {
        std::cerr << "Ошибка: поле 'isAnswer' должно быть boolean" << std::endl;
        return false;
    }

    bool isAnswer = j[KEY_IS_ANSWER].get<bool>();

    if (!isAnswer) {
        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            if (!j[KEY_YES].is_object()) {
                std::cerr << "Ошибка: ветка 'yes' должна быть объектом" << std::endl;
                return false;
            }
            if (!validateNode(j[KEY_YES])) {
                return false;
            }
        }

        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            if (!j[KEY_NO].is_object()) {
                std::cerr << "Ошибка: ветка 'no' должна быть объектом" << std::endl;
                return false;
            }
            if (!validateNode(j[KEY_NO])) {
                return false;
            }
        }
    }

    return true;
}

size_t TreeSerializer::countNodes(const json& j) {
    size_t count = 1;

    if (j.contains(KEY_IS_ANSWER) && !j[KEY_IS_ANSWER].get<bool>()) {
        if (j.contains(KEY_YES) && !j[KEY_YES].is_null()) {
            count += countNodes(j[KEY_YES]);
        }
        if (j.contains(KEY_NO) && !j[KEY_NO].is_null()) {
            count += countNodes(j[KEY_NO]);
        }
    }

    return count;
}
