//
// Created by София on 10.03.2026.
//

#ifndef AKINATOR_PROJECT_GAMESESSION_H
#define AKINATOR_PROJECT_GAMESESSION_H

#include "Player.h"
#include "KnowledgeTree.hpp"
#include <memory>
#include <iostream>
#include <string>

class GameSession {
private:
    KnowledgeTree& tree_; //ссылка на дерево которое создавала ангелина
    std::shared_ptr<Player> player_;
    int questions_asked_{0};
    bool game_active_{false};


    // Константы времени компиляции (constexpr!)
    constexpr static int MAX_QUESTIONS = 50;
    constexpr static int WIN_SCORE = 1;

public:
    GameSession(KnowledgeTree& tree, std::shared_ptr<Player> player);

    //основные методы
    void startNewGame();
    std::string getCurrentQuestion() const;
    void processAnswer(bool answer);
    bool isGameFinished() const;

    //проверка и обучение
    bool checkGuess();
    void learnMode();
    void surrender();

    int amountOfQuestionsAsked() const {return questions_asked_;}
    std::shared_ptr<Player> getPlayer() const {return player_;}

    //когда ошибки - исключения
    class GameException: public std::runtime_error {
    public:
        explicit GameException(const std::string& message): std::runtime_error("Game error: " + message){};
    };
};


#endif //AKINATOR_PROJECT_GAMESESSION_H