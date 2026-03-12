//
// Created by София on 10.03.2026.
//

#ifndef AKINATOR_PROJECT_GAMESESSION_H
#define AKINATOR_PROJECT_GAMESESSION_H

#include "Player.h"
#include "KnowledgeTree.hpp"
#include <memory> //умные указатели
#include <iostream>
#include <string>

class GameSession {
private:
    KnowledgeTree& tree_; //ссылка на дерево которое создавала ангелина
    std::shared_ptr<Player> player_;
    int questions_asked_{0};  // сколько задано было вопросов
    bool game_active_{false}; // активна ли игра


    // Константы времени компиляции (constexpr!)
    constexpr static int MAX_QUESTIONS = 50;   // максимум вопросов за игру
    constexpr static int WIN_SCORE = 1;        // очки за победу

public:
    GameSession(KnowledgeTree& tree, std::shared_ptr<Player> player);

    //основные методы
    void startNewGame();  //начать новую игру
    std::string getCurrentQuestion() const; //получить вопрос который акинатор задал
    void processAnswer(bool answer); //обработка ответа на вопрос - да или нет
    bool isGameFinished() const;    //закончена ли игра

    //проверка и обучение
    bool checkGuess(); //проверка угадал ли акинатор
    void learnMode(); //если не угадал - режим обучения
    void surrender(); //сдаться и сразу учиться

    int amountOfQuestionsAsked() const {return questions_asked_;}
    std::shared_ptr<Player> getPlayer() const {return player_;}

    //когда ошибки - исключения
    class GameException: public std::runtime_error {
    public:
        explicit GameException(const std::string& message): std::runtime_error("Game error: " + message){};
    };
};


#endif //AKINATOR_PROJECT_GAMESESSION_H