#include <iostream>
#include <memory>
#include <string>

#include "Player.h"
#include "GameSession.h"
#include "KnowledgeTree.hpp"
#include "FileHandler.h"
#include "tree_serializer.h"

int main() {
    try {
        std::cout << "╔════════════════════════════════╗\n";
        std::cout << "║          🌟AKINATOR 🌟         ║\n";
        std::cout << "║   the best game in your life   ║\n";
        std::cout << "╚════════════════════════════════╝\n\n";

        // создаём дерево знаний
        auto tree = std::make_unique<KnowledgeTree>();

        // создаём или загружаем игрока
        std::cout << "Your name: ";
        std::string playerName;
        std::getline(std::cin, playerName);

        auto player = std::make_shared<Player>(playerName);

        // пытаемся загрузить статистику игрока
        std::string statsFile = playerName + "_stats.txt";
        try {
            player->loadFromFile(statsFile);
            std::cout << "Statistics loaded! Welcome back, " << playerName << "!\n";
        } catch (const std::exception& e) {
            std::cout << "A new player! Welcome, dear " << playerName << "!\n";
        }

//      начинаем игровой цикл
        bool playAgain = true;

        while (playAgain) {
            GameSession session(*tree, player);

            std::cout << "\n--- A NEW GAME ---\n";

            // цикл - пока не дойдём до ответа
            while (!session.isGameFinished()) {
                try {
                    std::cout << "\nAKINATOR: " << session.getCurrentQuestion() << "\n";

                    // Получаем ответ от игрока
                    std::cout << "Your answer(yes/no/give up): ";
                    std::string input;
                    std::getline(std::cin, input);

                    // Проверка на сдачу
                    if (input == "сдаться" || input == "сдаюсь" || input == "give up") {
                        session.surrender();
                        break;
                    }

                    // Обработка ответа
                    bool answer = (input == "да" || input == "yes" || input == "д" || input == "y");
                    session.processAnswer(answer);

                } catch (const GameSession::GameException& e) {
                    std::cout << "game error: " << e.what() << "\n";
                    break;
                }
            }

            if (session.isGameFinished()) {
                std::cout << "\n";
                if (session.checkGuess()) {
                    std::cout << "🎉AKINATOR: Hooray! I got it again!🎉🎉🎉\n";
                } else {
                    std::cout << "AKINATOR: I've become smarter now!🤔🤔🤔\n";
                }
            }

            // Показываем статистику
            std::cout << "\n";
            player->printStats();

            // Спрашиваем, хочет ли игрок продолжать
            std::cout << "\nShall we play again? (yes/no): ";
            std::string again;
            std::getline(std::cin, again);
            playAgain = (again == "да" || again == "yes" || again == "д" || again == "y");
        }

        // СОХРАНЕНИЕ ПЕРЕД ВЫХОДОМ
        try {
            player->saveToFile(statsFile);
            std::cout << "Statistics saved. Goodbye, " << playerName << "!\n";
        } catch (const std::exception& e) {
            std::cout << "Couldn't save statistics: " << e.what() << "\n";
        }

        //СОХРАНЕНИЕ ДЕРЕВА
        try {
            TreeSerializer serializer;
            std::string jsonTree = serializer.serialize(*tree);

            FileHandler fileHandler("saves");
            fileHandler.writeFile("knowledge_tree.json", jsonTree);
            std::cout << "The tree of knowledge has been preserved.\n";
        } catch (const std::exception& e) {
            std::cout << "Couldn't save the tree: " << e.what() << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "\n❌ CRITICAL ERROR: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nSee you soon! 💗💗💗\n";
}