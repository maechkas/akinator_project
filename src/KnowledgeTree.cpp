#include "KnowledgeTree.hpp"

KnowledgeTree::KnowledgeTree():root_(nullptr),currentNode_(nullptr), nodeCount_(0) {

    std::unique_ptr<QuestionNode> root = std::make_unique<QuestionNode>("Is it a human?");
    std::unique_ptr<QuestionNode> noHuman_animal = std::make_unique<QuestionNode>("Is it an animal?");
    std::unique_ptr<QuestionNode> yesHuman_real = std::make_unique<QuestionNode>("Is it a real person?");

    std::unique_ptr<AnswerNode> noAnimal = std::make_unique<AnswerNode>("Cucumber");
    std::unique_ptr<AnswerNode> yesAnimal = std::make_unique<AnswerNode>("Fox");

    std::unique_ptr<AnswerNode> noReal = std::make_unique<AnswerNode>("Harry Potter");
    std::unique_ptr<AnswerNode> yesReal= std::make_unique<AnswerNode>("A.S.Pushkin");

    yesHuman_real->setYesBranch(std::move(yesReal));
    yesHuman_real->setNoBranch(std::move(noReal));

    noHuman_animal->setYesBranch(std::move(yesAnimal));
    noHuman_animal->setNoBranch(std::move(noAnimal));

    root->setNoBranch(std::move(noHuman_animal));
    root->setYesBranch(std::move(yesHuman_real));

    root_ = std::move(root);
    currentNode_ = root_.get();
    nodeCount_ = 7;
}