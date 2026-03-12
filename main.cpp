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
        std::cout << "║         АКИНАТОР v1.0          ║\n";
        std::cout << "║     Отгадай, что я загадал!    ║\n";
        std::cout << "╚════════════════════════════════╝\n\n";

        // ========== ИНИЦИАЛИЗАЦИЯ ==========
        // Создаём дерево знаний
        auto tree = std::make_unique<KnowledgeTree>();

        // Создаём или загружаем игрока
        std::cout << "Введите ваше имя: ";
        std::string playerName;
        std::getline(std::cin, playerName);

        auto player = std::make_shared<Player>(playerName);

        // Пытаемся загрузить статистику игрока
        std::string statsFile = playerName + "_stats.txt";
        try {
            player->loadFromFile(statsFile);
            std::cout << "Статистика загружена! С возвращением, " << playerName << "!\n";
        } catch (const std::exception& e) {
            std::cout << "Новый игрок! Добро пожаловать, " << playerName << "!\n";
        }

        // ========== ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ==========
        bool playAgain = true;

        while (playAgain) {
            // Создаём игровую сессию
            GameSession session(*tree, player);

            std::cout << "\n--- НОВАЯ ИГРА ---\n";

            // Игровой цикл - пока не дойдём до ответа
            while (!session.isGameFinished()) {
                try {
                    // Получаем и показываем вопрос/предположение
                    std::cout << "\nАкинатор: " << session.getCurrentQuestion() << "\n";

                    // Получаем ответ от игрока
                    std::cout << "Вы (да/нет/сдаться): ";
                    std::string input;
                    std::getline(std::cin, input);

                    // Проверка на сдачу
                    if (input == "сдаться" || input == "сдаюсь" || input == "сдать") {
                        session.surrender();
                        break;
                    }

                    // Обработка ответа
                    bool answer = (input == "да" || input == "yes" || input == "д" || input == "y");
                    session.processAnswer(answer);

                } catch (const GameSession::GameException& e) {
                    std::cout << "Ошибка в игре: " << e.what() << "\n";
                    break;
                }
            }

            // Проверяем результат игры (если не сдались)
            if (session.isGameFinished() && !session.getCurrentQuestion().empty()) {
                std::cout << "\n";
                if (session.checkGuess()) {
                    std::cout << "🎉 Акинатор: Ура! Я снова угадал!\n";
                } else {
                    std::cout << "🤔 Акинатор: Теперь я стал умнее!\n";
                }
            }

            // Показываем статистику
            std::cout << "\n";
            player->printStats();

            // Спрашиваем, хочет ли игрок продолжать
            std::cout << "\nСыграем ещё? (да/нет): ";
            std::string again;
            std::getline(std::cin, again);
            playAgain = (again == "да" || again == "yes" || again == "д" || again == "y");
        }

        // ========== СОХРАНЕНИЕ ПЕРЕД ВЫХОДОМ ==========
        try {
            player->saveToFile(statsFile);
            std::cout << "Статистика сохранена. До свидания, " << playerName << "!\n";
        } catch (const std::exception& e) {
            std::cout << "Не удалось сохранить статистику: " << e.what() << "\n";
        }

        // ========== СОХРАНЕНИЕ ДЕРЕВА ==========
        try {
            TreeSerializer serializer;
            std::string jsonTree = serializer.serialize(*tree);

            FileHandler fileHandler("saves");
            fileHandler.writeFile("knowledge_tree.json", jsonTree);
            std::cout << "Дерево знаний сохранено.\n";
        } catch (const std::exception& e) {
            std::cout << "Не удалось сохранить дерево: " << e.what() << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "\n❌ КРИТИЧЕСКАЯ ОШИБКА: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nДо новых встреч! 👋\n";
    return 0;
}