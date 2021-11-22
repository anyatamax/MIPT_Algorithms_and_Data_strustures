// Уже долгое время в Институте Мутантов, Фич Технологических и Иностранных языков разводят милых разноцветных зверюшек. Для удобства каждый цвет обозначен своим номером, всего цветов не более 109.
// В один из прекрасных дней в питомнике случилось чудо: все зверюшки выстроились в ряд в порядке возрастания цветов. Пользуясь случаем, лаборанты решили посчитать, сколько зверюшек разных цветов живет в питомнике, и, по закону жанра, попросили вас написать программу, которая поможет им в решении этой нелегкой задачи.
// Используйте для решения этой задачи готовые функции STL для std::vector и библиотеку algorithm.

// Формат ввода
// В первой строке входного файла содержится единственное число N (0 ≤ N ≤ 10^5) — количество зверюшек в Институте.
// В следующей строке находятся N упорядоченных по неубыванию неотрицательных целых чисел, не превосходящих 10^9 и разделенных пробелами — их цвета.
// В третьей строке файла записано число M (1 ≤ M ≤ 100000) — количество запросов вашей программе, в следующей строке через пробел записаны M целых неотрицательных чисел (не превышающих 10^9+1).
// Формат вывода
// Выходной файл должен содержать M строчек. Для каждого запроса выведите число зверюшек заданного цвета в питомнике. 

#include <iostream>
#include <vector>
#include <algorithm>

int main () {
    std::vector<long int> colors_of_animals;
    int num_of_animals;
    std::cin >> num_of_animals;
    for (size_t i = 0; i < num_of_animals; ++i) {
        long int color;
        std::cin >> color;
        colors_of_animals.push_back(color);
    }
    int num_of_requests;
    std::cin >> num_of_requests;
    for (size_t i = 0; i < num_of_requests; ++i) {
        long int request;
        std::cin >> request;
        if (colors_of_animals.empty() || request > colors_of_animals[colors_of_animals.size() - 1] || request < colors_of_animals[0]) {
            std::cout << 0 << "\n";
        } else {
            auto beg = std::lower_bound(colors_of_animals.begin(), colors_of_animals.end(), request);
            auto end = std::upper_bound(colors_of_animals.begin(), colors_of_animals.end(), request);
            std::cout << std::distance(beg, end) << "\n";
        }
    }
    return 0;
}
