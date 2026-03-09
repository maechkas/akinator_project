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
        std::cerr << "errror in deserialize" << e.what() << "\n";
        throw;
    }
}