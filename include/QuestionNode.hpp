#ifndef PROJECT_QUESTIONNODE_H
#define PROJECT_QUESTIONNODE_H

#include "Node.hpp"
#include <string>
#include <memory>

class QuestionNode: public Node {
private:
    std::unique_ptr<Node> yesBranch_;
    std::unique_ptr<Node> noBranch_;

public:
    explicit QuestionNode(const std::string& data);
    ~QuestionNode() override = default;

    QuestionNode(const QuestionNode&) = delete;
    QuestionNode& operator=(const QuestionNode&) = delete;
    QuestionNode(QuestionNode&&) = default;
    QuestionNode& operator=(QuestionNode&&) = default;

    Node* getYesBranch() const override;
    Node* getNoBranch() const override;

    std::unique_ptr<Node> releaseYesBranch();
    std::unique_ptr<Node> releaseNoBranch();

    void setYesBranch(std::unique_ptr<Node> yes);
    void setNoBranch(std::unique_ptr<Node> no);

    bool isAnswer() const override;
    bool hasYesBranch() const override;
    bool hasNoBranch() const override;
    bool isLeaf() const;

};


#endif //PROJECT_QUESTIONNODE_H