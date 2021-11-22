// Определите, принадлежит ли точка C заданной прямой, лучу и отрезку, образованными точками A и B.

// Формат ввода
// В первой строке входного файла даны два целых числа — координаты точки C. Во двух следующих строках в таком же формате заданы точки A и B (A ≠ B).
// Все числа во входном файле по модулю не превосодят 10 000.
// Формат вывода
// В первой строке выведите «YES», если точка C принадлежит прямой AB, и «NO» в противном случае. Во второй и третьей строках аналогично выведите ответы для луча AB (A — начало луча) и отрезка AB. 

#include <iostream>
#include <vector>

struct Point {
    int x = 0;
    int y = 0;
};

bool InLine(Point& point, Point& first, Point& second, std::vector<int>& vector_line) {
    return ((point.x - first.x) * vector_line[1] == (point.y - first.y) * vector_line[0]);
}

bool InRay(Point& point, Point& point_ray, std::vector<int>& vector_ray) {
    std::vector<int> ray_to_point;
    ray_to_point.push_back(point.x - point_ray.x);
    ray_to_point.push_back(point.y - point_ray.y);
    return ((ray_to_point[0] * vector_ray[1] - ray_to_point[1] * vector_ray[0] == 0)
            && (vector_ray[0] * ray_to_point[0] + vector_ray[1] * ray_to_point[1] >= 0));
}

bool InSegment(Point& point, Point& first, Point& second, std::vector<int>& vector_sgmt) {
    std::vector<int> first_to_point;
    std::vector<int> second_to_point;
    first_to_point.push_back(point.x - first.x);
    first_to_point.push_back(point.y - first.y);
    second_to_point.push_back(point.x - second.x);
    second_to_point.push_back(point.y - second.y);
    return ((first_to_point[0] * second_to_point[1] - first_to_point[1] * second_to_point[0] == 0)
            && (first_to_point[0] * second_to_point[0] + first_to_point[1] * second_to_point[1] <= 0));
}

int main() {
    Point point, first, second;
    std::cin >> point.x >> point.y >> first.x >> first.y >> second.x >> second.y;
    std::vector<int> vector;
    vector.push_back(second.x - first.x);
    vector.push_back(second.y - first.y);
    if (InLine(point, first, second, vector)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
    if (InRay(point, first, vector)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
    if (InSegment(point, first, second, vector)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
    return 0;
}
