#include <gtest/gtest.h>
#include "EntitySearcher.h"
#include "QuestionNode.hpp"
#include "AnswerNode.hpp"
#include "Node.hpp"

// вспомогательная функция, которая создает пробное дерево
std::unique_ptr <Node> createTestTree(){
    auto papa = std::make_unique<AnswerNode>("Dad");
    auto mama = std::make_unique<AnswerNode>("Mother");
    auto cat = std::make_unique<AnswerNode>("Cat");
    auto tiger = std::make_unique<AnswerNode>("Tiger");

    auto manQuestion = std::make_unique<QuestionNode>("Is it a man?");
    manQuestion->setYesBranch(std::move(papa));
    manQuestion->setNoBranch(std::move(mama));

    auto animalQuestion = std::make_unique<QuestionNode>("Is it a domestic animal?");
    animalQuestion->setYesBranch(std::move(cat));
    animalQuestion->setNoBranch(std::move(tiger));

    auto root = std::make_unique<QuestionNode>("Is it a human?");
    root->setYesBranch(std::move(manQuestion));
    root->setNoBranch(std::move(animalQuestion));

    return root;
}

//findAllPathsToObject
TEST (EntitySearcher, FindAllPathsToObject_Dad){
    auto root = createTestTree();
    auto path= EntitySearcher::findAllPathsToObject(root.get() , "Dad");
    EXPECT_EQ(path[0][0], "Is it a human?");
    EXPECT_EQ(path[0][1], "Yes");
    EXPECT_EQ(path[0][2], "Is it a man?");
    EXPECT_EQ(path[0][3], "Yes");
    EXPECT_EQ(path[0][4], "Dad");
}

TEST (EntitySearcher, FindAllPathsToObject_Mother){
    auto root = createTestTree();
    auto path= EntitySearcher::findAllPathsToObject(root.get() , "Mother");
    EXPECT_EQ(path[0][0], "Is it a human?");
    EXPECT_EQ(path[0][1], "Yes");
    EXPECT_EQ(path[0][2], "Is it a man?");
    EXPECT_EQ(path[0][3], "No");
    EXPECT_EQ(path[0][4], "Mother");
}

TEST (EntitySearcher, FindAllPathsToObject_Tiger){
    auto root = createTestTree();
    auto path= EntitySearcher::findAllPathsToObject(root.get() , "Tiger");
    EXPECT_EQ(path[0][0], "Is it a human?");
    EXPECT_EQ(path[0][1], "No");
    EXPECT_EQ(path[0][2], "Is it a domestic animal?");
    EXPECT_EQ(path[0][3], "No");
    EXPECT_EQ(path[0][4], "Tiger");
}

TEST (EntitySearcher, FindAllPathsToObject_Cat){
    auto root = createTestTree();
    auto path= EntitySearcher::findAllPathsToObject(root.get() , "Cat");
    EXPECT_EQ(path[0][0], "Is it a human?");
    EXPECT_EQ(path[0][1], "No");
    EXPECT_EQ(path[0][2], "Is it a domestic animal?");
    EXPECT_EQ(path[0][3], "Yes");
    EXPECT_EQ(path[0][4], "Cat");
}

//objectExists
TEST(EntitySearcher, objectExists_ReturnsTrue){
    auto root = createTestTree();
    EXPECT_TRUE(EntitySearcher::objectExists(root.get(), "Dad"));
    EXPECT_TRUE(EntitySearcher::objectExists(root.get(), "Mother"));
    EXPECT_TRUE(EntitySearcher::objectExists(root.get(), "Cat"));
    EXPECT_TRUE(EntitySearcher::objectExists(root.get(), "Tiger"));
    EXPECT_FALSE(EntitySearcher::objectExists(root.get(), "Dragon"));
    EXPECT_FALSE(EntitySearcher::objectExists(root.get(), ""));
}

TEST(EntitySearcher, objectExists_ReturnsFalse){
    auto root = createTestTree();
    EXPECT_FALSE(EntitySearcher::objectExists(root.get(), "Dragon"));
    EXPECT_FALSE(EntitySearcher::objectExists(root.get(), ""));
}

//findAllPathsToObject
TEST(EntitySearcher, getAllObjects){
    auto root = createTestTree();
    auto objects = EntitySearcher::getAllObjects(root.get());
    EXPECT_NE(std::find(objects.begin(), objects.end(),"Dad"),objects.end());
    EXPECT_NE(std::find(objects.begin(), objects.end(),"Mother"),objects.end());
    EXPECT_NE(std::find(objects.begin(), objects.end(),"Cat"),objects.end());
    EXPECT_NE(std::find(objects.begin(), objects.end(),"Tiger"),objects.end());
}

//findObjectByAnswers
TEST(EntitySearcher, findObjectByAnswers){
    auto root= createTestTree() ;
    auto object_dad = EntitySearcher::findObjectByAnswers(root.get(), {true, true});
    EXPECT_EQ(object_dad.value(),"Dad") ;
    auto object_mother = EntitySearcher::findObjectByAnswers(root.get(), {true, false});
    EXPECT_EQ(object_mother.value(),"Mother") ;
    auto object_cat = EntitySearcher::findObjectByAnswers(root.get(), {false, true});
    EXPECT_EQ(object_cat.value(),"Cat") ;
    auto object_tiger = EntitySearcher::findObjectByAnswers(root.get(), {false, false});
    EXPECT_EQ(object_tiger.value(),"Tiger") ;
}