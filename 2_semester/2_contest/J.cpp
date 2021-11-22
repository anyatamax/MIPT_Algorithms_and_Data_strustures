// Найдите площадь многоугольника.

// Формат входных данных
// В первой строке одно число N(3 <= N <= 100000). Далее в Nстроках по паре чисел - координаты
// очередной вершины простого многоугольника в порядке обход а по или против часовой стрелки.
// Координаты целые по модулю не превосходят 10000.
// Формат выходных данных
// Одно число - величина площади приведённого многоугольника с абсолютной точностью


#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <cmath>

struct Point {
    long int x = 0;
    long int y = 0;
};

long long Area(const std::vector<Point>& points) {
    long long area = 0;
    for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
        area += (points[i].x * points[j].y - points[i].y * points[j].x);
    }
    return std::abs(area);
}

void PrintArea(long long area) {
    long long res_area = area / 2;
    std::cout << res_area;
    if (area % 2 == 1) {
        std::cout << ".500000000000000";
    } else {
        std::cout << ".000000000000000";
    }
}

int main() {
    std::vector<Point> points;
    int n_points;
    std::cin >> n_points;
    for (size_t i = 0; i < n_points; i++) {
        Point point;
        std::cin >>  point.x >> point.y;
        points.push_back(point);
    }
    long long area = Area(points);
    PrintArea(area);
    return 0;
}
