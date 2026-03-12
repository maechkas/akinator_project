  #include "QuestionNode.hpp"

QuestionNode::QuestionNode(const std::string& data): Node(data), yesBranch_(nullptr), noBranch_(nullptr) {}

Node* QuestionNode::getYesBranch() const { return yesBranch_.get();}
Node* QuestionNode::getNoBranch() const { return noBranch_.get();}

void QuestionNode::setYesBranch(std::unique_ptr<Node> yes) {
    yesBranch_ = std::move(yes);
    if (yesBranch_) {
        yesBranch_->setParent(this);
    }
}
void QuestionNode::setNoBranch(std::unique_ptr<Node> no) {
    noBranch_ = std::move(no);
    if (noBranch_) {
        noBranch_->setParent(this);
    }
}

std::unique_ptr<Node> QuestionNode::releaseYesBranch() {
    if (yesBranch_) {
        yesBranch_->setParent(nullptr);
    }
    return std::move(yesBranch_);
}
std::unique_ptr<Node> QuestionNode::releaseNoBranch() {
    if (noBranch_) {
        noBranch_->setParent(nullptr);
    }
    return std::move(noBranch_);
}

bool QuestionNode::hasYesBranch() const { return yesBranch_ != nullptr; }
bool QuestionNode::hasNoBranch() const { return noBranch_ != nullptr; }
bool QuestionNode::isLeaf() const  { return !hasYesBranch() && !hasNoBranch(); }
bool QuestionNode::isAnswer() const {return false;}