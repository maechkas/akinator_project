#include "AnswerNode.hpp"

AnswerNode::AnswerNode(const std::string& data) : Node(data) {}

bool AnswerNode::isAnswer() const {return true;}
bool AnswerNode::hasYesBranch() const {return false;}
bool AnswerNode::hasNoBranch() const {return false;}