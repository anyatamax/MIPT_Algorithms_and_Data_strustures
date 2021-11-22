// Необходимо проверить, пересекаются ли два отрезка.

// Формат ввода
// В двух строках входного файла заданы по четыре целых числа, не превосходящих по модулю 10 000, — координаты концов первого отрезка, затем второго.
// Формат вывода
// В первой строке выходного файла выведите «YES», если отрезки имеют общие точки, и «NO» в противном случае. 

#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>
#include <string>

class Vector;

class IShape;
class Point;
class Segment;
class Ray;
class Line;
class Circle;
class Polygon;

class Vector {
    std::vector<int> vector_;
public:
    Vector(int first_x, int first_y, int second_x, int second_y) {
        vector_.push_back(second_x - first_x);
        vector_.push_back(second_y - first_y);
    }
    Vector(int point_x, int point_y) {
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
    int& operator[](const int ind) {
        if (ind == 0) {
            return vector_[0];
        }
        return vector_[1];
    }
    const int& operator[](const int ind) const {
        if (ind == 0) {
            return vector_[0];
        }
        return vector_[1];
    }
    friend Vector operator+(const Vector& lhs, const Vector& rhs);
    double Length() {
        return std::sqrt(vector_[0] * vector_[0] + vector_[1] * vector_[1]);
    }
    friend int ScalarMultiply(const Vector& first, const Vector& second);
    friend int VectorMultiply(const Vector& first, const Vector& second);
};

Vector operator+(const Vector& lhs, const Vector& rhs) {
    return Vector(lhs[0] + rhs[0], rhs[1] + lhs[1]);
}

int ScalarMultiply(const Vector& first, const Vector& second) {
    return (first[0] * second[0] + first[1] * second[1]);
}

int VectorMultiply(const Vector& first, const Vector& second) {
    return (first[0] * second[1] - first[1] * second[0]);
}


class IShape{
public:
    virtual IShape& Move(const Vector& vector) = 0;
    virtual bool ContainsPoint(const Point& point) const = 0;
    virtual bool CrossesSegment(const Segment& segment) const = 0;
    virtual std::unique_ptr<IShape> Clone() const = 0;
    virtual std::string ToString() = 0;
};

class Point: public IShape {
    int point_x_;
    int point_y_;
public:
    Point(int x, int y) : point_x_(x), point_y_(y) {
    }
    Point& operator=(const Point& other) {
        if (this != &other) {
            point_x_ = other.point_x_;
            point_y_ = other.point_y_;
        }
        return *this;
    }
    friend bool operator==(const Point& a, const Point& b);
    friend bool operator<(const Point& a, const Point& b);
    friend bool operator>(const Point& a, const Point& b);
    friend Vector operator-(const Point& lhs, const Point& rhs);
    bool ContainsPoint(const Point& point) const override {
        return (*this == point);
    }
    bool CrossesSegment(const Segment& segment) const override;
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
    std::unique_ptr<IShape> Clone() const override {
        auto point = new Point(point_x_, point_y_);
        std::unique_ptr<IShape> clone_ptr(point);
        return clone_ptr;
    }
    int GetX() const {
        return point_x_;
    }
    int GetY() const {
        return point_y_;
    }
};

bool operator==(const Point& a, const Point& b) {
    return ((a.point_x_ == b.point_x_) && (a.point_y_ == b.point_y_));
}

bool operator<(const Point& a, const Point& b) {
    if (a.point_x_ == b.point_x_) {
        return (a.point_y_ < b.point_y_);
    }
    return ((a.point_x_ < b.point_x_));
}

bool operator>(const Point& a, const Point& b) {
    if (a.point_x_ == b.point_x_) {
        return (a.point_y_ > b.point_y_);
    }
    return ((a.point_x_ > b.point_x_));
}

Vector operator-(const Point& rhs, const Point& lhs) {
    return Vector(lhs, rhs);
}

Vector::Vector(const Point& point_x, const Point& point_y) {
    vector_.push_back(point_y.GetX() - point_x.GetX());
    vector_.push_back(point_y.GetY() - point_x.GetY());
}

class Segment: public IShape {
public:
    Point first_, second_;
    Segment(const Point& first, const Point& second) : first_(first), second_(second) {
    }
    bool ContainsPoint(const Point& point) const override {
        Vector point_to_first(first_, point);
        Vector point_to_second(second_, point);
        return (VectorMultiply(point_to_first, point_to_second) == 0
                && ScalarMultiply(point_to_first, point_to_second) <= 0);
    }
    IShape& Move(const Vector& vector) override {
        first_.Move(vector);
        second_.Move(vector);
        return *this;
    }
    bool CrossesSegment(const Segment& segment) const override;
    std::unique_ptr<IShape> Clone() const override {
        auto segment = new Segment(first_, second_);
        std::unique_ptr<IShape> clone_ptr(segment);
        return clone_ptr;
    }
    std::string ToString() override {
        std::ostringstream ostr;
        ostr << "Segment(Point(" << first_.GetX() << ", " << first_.GetY()
             << "), Point(" << second_.GetX() << ", " << second_.GetY() << "))";
        std::string str = ostr.str();
        return str;
    }
};

bool Point::CrossesSegment(const Segment& segment) const {
    Vector segment_first_to_point(segment.first_, *this);
    Vector segment_second_to_point(segment.second_, *this);
    return (VectorMultiply(segment_first_to_point, segment_second_to_point) == 0
            && ScalarMultiply(segment_first_to_point, segment_second_to_point) <= 0);
}

class Line: public IShape {
public:
    Point first_, second_;
    Vector vector_;
    std::vector<int> equation_;
    Line(const Point& point_first, const Point& point_second)
            : first_(point_first), second_(point_second), vector_(Vector(point_first, point_second)) {
        equation_.push_back(vector_[1]);
        equation_.push_back(-vector_[0]);
        equation_.push_back(-vector_[1] * first_.GetX() + vector_[0] * first_.GetY());
    }
    bool ContainsPoint(const Point& point) const override {
        return ((point.GetX() - first_.GetX()) * vector_[1] == (point.GetY() - first_.GetY()) * vector_[0]);
    }
    IShape& Move(const Vector& vector) override {
        first_.Move(vector);
        second_.Move(vector);
        vector_ = Vector(first_, second_);
        equation_.clear();
        equation_.push_back(vector_[1]);
        equation_.push_back(-vector_[0]);
        equation_.push_back(-vector_[1] * first_.GetX() + vector_[0] * first_.GetY());
        return *this;
    }
    bool CrossesSegment(const Segment& segment) const override {
        return (((equation_[0] * segment.first_.GetX() + equation_[1] * segment.first_.GetY() + equation_[2]) <= 0
                 && (equation_[0] * segment.second_.GetX() + equation_[1] * segment.second_.GetY() + equation_[2]) >= 0)
                || ((equation_[0] * segment.first_.GetX() + equation_[1] * segment.first_.GetY() + equation_[2]) >= 0
                    && (equation_[0] * segment.second_.GetX() + equation_[1] * segment.second_.GetY() + equation_[2]) <= 0));
    }
    std::unique_ptr<IShape> Clone() const override {
        auto line = new Line(first_, second_);
        std::unique_ptr<IShape> clone_ptr(line);
        return clone_ptr;
    }
    std::string ToString() override {
        std::ostringstream ostr;
        ostr << "Line(" << equation_[0] << ", " << equation_[1] << ", " << equation_[2] << ")";
        std::string str = ostr.str();
        return str;
    }
};

bool Segment::CrossesSegment(const Segment& segment) const {
    Segment segment_1(first_, second_);
    Segment segment_2(segment.first_, segment.second_);
    if (segment_1.first_ > segment_1.second_) {
        Point temp(first_.GetX(), first_.GetY());
        segment_1.first_ = segment_1.second_;
        segment_1.second_ = temp;
    }
    if (segment_2.first_ > segment_2.second_) {
        Point temp(segment_2.first_.GetX(), segment_2.first_.GetY());
        segment_2.first_ = segment_2.second_;
        segment_2.second_ = temp;
    }
    Line line_1(segment_1.first_, segment_1.second_);
    Line line_2(segment_2.first_, segment_2.second_);
    if (line_1.vector_.Length() == 0) {
        if (line_2.vector_.Length() == 0) {
            return (line_1.first_ == line_2.first_);
        }
        return (segment_2.ContainsPoint(line_1.first_));
    }
    if (line_2.vector_.Length() == 0) {
        if (line_1.vector_.Length() == 0) {
            return (line_1.first_ == line_2.first_);
        }
        return (segment_1.ContainsPoint(line_2.first_));
    }
    if (line_2.CrossesSegment(segment_1) && line_1.CrossesSegment(segment_2)) {
        Segment seg_line_1_first_to_line_2_second(line_1.first_, line_2.second_);
        Segment seg_line_2_first_to_line_1_second(line_2.first_, line_1.second_);
        if (line_2.first_ > line_1.second_) {
            seg_line_2_first_to_line_1_second.first_ = line_1.second_;
            seg_line_2_first_to_line_1_second.second_ = line_2.first_;
        }
        if (seg_line_1_first_to_line_2_second.ContainsPoint(line_1.second_)
            && seg_line_1_first_to_line_2_second.ContainsPoint(line_2.first_)) {
            if (line_2.first_.GetX() == line_1.second_.GetX()) {
                return (line_2.first_.GetY() <= line_1.second_.GetY());
            }
            return (line_2.first_.GetX() <= line_1.second_.GetX());
        }
        if (seg_line_2_first_to_line_1_second.ContainsPoint(line_2.second_)
            && seg_line_2_first_to_line_1_second.ContainsPoint(line_1.first_)) {
            if (line_1.first_.GetX() == line_2.second_.GetX()) {
                return (line_1.first_.GetY() <= line_2.second_.GetY());
            }
            return (line_1.first_.GetX() <= line_2.second_.GetX());
        }
        return true;
    }
    return false;
}

int main() {
    int point_first_x, point_first_y, point_second_x, point_second_y;
    std::cin >> point_first_x >> point_first_y >> point_second_x >> point_second_y;
    Point first_sgmt1(point_first_x, point_first_y);
    Point second_sgmt1(point_second_x, point_second_y);
    Segment first(first_sgmt1, second_sgmt1);
    std::cin >> point_first_x >> point_first_y >> point_second_x >> point_second_y;
    Point first_sgmt2(point_first_x, point_first_y);
    Point second_sgmt2(point_second_x, point_second_y);
    Segment second(first_sgmt2, second_sgmt2);
    if (second.CrossesSegment(first)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
