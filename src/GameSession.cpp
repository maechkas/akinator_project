#include <iostream>
#include "GameSession.h"
#include <string>

// ========== CONSTRUCTOR ==========
GameSession::GameSession(KnowledgeTree& tree, std::shared_ptr<Player> player)
    : tree_(tree), player_(std::move(player)) {
    startNewGame();
}

// ========== START NEW GAME ==========
void GameSession::startNewGame() {
    tree_.resetGame();
    questions_asked_ = 0;
    game_active_ = true;
}

// ========== GET CURRENT QUESTION ==========
std::string GameSession::getCurrentQuestion() const {
    if (!game_active_) {
        return "";
    }

    const auto* current = tree_.getCurrentNode();
    if (!current) {
        throw GameException("Error: Current node is null.");
    }

    if (current->isAnswer()) {
        return "Is it \"" + current->getData() + "\"? (yes/no)";
    } else {
        return current->getData() + "? (yes/no)";
    }
}

// ========== PROCESS PLAYER ANSWER ==========
void GameSession::processAnswer(bool answer) {
    if (!game_active_) {
        throw GameException("Game is not active. Please start a new game.");
    }

    questions_asked_++;
    if (questions_asked_ > MAX_QUESTIONS) {
        throw GameException("Too many questions. Game over.");
    }

    if (answer) {
        tree_.moveYes();
    } else {
        tree_.moveNo();
    }

    const auto* current = tree_.getCurrentNode();
    if (!current || current->isAnswer()) {
        game_active_ = false;
    }
}

// ========== IS GAME FINISHED ==========
bool GameSession::isGameFinished() const {
    return !game_active_;
}

// ========== CHECK GUESS ==========
bool GameSession::checkGuess() {
    if (!tree_.isCurrentNodeAnswer()) {
        return false;
    }

    std::string answer = tree_.getCurrentNode()->getData();
    std::cout << "\n🤔 Akinator thinks: Is it \"" << answer << "\"? (yes/no): ";

    std::string userInput;
    std::getline(std::cin, userInput);

    // ✅ Fixed: proper OR operators
    if (userInput == "yes" || userInput == "y" ||
        userInput == "да" || userInput == "д") {
        player_->won();
        std::cout << "🎉 AKINATOR WINS!\n";
        return true;
    } else {
        std::cout << "😢 Missed! Teach me...\n";
        learnMode();
        player_->lost();
        return false;
    }
}

// ========== LEARNING MODE ==========
void GameSession::learnMode() {
    std::string oldAnswer = tree_.getCurrentNode()->getData();
    std::cout << "What did you think of? ";
    std::string newObject;
    std::getline(std::cin, newObject);

    std::cout << "Question to distinguish " << newObject << " from " << oldAnswer << "? ";
    std::string newQuestion;
    std::getline(std::cin, newQuestion);

    std::cout << "'" << newObject << "' answer is 'yes'? (yes/no) ";
    std::string response;
    std::getline(std::cin, response);

    // ✅ Fixed: proper OR operators
    bool answerForNew = (response == "yes" || response == "y" ||
                        response == "да" || response == "д");

    tree_.learnNewObject(newObject, newQuestion, answerForNew);
    std::cout << "✅ Learned! Tree updated!\n";
}

// ========== SURRENDER ==========
void GameSession::surrender() {
    if (!game_active_) {
        std::cout << "Game not active.\n";
        return;
    }
    std::cout << "Giving up? Teach me!\n";
    learnMode();
    player_->lost();
    game_active_ = false;
}
