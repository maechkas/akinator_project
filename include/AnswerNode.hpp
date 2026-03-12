#ifndef PROJECT_ANSWERNODE_H
#define PROJECT_ANSWERNODE_H

#include "Node.hpp"
#include <string>
#include <memory>

class AnswerNode : public Node {
public:
    explicit AnswerNode(const std::string& data);
    ~AnswerNode() override = default;

    AnswerNode(const AnswerNode&) = delete;
    AnswerNode& operator=(const AnswerNode&) = delete;
    AnswerNode(AnswerNode&&) = default;
    AnswerNode& operator=(AnswerNode&&) = default;

    bool isAnswer() const override;
    bool hasYesBranch() const override;
    bool hasNoBranch() const override;
};

#endif //PROJECT_ANSWERNODE_H