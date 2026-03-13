#include <gtest/gtest.h>
#include "tree_serializer.h"
#include "KnowledgeTree.hpp"

TEST(TreeSerializerTest, SimpleRoundTrip) {
    TreeSerializer s;

    // Создаём дерево
    auto tree = std::make_unique<KnowledgeTree>();
    auto root = std::make_unique<QuestionNode>("Животное?");
    auto yes = std::make_unique<AnswerNode>("Собака");
    root->setYesBranch(std::move(yes));

    tree->buildFromJson(std::move(root));

    std::string json = s.serialize(*tree);
    auto back = s.deserialize(json);

    EXPECT_EQ(back->getRoot()->getData(), "Животное?");
}



TEST(TreeSerializerTest, ValidateJSON) {
    TreeSerializer s;
    EXPECT_TRUE(s.validate(R"({"data":"test","isAnswer":true})"));
    EXPECT_FALSE(s.validate(R"({"data":"test"})"));
}
TEST(TreeSerializerTest, SerializeTree) {
    TreeSerializer serializer;

    // Корень (вопрос)
    auto root = std::make_unique<QuestionNode>("Это человек?");

    // Yes ветка — ответ (НЕ вопрос!)
    auto yesNode = std::make_unique<AnswerNode>("Жанна Фриски");
    // No ветка — ответ
    auto noNode = std::make_unique<AnswerNode>("Собака");

    root->setYesBranch(std::move(yesNode));
    root->setNoBranch(std::move(noNode));

    auto tree = std::make_unique<KnowledgeTree>();
    tree->buildFromJson(std::move(root));

    std::string json = serializer.serialize(*tree);
    EXPECT_FALSE(json.empty());
    std::cout << "Json: " << json << "\n";
}

TEST(TreeSerializerTest, FullRoundtrip) {
    TreeSerializer s;

    auto tree = std::make_unique<KnowledgeTree>();
    auto root = std::make_unique<QuestionNode>("Человек?");
    auto yesAns = std::make_unique<AnswerNode>("Бэтмен");
    root->setYesBranch(std::move(yesAns));
    tree->buildFromJson(std::move(root));

    // 2. Проверяем, что дерево НЕ пустое
    ASSERT_NE(tree->getRoot(), nullptr);

    // 3. serialize → deserialize
    std::string json = s.serialize(*tree);
    auto back = s.deserialize(json);

    //  Проверяем восстановление
    ASSERT_NE(back->getRoot(), nullptr);
    EXPECT_EQ(back->getRoot()->getData(), "Человек?");
}

TEST(TreeSerializerTest, GetJsonKeys) {
    TreeSerializer s;
    std::string jsonStr = R"({"root":{},"count":1,"serializedAT":123})";

    auto keys = s.getKeys(jsonStr);


    EXPECT_EQ(keys.size(), 3);
    EXPECT_NE(std::find(keys.begin(), keys.end(), "root"), keys.end());
    EXPECT_NE(std::find(keys.begin(), keys.end(), "count"), keys.end());
    EXPECT_NE(std::find(keys.begin(), keys.end(), "serializedAT"), keys.end());
}
