#include <iostream>
#include "GameSession.h"
#include <string>

GameSession::GameSession(KnowledgeTree& tree, std::shared_ptr<Player> player):
tree_(tree), player_(std::move(player)) {
    startNewGame();
}

void GameSession::startNewGame() {
    tree_.resetToRoot();        //метод Ангелины, встаем в начала дерева вопросов (в корень)
    questions_asked_= 0;
    game_active_ = true;
    player_->incrementGamesPlayed();
}



//-------ПОВЕДЕНИЕ АКТИНАТОРА------//
std::string GameSession::getCurrentQuestion() const {
    if (!game_active_) {
        throw GameException("сейчас игра не активна, хочешь сыграть - запусти");
    }

    const auto* current = tree_.getCurrentNode();  //ангелинин метод для получения текущего узла

    if (!current) {
        throw GameException("узла нет((");
    }

    if (current->isAnswer()) {          //дошли до предположения - выводим его
        return "боги нашептали мне, ты загадал: " + current->getData() + ", напиши 'да' - если я прав, 'нет' - если я ошибся";
    }
    else {                              //если еще не дошли до догадки и это вопрос - возвращаем этот вопрос
       return current->getData();
    }
}



//-------ОБРАБОТКА ОТВЕТОВ ИГРОКА------//
void GameSession::processAnswer(bool answer) {
    if (!game_active_) {
        throw GameException("сейчас игра не активна, хочешь сыграть - запусти");
    }
    questions_asked_++;

    if (questions_asked_ > MAX_QUESTIONS) {
        throw GameException("у акинатора выгорание, сыграем позже"); //защита на случай неправильной связи узлов -> рекурсия вопросов
    }

    if (answer == true) {
        tree_.moveToYes();
    }
    else {
        tree_.moveToNo();
    }

    if (tree_.isCurrentNodeAnswer()) { //если узел дошел до предположения - завершаем игру
        game_active_ = false;
    }
}

bool GameSession::isGameFinished() const {
    if (!game_active_) {
        return true;
    }
    return false;
}


//акинатор дошел до предположения
bool GameSession::checkGuess() {
    if (!tree_.isCurrentNodeAnswer()) {
        return false;
    }
    std::string answer = tree_.getCurrentNode()->getData();  // предположение
    std::cout << "боги нашептали мне, ты загадал: " + answer + ", напиши 'да' - если я прав, 'нет' - если я ошибся";
    std::string userInput;
    std::getline(std::cin, userInput);
    if (userInput == "да" || userInput == "д" || userInput == "yes" || userInput == "if" || userInput == "ye" || userInput == "дп" ) {
        player_->won();
        return true;
    }
    else {
        std::cout << "сдаюсь! прошу, поделись со мной своей мудростью";
        learnMode();
        player_->lost();
        return false;
    }
}



//обучение
void GameSession::learnMode(){
    std::string oldAnswer = tree_.getCurrentNode()->getData(); //сохраняем в каком узле(ответе) был комп
    std::cout << "поведай же, что было на твоем уме? ";
    std::string newObject;
    std::getline(std::cin, newObject);

    std::cout << "какой вопрос (с ответом да/нет) мне нужно было задать, чтобы отличить " + newObject + " от " + oldAnswer +"? ";
    std::string newQuestion;
    std::getline(std::cin, newQuestion);
    std::cout << "для " + newObject + " ответ 'да' или 'нет'? ";
    std::string answer;
    std::getline(std::cin, answer);

    bool answerForNew = (answer == "да" || answer == "д" || answer == "yes" || answer == "if" || answer == "ye" || answer == "дп" );
    tree_.learnNewObject(newObject, newQuestion, answerForNew);
    std::cout << "спасибо тебе, о мудрый человек! теперь я знаю о " + newObject +"!" << std::endl;
}


//игрок сдался
void GameSession::surrender() {
    if (!game_active_) {
        std::cout << "сейчас игра не активна" << std::endl;
        return;
    }
    std::cout << "сдаешься? расскажи, что ты загадал(а)?" <<std::endl;
    learnMode();
    player_->lost();
    game_active_ = false;

}




