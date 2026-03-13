#include <gtest/gtest.h>
#include "Player.h"
#include <fstream>

// ТЕСТ 1 (Создание игрока)
TEST(PlayerTest, CreatePlayer) {
    Player player("Sofia");

    EXPECT_EQ(player.getName(), "Sofia");
    EXPECT_EQ(player.getGamesPlayed(), 0);
    EXPECT_EQ(player.getGamesWon(), 0);
    EXPECT_EQ(player.getGamesLost(), 0);
    EXPECT_EQ(player.getWinRate(), 0.0);
}

// ТЕСТ 2 (Изменение статистики - увел счетчики)
TEST(PlayerTest, UpdateStats) {
    Player player("Test");

    player.incrementGamesPlayed();
    player.won();
    player.lost();

    EXPECT_EQ(player.getGamesPlayed(), 1);
    EXPECT_EQ(player.getGamesWon(), 1);
    EXPECT_EQ(player.getGamesLost(), 1);
}

// ТЕСТ 3 (Подсчет процента побед)
TEST(PlayerTest, WinRateCalculation) {
    Player player("Test");


    for(int i = 0; i < 10; i++) {
        player.incrementGamesPlayed();
    }
    for(int i = 0; i < 7; i++) { // Сыграно 10 игр, выиграно 7
        player.won();
    }


    EXPECT_NEAR(player.getWinRate(), 70.0, 0.001);  //Примерно 70% с погрешностью
}

// ТЕСТ 5 (Защита от деления на ноль)
TEST(PlayerTest, WinRateWhenNoGames) {
    Player player("NoGames");
    EXPECT_EQ(player.getWinRate(), 0.0);
}