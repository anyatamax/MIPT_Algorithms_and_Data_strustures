// Юный программист Саша написал свою первую тестирующую систему. Он так обрадовался тому, что она скомпилировалась, что решил пригласить школьных друзей на свой собственный контест.
// Но в конце тура выяснилось, что система не умеет сортировать команды в таблице результатов. Помогите Саше реализовать эту сортировку.
// Команды упорядочиваются по правилам ACM:
//     по количеству решённых задач в порядке убывания;
//     при равенстве количества решённых задач – по штрафному времени в порядке возрастания;
//     при прочих равных — по номеру команды в порядке возрастания.
// Используйте std::vector в этой задаче и собственный компаратор.

// Формат ввода
// Первая строка содержит натуральное число n (1 ≤ n ≤ 10^5) — количество команд, участвующих в контесте.
// В i-й из следующих n строк записано количество решенных задач S (0 ≤ S ≤ 100) и штрафное время T (0 ≤ T ≤ 10^5) команды с номером i.
// Формат вывода
// В выходной файл выведите n чисел — номера команд в отсортированном порядке. 


#include <iostream>
#include <vector>
#include <algorithm>

struct Command {
    size_t number = 0;
    int count_tasks = 0;
    int time = 0;
};

bool comp (Command& lhs, Command& rhs) {
    if (lhs.count_tasks != rhs.count_tasks) {
        return lhs.count_tasks > rhs.count_tasks;
    }
    if (lhs.time != rhs.time) {
        return lhs.time < rhs.time;
    }
    return lhs.number < rhs.number;
}

int main() {
    std::vector<Command> players;
    int number_players;
    std::cin >> number_players;
    for (size_t i = 0; i < number_players; i++) {
        Command new_command;
        std::cin >> new_command.count_tasks >> new_command.time;
        new_command.number = i + 1;
        players.push_back(new_command);
    }
    std::sort(players.begin(), players.end(), comp);
    for (size_t i = 0; i < number_players; i++) {
        std::cout << players[i].number << "\n";
    }
    return 0;
}
