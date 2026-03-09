//
// Created by София on 09.03.2026.
//

#include "Player.h"

#include <iomanip> //для вывода вывода (std::fixed, std::setprecision)
#include <ctime>  //для времени (std::time_t, std::ctime)

// ------------------------------------------------------------- КОНСТРУКТОР  ----------------------------------------------------------------------------------------------------

//так как я пользовалась современной инициализацией - мне остается только инициализировать имя игрока и новое время входа,
//тк остальные уже инициализированы в хедер файле нулями

Player::Player(std::string_view name): name_(name), last_played_(std::chrono::system_clock::now()){};

// ------------------------------------------------------------- СТАТИСТИКА  ----------------------------------------------------------------------------------------------------

//колво игр и последняя игра
void Player::incrementGamesPlayed() {
    games_played_++; //колличество сыграных +1
    last_played_ = std::chrono::system_clock::now(); //system_clock::now() - возвращает время в моменте
}

void Player::won() {        //акинатр угадал
    games_won_++;
}

void Player::lost() {       // акинатор не угадал
    games_lost_++;
}

double Player::getWinRate() const {
    if (games_played_ == 0) {        //защита от деления на 0: если игр не было - возвращаем 0
        return 0.0;
    }
    return (static_cast<double>(games_won_)/games_played_)*WIN_RATE_PRECISION; // Формула: (победы / всего игр) * 100
    // static_cast<double> нужно для преобразования int в double,
    // чтобы деление было вещественным, а не целочисленным
    // WIN_RATE_PRECISION = 100.0 - константа для перевода в проценты
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

    std::time_t time = std::chrono::system_clock::to_time_t(last_played_);
    //system_clock::to_time_t конвертирует C++ chrono время в C-структуру time_t
    // ctime преобразует time_t в строку вида "Wed Mar 9 12:34:56 2026"
    // Внимание: ctime добавляет перевод строки в конце!

    std::cout << "Время последней игры: " << std::ctime(&time); //на всякий случай используем более старую функцию,
                                                                //чтобы на всех устройствах поддерживаллось
}

// ------------------------------------------------------------- ФАЙЛЫ  ----------------------------------------------------------------------------------------------------

void Player::saveToFile(const std::string& filename) const {

    std::ofstream file(filename); //открываем поток вывода в файл для записи данных

    if (!file.is_open()) {
        throw std::runtime_error("анлаки, проблемы с файлом для сохранения: " + filename);   //исключение типа std::runtime_error
    }

    //записываем данные в файл (построчно)
    file << name_<< "\n";
    file << games_played_<< "\n";
    file << games_won_<< "\n";
    file << games_lost_<< "\n";



    file << last_played_.time_since_epoch().count() << "\n"; // time_since_epoch() - время, прошедшее с 1 января 1970 года
                                                            // count() - считаем количество "тиков" (единиц времени)


    // Файл автоматически закроется здесь, когда file выйдет из области видимости
    // Это называется RAII (Resource Acquisition Is Initialization)
    // Ресурс (файл) освобождается в деструкторе автоматически
}


//Загружаем статистику из файла если пользователь уже играл и зарегистрирован
void Player::loadFromFile(const std::string& filename) {
    std::ifstream file(filename); //открываем поток ввода из файла

    if (!file.is_open()) {
        throw std::runtime_error("анлаки, похоже у вас еще нет аккаунта" + filename);
    }

    //если ве хорошо и файл открылся - считываем данные

    std::getline(file, name_);
    file >> games_played_;
    file >> games_won_;
    file >> games_lost_;

    long long last_played_ticks;  //гигантская переменная для колличества тиков
    file >> last_played_ticks;


    // Восстанавливаем время из тиков
    // duration(ticks) - создает длительность из количества тиков
    // time_point(duration) - создает момент времени из длительности
    last_played_ = std::chrono::system_clock::time_point(
        std::chrono::system_clock::duration(last_played_ticks)
    );


    // Файл автоматически закроется здесь (RAII)

}




