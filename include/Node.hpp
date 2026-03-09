#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include <string>

class Node {
protected:
    std::string data_;
    Node* parent_;

public:
    explicit Node(const std::string& data);
    virtual ~Node() = default;

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = default;
    Node& operator=(Node&&) = default;

    const std::string& getData() const;
    Node* getParent() const;
    void setParent(Node* parent);

    virtual bool isAnswer() const = 0;
    virtual bool hasYesBranch() const = 0;
    virtual bool hasNoBranch() const = 0;

    virtual Node* getYesBranch() const;
    virtual Node* getNoBranch() const;
};

#endif //PROJECT_NODE_H