#ifndef PROJECT_QUESTIONNODE_H
#define PROJECT_QUESTIONNODE_H

#include <string>
#include <memory>
class QuestionNode {
private:
    std::string question_;
    bool isAnswer_;
    std::unique_ptr<QuestionNode> yes_;
    std::unique_ptr<QuestionNode> no_;
    QuestionNode* parent_;
public:
    QuestionNode(const std::string& question, bool isAnswer);

    const std::string& getData() const;
    bool isAnswerNode() const;
    QuestionNode* getYesBranch() const;
    QuestionNode* getNoBranch() const;
    QuestionNode* getParent() const;

    void setYesBranch(std::unique_ptr<QuestionNode> yes);
    void setNoBranch(std::unique_ptr<QuestionNode> no);
    void setParent(QuestionNode* parent);

}


#endif //PROJECT_QUESTIONNODE_H