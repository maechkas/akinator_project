//
// Created by София on 09.03.2026.
//

#ifndef AKINATOR_PROJECT_PLAYER_H
#define AKINATOR_PROJECT_PLAYER_H

#include <string>
#include <iostream>
#include <fstream>
#include <string_view>

class Player {
private:
    std::string name_;
    int games_played_{0};
    int games_won_{0};
    int games_lost_{0};

    constexpr static double WIN_RATE_PRECISION = 100.0; //константа - рейтинг, 100 чтобы было удобно перевести в проценты

public:
    explicit Player(std::string_view name);

    //геттеры
    std::string_view getName() const {return name_;}
    int getGamesPlayed() const {return games_played_;}
    int getGamesWon() const {return games_won_;}
    int getGamesLost() const {return games_lost_;}

    void incrementGamesPlayed();
    void won();
    void lost();

    double getWinRate() const;
    void printStats() const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif //AKINATOR_PROJECT_PLAYER_H