// Даны два ненулевых вектора. Требуется вычислить:
//     Длину первого и второго вектора (два числа)
//     Вектор, образованный сложением данных двух векторов
//     Скалярное и векторное произведения данных векторов
//     Площадь треугольника, построенного из этих векторов

// Формат ввода
// В двух строках входного файла заданы по четыре целых числа, не превосходящих по модулю 10 000, — координаты начала и конца первого вектора, затем второго.
// Формат вывода
// В каждой строке выходного файла — ответ на соответствующий пункт задачи с точностью не менее 10^(-6).

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

int main() {
    std::vector<int> first;
    std::vector<int> second;
    std::vector<double> sum_v;
    int point1_x, point1_y, point2_x, point2_y;
    double length_first, length_second, scalar_multiply, vector_multiply, square;
    std::cin >> point1_x >> point1_y >> point2_x >> point2_y;
    first.push_back(point2_x - point1_x);
    first.push_back(point2_y - point1_y);
    std::cin >> point1_x >> point1_y >> point2_x >> point2_y;
    second.push_back(point2_x - point1_x);
    second.push_back(point2_y - point1_y);
    length_first = std::sqrt(first[0] * first[0] + first[1] * first[1]);
    length_second = std::sqrt(second[0] * second[0] + second[1] * second[1]);
    sum_v.push_back(first[0] + second[0]);
    sum_v.push_back(first[1] + second[1]);
    scalar_multiply = static_cast<double>(first[0] * second[0] + first[1] * second[1]);
    vector_multiply = static_cast<double>(first[0] * second[1] - first[1] * second[0]);
    if (vector_multiply < 0) {
        square = (-vector_multiply) / 2;
    } else {
        square = vector_multiply / 2;
    }
    std::cout << std::fixed << std::setprecision(6) << length_first << " ";
    std::cout << std::fixed << std::setprecision(6) << length_second << std::endl;
    std::cout << std::fixed << std::setprecision(6) << sum_v[0] << " " << sum_v[1] << std::endl;
    std::cout << std::fixed << std::setprecision(6) << scalar_multiply << " " << vector_multiply << std::endl;
    std::cout << std::fixed << std::setprecision(6) << square;
    return 0;
}
