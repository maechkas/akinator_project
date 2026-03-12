#include <gtest/gtest.h>
#include "Node.hpp"
#include "QuestionNode.hpp"
#include "AnswerNode.hpp"

TEST(Answer_Question_TestNode, Constructor) {
    AnswerNode node("node");
    EXPECT_EQ(node.getData(), "node");
    EXPECT_EQ(node.getParent(), nullptr);

    AnswerNode nodeA("Answer");
    EXPECT_EQ(nodeA.getData(), "Answer");
    EXPECT_TRUE(nodeA.isAnswer());
    EXPECT_FALSE(nodeA.hasYesBranch());
    EXPECT_FALSE(nodeA.hasNoBranch());

    QuestionNode nodeQ("Question");
    EXPECT_EQ(nodeQ.getData(), "Question");
    EXPECT_FALSE(nodeQ.isAnswer());
    EXPECT_FALSE(nodeQ.hasYesBranch());
    EXPECT_FALSE(nodeQ.hasNoBranch());
    EXPECT_TRUE(nodeQ.isLeaf());
}

TEST(Answer_QuestionTest, SetParent) {
    AnswerNode child("Child");
    QuestionNode parent("Parent");

    child.setParent(&parent);
    EXPECT_EQ(child.getParent(), &parent);
}

TEST(QuestionNodeTest, SetBranch) {
    QuestionNode question("Question");
    std::unique_ptr<AnswerNode> yesAnswer = std::make_unique<AnswerNode>("AnsYes");
    std::unique_ptr<AnswerNode> noAnswer = std::make_unique<AnswerNode>("AnsNo");

    question.setYesBranch(std::move(yesAnswer));
    question.setNoBranch(std::move(noAnswer));

    EXPECT_TRUE(question.hasYesBranch());
    EXPECT_TRUE(question.hasNoBranch());
    EXPECT_FALSE(question.isLeaf());

    Node* yesBranch = question.getYesBranch();
    ASSERT_NE(yesBranch, nullptr);
    EXPECT_EQ(yesBranch->getData(), "AnsYes");
    EXPECT_EQ(yesBranch->getParent(), &question);

    Node* noBranch = question.getNoBranch();
    ASSERT_NE(noBranch, nullptr);
    EXPECT_EQ(noBranch->getData(), "AnsNo");
    EXPECT_EQ(noBranch->getParent(), &question);
}

TEST(TestNode, GetBranch) {
    AnswerNode node("Test");
    EXPECT_EQ(node.getYesBranch(), nullptr);
    EXPECT_EQ(node.getNoBranch(), nullptr);
}

TEST(QuestionNodeTest, ReleaseBranch) {
    QuestionNode question1("Question1");
    std::unique_ptr<AnswerNode> answer1 = std::make_unique<AnswerNode>("AnswerYes");
    question1.setYesBranch(std::move(answer1));
    EXPECT_TRUE(question1.hasYesBranch());
    std::unique_ptr<Node> released1 = question1.releaseYesBranch();
    EXPECT_FALSE(question1.hasYesBranch());
    ASSERT_NE(released1, nullptr);
    EXPECT_EQ(released1->getData(), "AnswerYes");
    EXPECT_EQ(released1->getParent(), nullptr);

    QuestionNode question2("Question1");
    std::unique_ptr<AnswerNode> answer2 = std::make_unique<AnswerNode>("AnswerNo");
    question2.setNoBranch(std::move(answer2));
    EXPECT_TRUE(question2.hasNoBranch());
    std::unique_ptr<Node> released2 = question2.releaseNoBranch();
    EXPECT_FALSE(question2.hasNoBranch());
    ASSERT_NE(released2, nullptr);
    EXPECT_EQ(released2->getData(), "AnswerNo");
    EXPECT_EQ(released2->getParent(), nullptr);

    QuestionNode question3("Question3");
    std::unique_ptr<Node> released3 = question3.releaseYesBranch();
    EXPECT_EQ(released3, nullptr);
}

TEST(QuestionNodeTest, IsLeaf) {
    QuestionNode question("Question");
    EXPECT_TRUE(question.isLeaf());
    std::unique_ptr<AnswerNode> answer = std::make_unique<AnswerNode>("Answer");
    question.setYesBranch(std::move(answer));
    EXPECT_FALSE(question.isLeaf());
}

TEST(Answer_Question_TestNode, VirtualMethods) {
    AnswerNode answer("Answer");
    EXPECT_TRUE(answer.isAnswer());
    EXPECT_FALSE(answer.hasYesBranch());
    EXPECT_FALSE(answer.hasNoBranch());
    EXPECT_EQ(answer.getYesBranch(), nullptr);
    EXPECT_EQ(answer.getNoBranch(), nullptr);

    QuestionNode question("Question?");
    EXPECT_FALSE(question.isAnswer());
    EXPECT_FALSE(question.hasYesBranch());
    EXPECT_FALSE(question.hasNoBranch());
}

TEST(Answer_Question_TestNode, Polymorphism) {
    std::unique_ptr<Node> answer = std::make_unique<AnswerNode>("Answer");
    EXPECT_TRUE(answer->isAnswer());
    EXPECT_FALSE(answer->hasYesBranch());
    EXPECT_FALSE(answer->hasNoBranch());
    EXPECT_EQ(answer->getYesBranch(), nullptr);
    EXPECT_EQ(answer->getNoBranch(), nullptr);
    EXPECT_EQ(answer->getData(), "Answer");

    std::unique_ptr<Node> question = std::make_unique<QuestionNode>("Question");
    EXPECT_FALSE(question->isAnswer());
    EXPECT_FALSE(question->hasYesBranch());
    EXPECT_FALSE(question->hasNoBranch());
    EXPECT_EQ(question->getYesBranch(), nullptr);
    EXPECT_EQ(question->getNoBranch(), nullptr);
    EXPECT_EQ(question->getData(), "Question");
}
