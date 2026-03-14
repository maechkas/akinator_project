#include <gtest/gtest.h>
#include "GameSession.h"
#include "Player.h"
#include "KnowledgeTree.hpp"
#include <memory>

// ТЕСТ 1 - создание сессии
TEST(GameSessionTest, CreateSession) {
    KnowledgeTree tree;
    auto player = std::make_shared<Player>("Sofia");

    GameSession session(tree, player);

    EXPECT_EQ(session.amountOfQuestionsAsked(), 0);
    EXPECT_FALSE(session.isGameFinished());
}

//ТЕСТ 2 - получение вопроса
TEST(GameSessionTest, GetQuestion) {
    KnowledgeTree tree;
    auto player = std::make_shared<Player>("Sofia");

    GameSession session(tree, player);

    EXPECT_NO_THROW({
        std::string q = session.getCurrentQuestion();
    });
}

// ТЕСТ 4 - обработка ответа да
TEST(GameSessionTest, ProcessYesAnswer) {
    KnowledgeTree tree;
    auto player = std::make_shared<Player>("Sofia");

    GameSession session(tree, player);

    EXPECT_NO_THROW({
        session.processAnswer(true);
    });
    EXPECT_GT(session.amountOfQuestionsAsked(), 0);
}

// ТЕСТ 5 - обработка ответа нет
TEST(GameSessionTest, ProcessNoAnswer) {
    KnowledgeTree tree;
    auto player = std::make_shared<Player>("Sofia");

    GameSession session(tree, player);

    EXPECT_NO_THROW({
        session.processAnswer(false);
    });

    EXPECT_GT(session.amountOfQuestionsAsked(), 0);  //глупая ошибка

}