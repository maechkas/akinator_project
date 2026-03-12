//
// Created by София on 09.03.2026.
//

#include "Player.h"

#include <iomanip>

// ------------------------------------------------------------- КОНСТРУКТОР  ----------------------------------------------------------------------------------------------------
Player::Player(std::string_view name): name_(name) {};


// ------------------------------------------------------------- СТАТИСТИКА  ----------------------------------------------------------------------------------------------------
//колво игр и последняя игра
void Player::incrementGamesPlayed() {
    games_played_++;
}

void Player::won() {
    games_won_++;
}

void Player::lost() {
    games_lost_++;
}

double Player::getWinRate() const {
    if (games_played_ == 0) {        //защита от деления на 0: если игр не было - возвращаем 0
        return 0.0;
    }
    return (static_cast<double>(games_won_)/games_played_)*WIN_RATE_PRECISION; // Формула: (победы / всего игр) * 100
}

void Player::printStats() const {

    std::cout << "\n___Ваша статистика___\n";
    std::cout << "Имя: " << name_ << "\n";
    std::cout << "Всего игр: " << games_played_ << "\n";
    std::cout << "Акинатор угадал: " << games_won_ << "\n";
    std::cout << "Акинатор не угадал: " << games_lost_ << "\n";

    std::cout << "Процент побед: " << std::fixed << std::setprecision(1) << getWinRate() << "%\n";
    //процент побед с точностью 1 знак после запятой
    //std::fixed чтобы вывод был в норм формате без всяких 1e+2
}

// ------------------------------------------------------------- ФАЙЛЫ  ----------------------------------------------------------------------------------------------------

void Player::saveToFile(const std::string& filename) const {

    std::ofstream file(filename); //открываем поток вывода в файл для записи данных

    if (!file.is_open()) {
        throw std::runtime_error("анлаки, проблемы с файлом для сохранения: " + filename);
    }

    file << name_<< "\n";
    file << games_played_<< "\n";
    file << games_won_<< "\n";
    file << games_lost_<< "\n";
}


//Загружаем статистику из файла если пользователь уже играл и зарегистрирован
void Player::loadFromFile(const std::string& filename) {
    std::ifstream file(filename); //открываем поток ввода из файла

    if (!file.is_open()) {
        throw std::runtime_error("анлаки, похоже у вас еще нет аккаунта" + filename);
    }

    //если все хорошо и файл открылся - считываем данные

    std::getline(file, name_);
    file >> games_played_;
    file >> games_won_;
    file >> games_lost_;

    // Файл автоматически закроется здесь (RAII)
}