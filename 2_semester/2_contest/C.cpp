

// Даны коэффициенты A1, B1, C1 уравнения первой прямой и коэффициенты A2, B2, C2 уравнения второй прямой. Требуется:
//     Построить направляющие векторы для обеих прямых (вывести координаты)
//     Найти точку пересечения двух прямых или вычислить расстояние между ними, если они параллельны

// Формат ввода
// В первой строке входного файла находятся три числа — коэффициенты нормального уравнения для первой прямой. Во второй строке — коэффициенты для второй прямой.
// Все числа во входном файле по модулю не превосодят 10 000.
// Формат вывода
// В каждой строке выходного файла выведите ответ на соответствующий пункт задачи с точностью до 10^(-6). 

#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <cmath>

int main() {
    std::vector<double> first;
    std::vector<double> second;
    double coef_first_A, coef_first_B, coef_first_C, coef_second_A, coef_second_B, coef_second_C;
    std::cin >> coef_first_A >> coef_first_B >> coef_first_C >> coef_second_A >> coef_second_B >> coef_second_C;
    first.push_back(coef_first_B);
    first.push_back(-coef_first_A);
    second.push_back(coef_second_B);
    second.push_back(-coef_second_A);
    std::cout << std::fixed << std::setprecision(6) << first[0] << " " << first[1] << std::endl;
    std::cout << std::fixed << std::setprecision(6) << second[0] << " " << second[1] << std::endl;
    if (coef_first_A * coef_second_B - coef_second_A * coef_first_B == 0) {
        double distance, coef;
        if (coef_first_A != 0) {
            if (coef_first_A > coef_second_A) {
                coef = coef_first_A / coef_second_A;
                coef_first_C /= coef;
            } else {
                coef = coef_second_A / coef_first_A;
                coef_second_C /= coef;
            }
        } else {
            if (coef_first_B > coef_second_B) {
                coef = coef_first_B / coef_second_B;
                coef_first_C /= coef;
            } else {
                coef = coef_second_B / coef_first_B;
                coef_second_C /= coef;
            }
        }
        double A = std::min(coef_second_A, coef_first_A);
        double B = std::min(coef_first_B, coef_second_B);
        distance = (coef_first_C - coef_second_C) / std::sqrt(A * A + B * B);
        if (distance < 0) {
            std::cout << std::fixed << std::setprecision(6) << (-distance);

        } else {
            std::cout << std::fixed << std::setprecision(6) << distance;
        }
    } else {
        double x_0, y_0;
        x_0 = static_cast<double>((-coef_first_C) * coef_second_B + coef_second_C * coef_first_B)
              / (coef_first_A * coef_second_B - coef_second_A * coef_first_B);
        y_0 = static_cast<double>((-coef_first_A) * coef_second_C + coef_second_A * coef_first_C)
              / (coef_first_A * coef_second_B - coef_second_A * coef_first_B);
        std::cout << std::fixed << std::setprecision(6) << x_0 << " " << y_0;
    }
    return 0;
}
