// Проверьте многоугольник на выпуклость

// Формат ввода
// В первой строке одно число N (3 ≤ N ≤ 100000). Далее в N строках по паре целых чисел — координаты очередной вершины простого многоугольника в порядке обхода по или против часовой стрелки.
// Координаты всех точек целые, по модулю не превосходят 10^7.
// Формат вывода
// Одна строка «YES», если приведённый многоугольник является выпуклым, и «NO» в противном случае. 


#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>
#include <string>

class Point;

class Vector {
    std::vector<long long int> vector_;
public:
    Vector(int first_x, int first_y, int second_x, int second_y) {
        vector_.push_back(second_x - first_x);
        vector_.push_back(second_y - first_y);
    }
    Vector(long long int point_x, long long int point_y) {
        vector_.push_back(point_x);
        vector_.push_back(point_y);
    }
    Vector(const Point& point_x, const Point& point_y);
    Vector(const Vector& other) {
        vector_ = other.vector_;
    }
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            vector_ = other.vector_;
        }
        return *this;
    }
    long long int& operator[](const int ind) {
        if (ind == 0) {
            return vector_[0];
        }
        return vector_[1];
    }
    const long long int& operator[](const int ind) const {
        if (ind == 0) {
            return vector_[0];
        }
        return vector_[1];
    }
    friend Vector operator+(const Vector& a, const Vector& b);
    double Length() {
        return std::sqrt(vector_[0] * vector_[0] + vector_[1] * vector_[1]);
    }
    friend long long int ScalarMultiply(const Vector& first, const Vector& second);
    friend long long int VectorMultiply(const Vector& first, const Vector& second);
};

Vector operator+(const Vector& first, const Vector& second) {
    return Vector(first[0] + second[0], first[1] + second[1]);
}

long long int ScalarMultiply(const Vector& first, const Vector& second) {
    return (first[0] * second[0] + first[1] * second[1]);
}

long long int VectorMultiply(const Vector& first, const Vector& second) {
    return (first[0] * second[1] - first[1] * second[0]);
}


class IShape{
public:
    virtual IShape& Move(const Vector& vector) = 0;
    virtual bool ContainsPoint(const Point& point) const = 0;
    virtual std::string ToString() = 0;
};

class Point: public IShape {
    long long int point_x_;
    long long int point_y_;
public:
    Point(long long int x, long long int y) : point_x_(x), point_y_(y) {
    }
    Point& operator=(const Point& other) {
        if (this != &other) {
            point_x_ = other.point_x_;
            point_x_ = other.point_y_;
        }
        return *this;
    }
    friend bool operator==(const Point& a, const Point& b);
    friend Vector operator-(const Point& b, const Point& a);
    bool ContainsPoint(const Point& point) const override {
        return (*this == point);
    }
    Point& Move(const Vector& vector) override {
        point_x_ += vector[0];
        point_y_ += vector[1];
        return *this;
    }
    std::string ToString() override {
        std::ostringstream ostr;
        ostr << "Point(" << point_x_ << ", " << point_y_ << ")";
        std::string str = ostr.str();
        return str;
    }
    long long int GetX() const {
        return point_x_;
    }
    long long int GetY() const {
        return point_y_;
    }
};

bool operator==(const Point& a, const Point& b) {
    return ((a.point_x_ == b.point_x_) && (a.point_y_ == b.point_y_));
}

Vector operator-(const Point& b, const Point& a) {
    return Vector(a, b);
}

Vector::Vector(const Point& point_x, const Point& point_y) {
    vector_.push_back(point_y.GetX() - point_x.GetX());
    vector_.push_back(point_y.GetY() - point_x.GetY());
}

int main() {
    long long int x, y;
    size_t n_points = 0;
    std::cin >> n_points;
    std::vector<Point> points;
    points.reserve(n_points);
    int digit = 1;
    bool bulging = true;
    for (size_t i = 0; i < n_points; ++i) {
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }
    long long int multiplication = VectorMultiply(Vector(points[n_points - 1], points[0]), Vector(points[0], points[1]));
    if (multiplication == 0) {
        for (size_t i = 0; i < n_points - 2 && multiplication == 0; i++) {
            multiplication = VectorMultiply(Vector(points[i], points[i + 1]), Vector(points[i + 1], points[i + 2]));
        }
    }
    if (multiplication > 0) {
        digit = 1;
    } else {
        digit = -1;
    }
    for (size_t i = 0; i < n_points - 2 && bulging; i++) {
        Vector side_first(points[i], points[i + 1]);
        Vector side_second(points[i + 1], points[i + 2]);
        multiplication = VectorMultiply(side_first, side_second);
        if (digit * multiplication < 0) {
            bulging = false;
        }
    }
    if (bulging) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
    return 0;
}
