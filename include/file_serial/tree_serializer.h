//
// Created by Maya on 10.03.2026.
//

#ifndef AKINATOR_PROJECTSS_TREE_SERIALIZER_H
#define AKINATOR_PROJECTSS_TREE_SERIALIZER_H
#include <nlohmann/json.hpp>
#include <memory>
#include <QuestionNode.hpp>
#include<KnowledgeTree.hpp>
class Treeserializer {
public:
    Treeserializer()=default;
    ~Treeserializer()=default;
    Treeserializer(const Treeserializer&) = delete;
    Treeserializer& operator=(const Treeserializer& )= delete;
    Treeserializer(Treeserializer&&)=default;
    Treeserializer& operator+(const Treeserializer&&)= default;
    std::string serialize(const KnowledgeTree& tree);
    std::unique_ptr<KnowledgeTree> desertilize(const std::string& jsonstr);
    bool validate (const std::string& jsonstr);
    std::vector<std::string> getKeys (const std::string& jsonstr);
    bool SaveTo (const KnowledgeTree& tree, const std::string& filename);
    std::unique_ptr<KnowledgeTree> LoadingFrom(const std::string& filename);
private:
    nlohmann::json nodeTojson(QuestionNode* node);
    std::unique_ptr <QuestionNode> jsonTonode(const nlohmann::json& j);
    bool valiateNode(const nlohmann::json& j); //checking fields
    size_t countNode(const nlohmann::json& j);
    static constexpr const char* KEY_DATA = "data";
    static constexpr const char* KEY_IS_ANSWER = "isAnswer";
    static constexpr const char* KEY_YES = "yes";
    static constexpr const char* KEY_NO = "no";
    static constexpr int JSON_INDENT = 4;


};




#endif //AKINATOR_PROJECTSS_TREE_SERIALIZER_H