#include <gtest/gtest.h>
#include <filesystem>
#include "file_serial/tree_serializer.h"
#include "QuestionNode.hpp"
#include "KnowledgeTree.hpp"
#include <memory>
#include "FileHandler.h"

//Test1
TEST(TreeSerializerTest, EmptyTree) { //группа, имя
    TreeSerializer serializer;
    auto tree = std::make_unique<KnowledgeTree>(); //created empty tree
    EXPECT_THROW(serializer.serialize(*tree), std::runtime_error); //waited for run time error when we call serialize method
}

TEST(TreeSerializerTest, SerializeTree) {
    TreeSerializer serializer;
    auto root = std::make_unique<QuestionNode>("Это человек?"); //  только 1-arg конструктор
    auto yesNode = std::make_unique<QuestionNode>("Жанна Фриски"); // только 1-arg конструктор
    auto noNode = std::make_unique<QuestionNode>("Собака");



    root->setYesBranch(std::move(yesNode)); //setYesBranch → setNoBranch! std::move, because copy is prohibited in unique_ptr
    root->setNoBranch(std::move(noNode));

    auto tree = std::make_unique<KnowledgeTree>();
    tree->buildFromJson(root.get());

    std::string json = serializer.serialize(*tree);
    EXPECT_FALSE(json.empty());
    std::cout <<"Json: " <<json << "\n";
}

TEST(TreeSerializerTest, SaveAndLoadFile) {
    TreeSerializer serializer;
    auto root = std::make_unique<QuestionNode>("Тестовый вопрос");
    auto answer = std::make_unique<QuestionNode>("Тестовый ответ");


    root->setYesBranch(std::move(answer)); //то же самое что во 2 тесте, создаем обычное дерево тестоое.

    auto tree = std::make_unique<KnowledgeTree>();
    tree->buildFromJson(root.get());

    std::string filename = "test_save.json";
    bool saved = serializer.SaveTo(*tree, filename);
    EXPECT_TRUE(saved);
    EXPECT_TRUE(std::filesystem::exists(filename)); //проверка, что файл реально создался и нам это не приснилось
    auto loadedTree = serializer.LoadingFrom(filename);
    EXPECT_NE(loadedTree, nullptr); //выгружаем обратно и молимся что не пустота нам вернулась. В идеале(как задуманно) должен вернуться указатель на дерево
    std::filesystem::remove(filename); //удаляем от греха
}
