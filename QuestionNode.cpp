`#include "QuestionNode.hpp"

QuestionNode::QuestionNode(const std::string& question, bool isAnswer):question_(question), isAnswer_(isAnswer),yes_(nullptr), no_(nullptr), parent_(nullptr) {}

const std::string& QuestionNode::getData() const { return question_;}
bool QuestionNode::isAnswerNode() const { return isAnswer_;}
QuestionNode* QuestionNode::getYesBranch() const { return yes_.get();}
QuestionNode* QuestionNode::getNoBranch() const { return no_.get();}
QuestionNode* QuestionNode::getParent() const { return parent_;}

void QuestionNode::setYesBranch(std::unique_ptr<QuestionNode> yes) {
    yes_ = std::move(yes);
}
void QuestionNode::setNoBranch(std::unique_ptr<QuestionNode> no) {
    no_ = std::move(no);
}
void QuestionNode::setParent(QuestionNode* parent);