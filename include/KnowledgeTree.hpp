#ifndef AKINATOR_PROJECT_KNOWLEDGETREE_H
#define AKINATOR_PROJECT_KNOWLEDGETREE_H

#include "Node.hpp"
#include "QuestionNode.hpp"
#include "AnswerNode.hpp"
#include <memory>

class KnowledgeTree {
private:
    std::unique_ptr<Node>root_;
    Node* currentNode_;
    size_t nodeCount_;

public:
    KnowledgeTree();

    KnowledgeTree(const KnowledgeTree&) = delete;
    KnowledgeTree& operator=(const KnowledgeTree&) = delete;
    KnowledgeTree(KnowledgeTree&&) = default;
    KnowledgeTree& operator=(KnowledgeTree&&) = default;

    Node* getRoot() const;
    size_t getNodeCount() const;

    bool isEmpty() const;
    bool hasObject(const std::string& objectName) const;

    void resetGame();
    void moveYes();
    void moveNo();
    std::string getCurrentData() const;
    bool isCurrentNodeAnswer() const;
    Node* getCurrentNode() const;

    void learnNewObject(const std::string& newObject,const std::string& newQuestion,bool answerForNew);
    void buildFromJson(Node* newRoot);

private:
    size_t calculateNodeCount(Node* node) const;
};



#endif //AKINATOR_PROJECT_KNOWLEDGETREE_H