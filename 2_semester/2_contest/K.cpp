// Дано N точек на плоскости. Нужно построить их выпуклую оболочку.
// Гарантируется, что выпуклая оболочка не вырождена.

// Формат ввода
// На первой строке число N (3 ≤ N ≤ 10^5). Следующие N строк содержат пары целых чисел x и y (-10^9 ≤ x, y ≤ 10^9) — точки.
// Будьте аккуратны! Точки произвольны. Бывают совпадающие, бывают лежащие на одной прямой в большом количестве.
// Формат вывода
// В первой строке выведите K — число вершин выпуклой оболочки. Следующие K строк должны содержать координаты вершин в порядке обхода. Никакие три подряд идущие точки не должны лежать на одной прямой.
// Кроме того, в последней строке выведите площадь получившейся выпуклой оболочки. Площадь необходимо вывести абсолютно точно.


#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <algorithm>

struct Point {
    long int x = 0;
    long int y = 0;
};

bool LeftTurn(Point first, Point centre, Point last) {
    return first.x * (centre.y - last.y) + centre.x * (last.y - first.y) + last.x * (first.y - centre.y) < 0;
}

bool RightTurn(Point first, Point centre, Point last) {
    return first.x * (centre.y - last.y) + centre.x * (last.y - first.y) + last.x * (first.y - centre.y) > 0;
}

bool cmp(Point& lhs, Point& rhs) {
    return (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y));
}

std::vector<Point> ConvexHall(std::vector<Point>& points) {
    sort(points.begin(), points.end(), cmp);
    Point point_first = points[0];
    Point point_last = points.back();
    std::vector<Point> up, down;
    up.push_back(point_first);
    down.push_back(point_first);
    for (int i = 1; i < points.size(); ++i) {
        if (i == points.size() - 1 || LeftTurn(point_first, points[i], point_last)) {
            while(up.size() >= 2 && !LeftTurn(up[up.size() - 2], up[up.size() - 1], points[i])) {
                up.pop_back();
            }
            up.push_back(points[i]);
        }
        if (i == points.size() - 1 || RightTurn(point_first, points[i], point_last)) {
            while(down.size() >= 2 && !RightTurn(down[down.size() - 2], down[down.size() - 1], points[i])) {
                down.pop_back();
            }
            down.push_back(points[i]);
        }
    }
    std::vector<Point> res;
    for (int i = 0; i < up.size(); ++i) {
        res.push_back(up[i]);
    }
    for (size_t i = down.size() - 2; i > 0; --i) {
        res.push_back(down[i]);
    }
    return res;
}

long long Area(const std::vector<Point>& points) {
    long long area = 0;
    for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
        area += (points[i].x * points[j].y - points[i].y * points[j].x);
    }
    area = std::abs(area);
    return area;
}

void PrintArea(long long area) {
    long long res_area = area / 2;
    std::cout << res_area;
    if (area % 2 == 1) {
        std::cout << ".5";
    } else {
        std::cout << ".0";
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
    std::vector<Point> res_points = ConvexHall(points);
    std::cout << res_points.size() << std::endl;
    for (size_t i = 0; i < res_points.size(); i++) {
        std::cout << res_points[i].x << " " << res_points[i].y << std::endl;
    }
    long long area = Area(res_points);
    PrintArea(area);
    return 0;
}
