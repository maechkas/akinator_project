#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include "Player.h"
#include "GameSession.h"
#include "KnowledgeTree.hpp"
#include "FileHandler.h"
#include "tree_serializer.h"
#include "EntitySearcher.h"

// 🌈 COLORS
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// ✨ АНИМАЦИИ ФУНКЦИИ (добавлены ВЕРХУ)
void printSlow(const std::string& text, int delay = 35) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << "\n";
}

void sparkleEffect() {
    std::cout << MAGENTA "✨✨✨ " RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

int main() {
    try {
        // 🎬 АНИМАЦИЯ ЗАПУСКА
        printSlow(BOLD MAGENTA "🌟 AKINATOR v4.0 - LOADING PSYCHIC POWERS... 🌟" RESET, 45);

        std::cout << GREEN "╔════════════════════════════════╗" RESET "\n";
        std::cout << "║          " BOLD YELLOW "AKINATOR" RESET "            ║\n";
        std::cout << "║ " CYAN "the best game in your life" RESET "   ║\n";
        std::cout << "╚════════════════════════════════╝\n\n";

        std::unique_ptr<KnowledgeTree> tree;
        FileHandler fileHandler("saves");
        TreeSerializer serializer;

        // Проверяем существование файла с сохранённым деревом
        if (fileHandler.fileExists("knowledge_tree.json")) {
            printSlow(GREEN "📂 " RESET "Loading knowledge tree from previous sessions...", 38);
            sparkleEffect();

            auto loadedTree = serializer.LoadingFrom("saves/knowledge_tree.json");
            if (loadedTree && loadedTree->getRoot()) {
                tree = std::move(loadedTree);
                auto objects = EntitySearcher::getAllObjects(tree->getRoot());
                printSlow(GREEN "✅ " RESET "Tree loaded! I know about " +
                         std::to_string(objects.size()) + " objects! 🧠", 40);
                sparkleEffect();
            } else {
                printSlow(YELLOW "⚠️ " RESET "Failed to load tree, creating new one...", 35);
                tree = std::make_unique<KnowledgeTree>();
            }
        } else {
            printSlow(YELLOW "🌱 " RESET "No saved tree found, creating new knowledge tree...", 38);
            tree = std::make_unique<KnowledgeTree>();
        }

        auto initialObjects = EntitySearcher::getAllObjects(tree->getRoot());
        printSlow(CYAN "🧠 " RESET "I know about: ", 30);
        for (const auto& obj : initialObjects) {
            std::cout << YELLOW << obj << " " RESET;
        }
        std::cout << "\n\n";

        // ИГРОК
        std::cout << MAGENTA "👤 " BOLD "Enter your name: " RESET;
        std::string playerName;
        std::getline(std::cin, playerName);

        auto player = std::make_shared<Player>(playerName);

        // Пытаемся загрузить статистику игрока
        std::string statsFile = playerName + "_stats.txt";
        try {
            player->loadFromFile(statsFile);
            printSlow(GREEN "📊 " RESET "Statistics loaded! Welcome back, " + playerName + "!", 38);
        } catch (const std::exception& e) {
            printSlow(YELLOW "✨ " RESET "A new player! Welcome, dear " + playerName + "!", 40);
        }

        sparkleEffect();
        player->printStats();

        //ИГРОВОЙ ЦИКЛ
        bool playAgain = true;

        while (playAgain) {
            printSlow(BOLD BLUE "🎮 NEW TELEPATHY SESSION STARTED! 🎮" RESET, 45);

            std::cout << "\n" CYAN << std::string(50, '=') << RESET "\n";
            printSlow(" NEW GAME ", 50);
            std::cout << CYAN << std::string(50, '=') << RESET "\n";
            printSlow(YELLOW "Think of something, and I'll try to guess it! 👁️‍🗨️" RESET, 38);

            // Создаём новую игровую сессию
            GameSession session(*tree, player);

            // Основной игровой цикл - пока не дойдём до ответа
            while (!session.isGameFinished()) {
                try {
                    std::cout << "\n" YELLOW BOLD "🧠 AKINATOR: " RESET;
                    printSlow(session.getCurrentQuestion(), 32);

                    // Получаем ответ от игрока
                    std::cout << GREEN "⚡ Your answer " BOLD "[yes/no/give up]: " RESET;
                    std::string input;
                    std::getline(std::cin, input);

                    if (input == "сдаться" || input == "сдаюсь" || input == "give up") {
                        printSlow(RED BOLD "💥 Surrender detected! Wisdom transfer mode... 💥" RESET, 42);
                        session.surrender();
                        break;
                    }

                    bool answer = (input == "да" || input == "yes" || input == "д" ||
                                  input == "y" || input == "lf" || input == "da");
                    session.processAnswer(answer);

                    // 🤔 АНИМАЦИЯ РАЗМЫШЛЕНИЙ
                    printSlow(CYAN "🔮 Scanning neural patterns... 92% complete" RESET, 35);
                    std::this_thread::sleep_for(std::chrono::milliseconds(700));

                } catch (const GameSession::GameException& e) {
                    std::cout << RED BOLD "❌ Game error: " << e.what() << RESET "\n";
                    break;
                }
            }

            if (session.isGameFinished()) {
                std::cout << "\n" CYAN << std::string(40, '-') << RESET "\n";
                if (session.checkGuess()) {
                    printSlow(BOLD GREEN "🎉 AKINATOR: Hooray! I got it again! 🏆🎉" RESET, 50);
                }
                std::cout << CYAN << std::string(40, '-') << RESET "\n";
            }

            std::cout << "\n";
            sparkleEffect();
            player->printStats();

            auto allObjects = EntitySearcher::getAllObjects(tree->getRoot());
            printSlow(GREEN "🌟 I now know about " RESET + std::to_string(allObjects.size()) + " things! 🚀", 38);

            std::cout << "\n" MAGENTA "🔄 Play again? " BOLD "[yes/no]: " RESET;
            std::string again;
            std::getline(std::cin, again);
            playAgain = (again == "да" || again == "yes" || again == "д" ||
                        again == "y" || again == "lf" || again == "da");
        }

        // СОХРАНЕНИЕ
        printSlow(BOLD GREEN "💾 SAVING TO ETERNITY... 💾" RESET, 48);
        std::cout << "\n" CYAN << std::string(50, '=') << RESET "\n";
        printSlow(" SAVING... ", 55);

        try {
            player->saveToFile(statsFile);
            printSlow(GREEN "✅ Player statistics saved!" RESET, 38);
        } catch (const std::exception& e) {
            printSlow(RED "❌ Couldn't save player statistics: " + std::string(e.what()));
        }

        // Сохраняем дерево знаний
        try {
            printSlow(YELLOW "🌳 Serializing Universal Knowledge Matrix..." RESET, 40);
            std::string jsonTree = serializer.serialize(*tree);
            if (fileHandler.writeFile("knowledge_tree.json", jsonTree)) {
                printSlow(GREEN BOLD "✅ Knowledge tree saved to cosmos! 🌌" RESET, 42);

                auto savedObjects = EntitySearcher::getAllObjects(tree->getRoot());
                printSlow(GREEN "📊 Tree contains " RESET + std::to_string(savedObjects.size()) + " neural nodes", 38);
            } else {
                printSlow(RED "❌ Failed to save knowledge tree!" RESET, 40);
            }
        } catch (const std::exception& e) {
            printSlow(RED "❌ Couldn't save knowledge tree: " + std::string(e.what()) );
        }

        // Финальная статистика
        std::cout << "\n" CYAN << std::string(50, '=') << RESET "\n";
        printSlow(BOLD BLUE " FINAL STATISTICS " RESET, 50);

        player->printStats();

        auto finalObjects = EntitySearcher::getAllObjects(tree->getRoot());
        printSlow(CYAN "🌟 AKINATOR knows: " RESET, 35);
        for (size_t i = 0; i < finalObjects.size(); ++i) {
            if (i > 0) std::cout << YELLOW ", " RESET;
            std::cout << BOLD GREEN << finalObjects[i] << RESET;
        }
        std::cout << "\n";

    } catch (const std::exception& e) {
        std::cerr << RED BOLD "\n❌ CRITICAL ERROR: " << e.what() << RESET "\n";
        return 1;
    }

    std::cout << "\n" CYAN << std::string(50, '=') << RESET "\n";
    printSlow(BOLD MAGENTA " Thanks for playing! Your mind lives forever! 💗🌟" RESET, 55);
    std::cout << std::string(50, '=') << "\n";

    return 0;
}
