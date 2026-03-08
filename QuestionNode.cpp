#include "QuestionNode.hpp"

QuestionNode::QuestionNode(const std::string& data, bool isAnswer):data_(data), isAnswer_(isAnswer),yesBranch_(nullptr), noBranch_(nullptr), parent_(nullptr) {}

const std::string& QuestionNode::getData() const { return data_;}
bool QuestionNode::isAnswerNode() const { return isAnswer_;}
QuestionNode* QuestionNode::getYesBranch() const { return yesBranch_.get();}
QuestionNode* QuestionNode::getNoBranch() const { return noBranch_.get();}
QuestionNode* QuestionNode::getParent() const { return parent_;}

void QuestionNode::setYesBranch(std::unique_ptr<QuestionNode> yes) {
    yesBranch_ = std::move(yes);
    if (yesBranch_) {
        yesBranch_->setParent(this);
    }
}
void QuestionNode::setNoBranch(std::unique_ptr<QuestionNode> no) {
    noBranch_ = std::move(no);
    if (noBranch_) {
        noBranch_->setParent(this);
    }
}
void QuestionNode::setParent(QuestionNode* parent) {
    parent_ = parent;
}

bool QuestionNode::hasYesBranch() const { return yesBranch_ != nullptr; }
bool QuestionNode::hasNoBranch() const { return noBranch_ != nullptr; }
bool QuestionNode::isLeaf() const  { return !hasYesBranch() && !hasNoBranch(); }

void QuestionNode::replaceWithQuestion(const std::string& newQuestion, std::unique_ptr<QuestionNode> newYesAnswer,std::unique_ptr<QuestionNode> newNoAnswer) {
    data_ = newQuestion;
    isAnswer_ = false;
    if (newYesAnswer) {
        newYesAnswer->setParent(this);
    }
    if (newNoAnswer) {
        newNoAnswer->setParent(this);
    }
    yesBranch_ = std::move(newYesAnswer);
    noBranch_ = std::move(newNoAnswer);
}
