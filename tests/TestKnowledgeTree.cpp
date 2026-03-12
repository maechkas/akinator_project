#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include "KnowledgeTree.hpp"
#include "QuestionNode.hpp"
#include "AnswerNode.hpp"

TEST(KnowledgeTreeTest, DefaultConstructor) {
    KnowledgeTree tree;
    EXPECT_FALSE(tree.isEmpty());
    EXPECT_NE(tree.getRoot(), nullptr);
    EXPECT_EQ(tree.getNodeCount(), 7);
    EXPECT_FALSE(tree.getRoot()->isAnswer());
}

TEST(KnowledgeTreeTest, ResetGame) {
    KnowledgeTree tree;
    tree.moveYes();
    tree.moveYes();
    EXPECT_NE(tree.getCurrentNode(), tree.getRoot());
    tree.resetGame();
    EXPECT_EQ(tree.getCurrentNode(), tree.getRoot());
}

TEST(KnowledgeTreeTest, Navigation) {
    KnowledgeTree tree;
    EXPECT_TRUE(tree.getRoot()->hasYesBranch());
    EXPECT_TRUE(tree.getRoot()->hasNoBranch());

    Node* yesNode = tree.getRoot()->getYesBranch();
    tree.moveYes();
    EXPECT_EQ(tree.getCurrentNode(), yesNode);

    tree.resetGame();
    tree.moveNo();
    EXPECT_NE(tree.getCurrentNode(), tree.getRoot());
}

TEST(KnowledgeTreeTest, IsCurrentNodeAnswer) {
    KnowledgeTree tree;
    EXPECT_FALSE(tree.isCurrentNodeAnswer());
    tree.moveYes();
    tree.moveYes();
    EXPECT_TRUE(tree.isCurrentNodeAnswer());
}

TEST(KnowledgeTreeTest, GetCurrentData) {
    KnowledgeTree tree;
    EXPECT_EQ(tree.getCurrentData(), "Is it a human?");
    tree.moveYes();
    EXPECT_EQ(tree.getCurrentData(), "Is it a real person?");
    tree.moveYes();
    EXPECT_EQ(tree.getCurrentData(), "A.S.Pushkin");
}

TEST(KnowledgeTreeTest, HasObject) {
    KnowledgeTree tree;
    EXPECT_TRUE(tree.hasObject("A.S.Pushkin"));
    EXPECT_TRUE(tree.hasObject("Harry Potter"));
    EXPECT_TRUE(tree.hasObject("Fox"));
    EXPECT_TRUE(tree.hasObject("Cucumber"));
    EXPECT_FALSE(tree.hasObject("NonExistent"));
}

TEST(KnowledgeTreeTest, LearnNewObject) {
    KnowledgeTree tree;
    tree.moveNo();
    tree.moveNo();
    ASSERT_TRUE(tree.isCurrentNodeAnswer());
    std::string oldAnswer = tree.getCurrentData();
    EXPECT_EQ(oldAnswer, "Cucumber");

    size_t oldCount = tree.getNodeCount();
    std::string newObject = "Tomato";
    std::string newQuestion = "Is it red?";
    bool answerForNew = true;

    tree.learnNewObject(newObject, newQuestion, answerForNew);

    EXPECT_FALSE(tree.isCurrentNodeAnswer());
    EXPECT_EQ(tree.getCurrentData(), newQuestion);
    EXPECT_EQ(tree.getNodeCount(), oldCount + 2);

    Node* questionNode = tree.getCurrentNode();
    ASSERT_TRUE(questionNode->hasYesBranch());
    ASSERT_TRUE(questionNode->hasNoBranch());

    Node* yesBranch = questionNode->getYesBranch();
    Node* noBranch = questionNode->getNoBranch();

    ASSERT_TRUE(yesBranch->isAnswer());
    ASSERT_TRUE(noBranch->isAnswer());
    EXPECT_EQ(yesBranch->getData(), newObject);
    EXPECT_EQ(noBranch->getData(), oldAnswer);
    EXPECT_EQ(yesBranch->getParent(), questionNode);
    EXPECT_EQ(noBranch->getParent(), questionNode);
}

TEST(KnowledgeTreeTest, LearnNewObjectOnNonAnswerThrows) {
    KnowledgeTree tree;
    EXPECT_THROW({
        tree.learnNewObject("New", "Question?", true);
    }, std::runtime_error);
}

TEST(KnowledgeTreeTest, BuildFromJson) {
    KnowledgeTree tree;
    auto root = std::make_unique<QuestionNode>("Is it alive?");
    auto yesAns = std::make_unique<AnswerNode>("Dog");
    auto noAns = std::make_unique<AnswerNode>("Stone");

    yesAns->setParent(root.get());
    noAns->setParent(root.get());
    root->setYesBranch(std::move(yesAns));
    root->setNoBranch(std::move(noAns));

    Node* rootPtr = root.get();

    tree.buildFromJson(std::move(root));

    EXPECT_EQ(tree.getRoot(), rootPtr);
    EXPECT_EQ(tree.getNodeCount(), 3);
    EXPECT_TRUE(tree.hasObject("Dog"));
    EXPECT_TRUE(tree.hasObject("Stone"));
    EXPECT_FALSE(tree.hasObject("Cat"));
}

TEST(KnowledgeTreeTest, NodeCountAfterLearning) {
    KnowledgeTree tree;
    size_t before = tree.getNodeCount();
    tree.moveNo();
    tree.moveNo();  // на "Cucumber"
    tree.learnNewObject("Tomato", "Is it red?", true);
    EXPECT_EQ(tree.getNodeCount(), before + 2);
}