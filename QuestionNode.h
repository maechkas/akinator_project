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
}


#endif //PROJECT_QUESTIONNODE_H