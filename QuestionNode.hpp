#ifndef PROJECT_QUESTIONNODE_H
#define PROJECT_QUESTIONNODE_H

#include <string>
#include <memory>

class QuestionNode {
private:
    std::string data_;
    bool isAnswer_;
    std::unique_ptr<QuestionNode> yesBranch_;
    std::unique_ptr<QuestionNode> noBranch_;
    QuestionNode* parent_;
public:
    explicit QuestionNode(const std::string& data, bool isAnswer);

    QuestionNode(const QuestionNode&) = delete;
    QuestionNode& operator=(const QuestionNode&) = delete;
    QuestionNode(QuestionNode&&) = default;
    QuestionNode& operator=(QuestionNode&&) = default;

    const std::string& getData() const;
    bool isAnswerNode() const;
    QuestionNode* getYesBranch() const;
    QuestionNode* getNoBranch() const;
    QuestionNode* getParent() const;

    void setYesBranch(std::unique_ptr<QuestionNode> yes);
    void setNoBranch(std::unique_ptr<QuestionNode> no);
    void setParent(QuestionNode* parent);

    bool hasYesBranch() const;
    bool hasNoBranch() const;
    bool isLeaf() const;
    void replaceWithQuestion(const std::string& newQuestion, std::unique_ptr<QuestionNode> newYesAnswer,std::unique_ptr<QuestionNode> newNoAnswer);

};


#endif //PROJECT_QUESTIONNODE_H