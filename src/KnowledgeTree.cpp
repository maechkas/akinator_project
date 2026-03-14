#include "../include/KnowledgeTree.hpp"
#include <functional>

KnowledgeTree::KnowledgeTree():root_(nullptr),currentNode_(nullptr), nodeCount_(0) {

    std::unique_ptr<QuestionNode> root = std::make_unique<QuestionNode>("Is it a human?");
    std::unique_ptr<QuestionNode> noHuman_animal = std::make_unique<QuestionNode>("Is it an animal?");
    std::unique_ptr<QuestionNode> yesHuman_real = std::make_unique<QuestionNode>("Is it a real person?");

    std::unique_ptr<AnswerNode> noAnimal = std::make_unique<AnswerNode>("Cucumber");
    std::unique_ptr<AnswerNode> yesAnimal = std::make_unique<AnswerNode>("Fox");

    std::unique_ptr<AnswerNode> noReal = std::make_unique<AnswerNode>("Harry Potter");
    std::unique_ptr<AnswerNode> yesReal= std::make_unique<AnswerNode>("A.S.Pushkin");

    yesHuman_real->setYesBranch(std::move(yesReal));
    yesHuman_real->setNoBranch(std::move(noReal));

    noHuman_animal->setYesBranch(std::move(yesAnimal));
    noHuman_animal->setNoBranch(std::move(noAnimal));

    root->setNoBranch(std::move(noHuman_animal));
    root->setYesBranch(std::move(yesHuman_real));

    root_ = std::move(root);
    currentNode_ = root_.get();
    nodeCount_ = 7;
}
Node* KnowledgeTree::getRoot() const {return root_.get();}
size_t KnowledgeTree::getNodeCount() const {return nodeCount_;}
bool KnowledgeTree::isEmpty() const {return root_ == nullptr;}

bool KnowledgeTree::hasObject(const std::string& objectName) const {
    if (!root_) {
        return false;
    }
    std::function<bool(Node*)> search = [&](Node* node)->bool {
        if (!node) return false;

        if (node->isAnswer() && node->getData() == objectName) {
            return true;
        }
        if (node->hasYesBranch() && search(node->getYesBranch())) {
            return true;
        }
        if (node->hasNoBranch() && search(node->getNoBranch())) {
            return true;
        }
        return false;
    };
    return search(root_.get());
}

void KnowledgeTree::resetGame() {
    if (!root_) {
        throw std::runtime_error("Tree is empty");
    }
    currentNode_ = root_.get();
}
void KnowledgeTree::moveYes(){
    if (!currentNode_) {
        throw std::runtime_error("No current node");
    }
    if (!currentNode_->hasYesBranch()) {
        throw std::runtime_error("No YES branch from: " + currentNode_->getData());
    }
    currentNode_ = currentNode_->getYesBranch();
}
void KnowledgeTree::moveNo() {
    if (!currentNode_) {
        throw std::runtime_error("No current node");
    }
    if (!currentNode_->hasNoBranch()) {
        throw std::runtime_error("No NO branch from: " + currentNode_->getData());
    }
    currentNode_ = currentNode_->getNoBranch();
}

std::string KnowledgeTree::getCurrentData() const {
    if (!currentNode_) {
        throw std::runtime_error("No current node");
    }
    return currentNode_->getData();
}
bool KnowledgeTree::isCurrentNodeAnswer() const {
    if (!currentNode_) {
        throw std::runtime_error("No current node");
    }
   return currentNode_->isAnswer();
}
Node* KnowledgeTree::getCurrentNode() const {
    if (!currentNode_) {
        throw std::runtime_error("No current node");
    }
    return currentNode_;
}

void KnowledgeTree::learnNewObject(const std::string& newObject, const std::string& newQuestion, bool answerForNew) {
    if (!currentNode_ || !(currentNode_->isAnswer())) {
        throw std::runtime_error("Not answer or no current node");
    }

    std::string oldObject = currentNode_->getData();
    Node* parent = currentNode_->getParent();
    Node* oldNodePtr = currentNode_;

    bool isRootNode = (parent == nullptr);

    auto newQuestionNode = std::make_unique<QuestionNode>(newQuestion);
    Node* newQuestionPtr = newQuestionNode.get();

    auto oldAnswerNode = std::make_unique<AnswerNode>(oldObject);
    auto newAnswerNode = std::make_unique<AnswerNode>(newObject);

    if (answerForNew) {
        newQuestionNode->setYesBranch(std::move(newAnswerNode));
        newQuestionNode->setNoBranch(std::move(oldAnswerNode));
    } else {
        newQuestionNode->setYesBranch(std::move(oldAnswerNode));
        newQuestionNode->setNoBranch(std::move(newAnswerNode));
    }

    if (isRootNode) {
        root_ = std::move(newQuestionNode);
        currentNode_ = root_.get();
    }
    else {
        QuestionNode* parentQuestion = dynamic_cast<QuestionNode*>(parent);
        if (!parentQuestion) {
            throw std::runtime_error("Parent must be a question node");
        }

        if (parentQuestion->getYesBranch() == oldNodePtr) {
            parentQuestion->releaseYesBranch();
            parentQuestion->setYesBranch(std::move(newQuestionNode));
        }
        else if (parentQuestion->getNoBranch() == oldNodePtr) {
            parentQuestion->releaseNoBranch();
            parentQuestion->setNoBranch(std::move(newQuestionNode));
        }
        else {
            throw std::runtime_error("Current node not found in parent's branches");
        }
        currentNode_ = newQuestionPtr;
    }

    nodeCount_ += 2;
}
void KnowledgeTree::buildFromJson(std::unique_ptr<Node> newRoot){
    if (!newRoot) {
        throw std::runtime_error("Cannot build from null root");
    }
    root_ = std::move(newRoot);
    currentNode_ = root_.get();
    nodeCount_ = calculateNodeCount(root_.get());
}
size_t KnowledgeTree::calculateNodeCount(Node* node) const {
    if (!node){ return 0;}
    size_t nodeCount = 1;
    if (node->hasYesBranch()) {
        nodeCount+=calculateNodeCount(node->getYesBranch());
    }
    if (node->hasNoBranch()) {
        nodeCount += calculateNodeCount(node->getNoBranch());
    }
    return nodeCount;
}
