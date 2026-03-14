#ifndef AKINATOR_PROJECTSS_TREE_SERIALIZER_H
#define AKINATOR_PROJECTSS_TREE_SERIALIZER_H

#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <vector>
#include <ctime>
#include "QuestionNode.hpp"
#include "AnswerNode.hpp"
#include "KnowledgeTree.hpp"

class TreeSerializer {
public:
    TreeSerializer() = default;
    ~TreeSerializer() = default;

    TreeSerializer(const TreeSerializer&) = delete;
    TreeSerializer& operator=(const TreeSerializer&) = delete;
    TreeSerializer(TreeSerializer&&) = default;
    TreeSerializer& operator=(TreeSerializer&&) = default;

    std::string serialize(const KnowledgeTree& tree);
    std::unique_ptr<KnowledgeTree> deserialize(const std::string& jsonstr);
    bool validate(const std::string& jsonstr);
    std::vector<std::string> getKeys(const std::string& jsonstr);
    bool SaveTo(const KnowledgeTree& tree, const std::string& filename);
    std::unique_ptr<KnowledgeTree> LoadingFrom(const std::string& filename);

private:
    // Вспомогательные функции
    nlohmann::json nodeToJsonHelper(const Node* node);
    std::unique_ptr<Node> jsonTonode(const nlohmann::json& j);
    bool validateNode(const nlohmann::json& j);
    size_t countNode(const nlohmann::json& j);

    static constexpr const char* KEY_DATA = "data";
    static constexpr const char* KEY_IS_ANSWER = "isAnswer";
    static constexpr const char* KEY_YES = "yes";
    static constexpr const char* KEY_NO = "no";
    static constexpr int JSON_INDENT = 4;
};

#endif