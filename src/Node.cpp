#include "Node.hpp"

Node::Node(const std::string& data) : data_(data), parent_(nullptr) {}

const std::string& Node::getData() const {return data_;}
Node* Node::getParent() const {return parent_;}
Node* Node::getYesBranch() const {return nullptr;}
Node* Node::getNoBranch() const {return nullptr;}

void Node::setParent(Node* parent) {parent_ = parent;}
