// Даны три точки A, B и C. Необходимо подсчитать расстояния от точки C до прямой, луча и отрезка, образованного точками A и B.

// Формат ввода
// В первой строке входного файла даны два целых числа — координаты точки C. Во двух следующих строках в таком же формате заданы точки A и B (A ≠ B).
// Все числа во входном файле по модулю не превосодят 10 000.
// Формат вывода
// В первой строке выходного файла выведите одно вещественное число — расстояние от точки C до прямой. В следующих двух строках выведите соответственно расстояния до луча AB (A — начало луча) и до отрезка AB. Все числа выводить с точностью не менее 10^(-6). Луч строится по направлению от точки A к точке B.


#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Point {
    int x = 0;
    int y = 0;
};

double ToLine(Point& point, Point& first, Point& second, std::vector<int>& vector) {
    double result = static_cast<double>(vector[1] * point.x - vector[0] * point.y + first.y
                                                                                    * vector[0] - first.x * vector[1])
                    / std::sqrt(vector[1] * vector[1] + vector[0] * vector[0]);
    if (result < 0) {
        return (-result);
    }
    return result;
}

double ToRay(Point& point, Point& first, Point& second, std::vector<int>& vector) {
    std::vector<int> first_to_second;
    first_to_second.push_back(point.x - first.x);
    first_to_second.push_back(point.y - first.y);
    if (first_to_second[0] * vector[0] + first_to_second[1] * vector[1] >= 0) {
        return ToLine(point, first, second, vector);
    }
    return std::sqrt(first_to_second[0] * first_to_second[0] + first_to_second[1] * first_to_second[1]);
}

double ToSegment(Point& point, Point& first, Point& second, std::vector<int>& vector, std::vector<int>& vector_reverse) {
    std::vector<int> first_to_point;
    std::vector<int> second_to_point;
    first_to_point.push_back(point.x - first.x);
    first_to_point.push_back(point.y - first.y);
    second_to_point.push_back(point.x - second.x);
    second_to_point.push_back(point.y - second.y);
    if (first_to_point[0] * vector[0] + first_to_point[1] * vector[1] < 0) {
        return std::sqrt(first_to_point[0] * first_to_point[0] + first_to_point[1] * first_to_point[1]);
    }
    if (second_to_point[0] * vector_reverse[0] + second_to_point[1] * vector_reverse[1] < 0) {
        return std::sqrt(second_to_point[0] * second_to_point[0] + second_to_point[1] * second_to_point[1]);
    }
    return ToLine(point, first, second, vector);
}

int main() {
    Point point, first, second;
    std::cin >> point.x >> point.y >> first.x >> first.y >> second.x >> second.y;
    std::vector<int> vector;
    std::vector<int> vector_reverse;
    vector.push_back(second.x - first.x);
    vector.push_back(second.y - first.y);
    vector_reverse.push_back(first.x - second.x);
    vector_reverse.push_back(first.y - second.y);
    std::cout << std::fixed << std::setprecision(9) << ToLine(point, first, second, vector) << std::endl;
    std::cout << std::fixed << std::setprecision(9) << ToRay(point, first, second, vector) << std::endl;
    std::cout << std::fixed << std::setprecision(9) << ToSegment(point, first, second, vector, vector_reverse) << std::endl;
    return 0;
}
