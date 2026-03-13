#include <iostream>
#include "GameSession.h"
#include <string>

GameSession::GameSession(KnowledgeTree& tree, std::shared_ptr<Player> player):
tree_(tree), player_(std::move(player)) {
    startNewGame();
}

void GameSession::startNewGame() {
    tree_.resetGame();
    questions_asked_= 0;
    game_active_ = true;
    player_->incrementGamesPlayed();
}

//-------ПОВЕДЕНИЕ АКТИНАТОРА------//
std::string GameSession::getCurrentQuestion() const {
    if (!game_active_) {
        throw GameException("the game is not active right now, but if you want to play, you can start it");
    }

    const auto* current = tree_.getCurrentNode();  // метод Ангелины

    if (!current) {
        throw GameException("there is no node((");
    }

    if (current->isAnswer()) {
        return "the gods whispered to me, you were thinking about: " + current->getData() + ", write 'yes' if I'm right, and 'no' if I'm wrong";
    }
    else {
        return current->getData();
    }
}

//-------ОБРАБОТКА ОТВЕТОВ ИГРОКА------//
void GameSession::processAnswer(bool answer) {
    if (!game_active_) {
        throw GameException("the game is not active right now, but if you want to play, you can start it");
    }
    questions_asked_++;

    if (questions_asked_ > MAX_QUESTIONS) {
        throw GameException("the actor is burned out, we'll play later");
    }

    if (answer == true) {
        tree_.moveYes();
    }
    else {
        tree_.moveNo();
    }

    if (tree_.isCurrentNodeAnswer()) {
        game_active_ = false;
    }
}

bool GameSession::isGameFinished() const {
    return !game_active_;
}

//акинатор дошел до предположения
bool GameSession::checkGuess() {
    if (!tree_.isCurrentNodeAnswer()) {
        return false;
    }
    std::string answer = tree_.getCurrentNode()->getData();  // предположение
    std::cout << "the gods whispered to me, you were thinking about: " + answer + ", write 'yes' if I'm right, and 'no' if I'm wrong";
    std::string userInput;
    std::getline(std::cin, userInput);
    if (userInput == "да" || userInput == "д" || userInput == "yes" || userInput == "if" || userInput == "ye" || userInput == "дп" ) {
        player_->won();
        return true;
    }
    else {
        std::cout << "I give up! Please share your wisdom with me ";
        learnMode();
        player_->lost();
        return false;
    }
}

//обучение
void GameSession::learnMode(){
    std::string oldAnswer = tree_.getCurrentNode()->getData();
    std::cout << "Tell me, what was on your mind? ";
    std::string newObject;
    std::getline(std::cin, newObject);

    std::cout << "what question (with a yes/no answer) did I need to ask to distinguish " + newObject + " from " + oldAnswer +"? ";
    std::string newQuestion;
    std::getline(std::cin, newQuestion);
    std::cout << "for " + newObject + " the answer is 'yes' or 'no'? ";
    std::string answer;
    std::getline(std::cin, answer);

    bool answerForNew = (answer == "да" || answer == "д" || answer == "yes" || answer == "if" || answer == "ye" || answer == "дп" );
    tree_.learnNewObject(newObject, newQuestion, answerForNew);
    std::cout << "Thank you, O wise man/woman! Now I know about " + newObject +"!" << std::endl;
}

//игрок сдался
void GameSession::surrender() {
    if (!game_active_) {
        std::cout << "the game is not active right now" << std::endl;
        return;
    }
    std::cout << "Are you giving up? Tell me, what were you thinking?" <<std::endl;
    learnMode();
    player_->lost();
    game_active_ = false;
}