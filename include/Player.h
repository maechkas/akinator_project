//
// Created by София on 09.03.2026.
//

#ifndef AKINATOR_PROJECT_PLAYER_H //если такое имя не определено - определить
#define AKINATOR_PROJECT_PLAYER_H// определяем

#include <string>
#include <chrono> //для работы со временем
#include <iostream>
#include <fstream> //для работы с файлами
#include <string_view> //для просмотра строк без копирования

class Player {
private:
    std::string name_;
    int games_played_{0}; //сколько игр сыграно
    int games_won_{0};   // сколько раз компьютер угадал
    int games_lost_{0};  // сколько раз компьютер не угадал
    std::chrono::system_clock::time_point last_played_{}; //system_clock - системные часы
                                                        //time_point - момент времени последней игры

    constexpr static double WIN_RATE_PRECISION = 100.0; //рейтинг, 100 чтобы было удобно перевести в проценты

public:
    explicit Player(std::string_view name); //explicit запрещает неявное преобразование типов(нпример чтобы случайно не создать игрока из числа)

    //геттеры - методы для всякой работы со статистикой
    std::string_view getName() const {return name_;}
    int getGamesPlayed() const {return games_played_;}
    int getGamesWon() const {return games_won_;}            //конст методы не меняющие данные
    int getGamesLost() const {return games_lost_;}

    void incrementGamesPlayed(); //увеличить счетчит игр
    void won();                  // записать победу
    void lost();                //записать поражение

    double getWinRate() const; //процент побед
    void printStats() const; //выводит статистику в консоль

    void saveToFile(const std::string& filename) const; //сохраняем в файл все данные игрока и статистику, конст - никакие данные игрока не меняет
    void loadFromFile(const std::string& filename);     //берет из файла уже существующую статистику игрока, не конст - заполняет поля игрока данными из файла


};

#endif //AKINATOR_PROJECT_PLAYER_H