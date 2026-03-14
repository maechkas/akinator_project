#include <iostream>
#include <memory>
#include <string>

#include "Player.h"
#include "GameSession.h"
#include "KnowledgeTree.hpp"
#include "FileHandler.h"
#include "tree_serializer.h"
#include "EntitySearcher.h"

int main() {
    try {
        std::cout << "╔════════════════════════════════╗\n";
        std::cout << "║            AKINATOR            ║\n";
        std::cout << "║   the best game in your life   ║\n";
        std::cout << "╚════════════════════════════════╝\n\n";

        std::unique_ptr<KnowledgeTree> tree;
        FileHandler fileHandler("saves");
        TreeSerializer serializer;

        // Проверяем существование файла с сохранённым деревом
        if (fileHandler.fileExists("knowledge_tree.json")) {
            std::cout << " Loading knowledge tree from previous sessions...\n";

            auto loadedTree = serializer.LoadingFrom("saves/knowledge_tree.json");
            if (loadedTree && loadedTree->getRoot()) {
                tree = std::move(loadedTree);
                auto objects = EntitySearcher::getAllObjects(tree->getRoot());
                std::cout << " Tree loaded successfully! I know about "
                          << objects.size() << " objects.\n";
            } else {
                std::cout << " Failed to load tree, creating new one...\n";
                tree = std::make_unique<KnowledgeTree>();
            }
        } else {
            std::cout << " No saved tree found, creating new knowledge tree...\n";
            tree = std::make_unique<KnowledgeTree>();
        }

        auto initialObjects = EntitySearcher::getAllObjects(tree->getRoot());
        std::cout << " I know about: ";
        for (const auto& obj : initialObjects) {
            std::cout << obj << " ";
        }
        std::cout << "\n\n";

        // ИГРОК
        std::cout << " Enter your name: ";
        std::string playerName;
        std::getline(std::cin, playerName);

        auto player = std::make_shared<Player>(playerName);

        // Пытаемся загрузить статистику игрока
        std::string statsFile = playerName + "_stats.txt";
        try {
            player->loadFromFile(statsFile);
            std::cout << "Statistics loaded! Welcome back, " << playerName << "!\n";
        } catch (const std::exception& e) {
            std::cout << " A new player! Welcome, dear " << playerName << "!\n";
        }

        player->printStats();

        //ИГРОВОЙ ЦИКЛ
        bool playAgain = true;

        while (playAgain) {
            std::cout << "\n" << std::string(50, '=') << "\n";
            std::cout << " NEW GAME \n";
            std::cout << std::string(50, '=') << "\n";
            std::cout << " Think of something, and I'll try to guess it!\n";

            // Создаём новую игровую сессию
            GameSession session(*tree, player);

            // Основной игровой цикл - пока не дойдём до ответа
            while (!session.isGameFinished()) {
                try {
                    std::cout << "\n AKINATOR: " << session.getCurrentQuestion() << "\n";
                    std::cout << "Your answer (yes/no/give up): ";

                    std::string input;
                    std::getline(std::cin, input);

                    if (input == "сдаться" || input == "сдаюсь" || input == "give up") {
                        session.surrender();
                        break;
                    }

                    bool answer = (input == "да" || input == "yes" || input == "д" ||
                                  input == "y" || input == "lf" || input == "da");
                    session.processAnswer(answer);

                } catch (const GameSession::GameException& e) {
                    std::cout << " Game error: " << e.what() << "\n";
                    break;
                }
            }

            if (session.isGameFinished()) {
                std::cout << "\n" << std::string(40, '-') << "\n";
                if (session.checkGuess()) {
                    std::cout << " AKINATOR: Hooray! I got it again! 🎉\n";
                }
                std::cout << std::string(40, '-') << "\n";
            }

            std::cout << "\n";
            player->printStats();

            auto allObjects = EntitySearcher::getAllObjects(tree->getRoot());
            std::cout << " I now know about " << allObjects.size() << " things!\n";

            std::cout << "\nPlay again? (yes/no): ";
            std::string again;
            std::getline(std::cin, again);
            playAgain = (again == "да" || again == "yes" || again == "д" ||
                        again == "y" || again == "lf" || again == "da");
        }

        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << " SAVING...\n";

        try {
            player->saveToFile(statsFile);
            std::cout << " Player statistics saved!\n";
        } catch (const std::exception& e) {
            std::cout << " Couldn't save player statistics: " << e.what() << "\n";
        }

        // Сохраняем дерево знаний
        try {
            std::string jsonTree = serializer.serialize(*tree);
            if (fileHandler.writeFile("knowledge_tree.json", jsonTree)) {
                std::cout << " Knowledge tree saved!\n";

                auto savedObjects = EntitySearcher::getAllObjects(tree->getRoot());
                std::cout << " Tree has " << savedObjects.size() << " objects\n";
            } else {
                std::cout << " Failed to save knowledge tree!\n";
            }
        } catch (const std::exception& e) {
            std::cout << " Couldn't save knowledge tree: " << e.what() << "\n";
        }

        // Финальная статистика
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << " FINAL STATISTICS\n";
        player->printStats();

        auto finalObjects = EntitySearcher::getAllObjects(tree->getRoot());
        std::cout << "\n I know: ";
        for (size_t i = 0; i < finalObjects.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << finalObjects[i];
        }
        std::cout << "\n";

    } catch (const std::exception& e) {
        std::cerr << "\n CRITICAL ERROR: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << " Thanks for playing! \n";
    std::cout << std::string(50, '=') << "\n";

    return 0;
}