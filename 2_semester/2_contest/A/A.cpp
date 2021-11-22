#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>
#include <string>

namespace geometry {

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


    class Ray: public IShape {
    public:
        Point first_, second_;
        Vector vector_;
        Ray(const Point& first, const Point& second) : first_(first), second_(second), vector_(Vector(first, second)) {
        }
        bool ContainsPoint(const Point& point) const override {
            Vector first_to_point(first_, point);
            return ((first_to_point[0] * vector_[1] - first_to_point[1] * vector_[0] == 0)
                    && (vector_[0] * first_to_point[0] + vector_[1] * first_to_point[1] >= 0));
        }
        IShape& Move(const Vector& vector) override {
            first_.Move(vector);
            return *this;
        }
        bool CrossesSegment(const Segment& segment) const override {
            if (ContainsPoint(segment.first_) || ContainsPoint(segment.second_) || segment.ContainsPoint(first_)) {
                return true;
            }
            Vector point_to_sgmt_first(first_, segment.first_);
            Vector point_to_sgmt_second(first_, segment.second_);
            if (VectorMultiply(point_to_sgmt_first, point_to_sgmt_second) == 0) {
                if (VectorMultiply(point_to_sgmt_first, vector_) == 0) {
                    return (ScalarMultiply(point_to_sgmt_first, vector_) >= 0
                            || ScalarMultiply(point_to_sgmt_second, vector_) >= 0);
                }
            }
            return ((VectorMultiply(point_to_sgmt_first, vector_) > 0
                     && VectorMultiply(vector_, point_to_sgmt_second) > 0
                     && VectorMultiply(point_to_sgmt_first, point_to_sgmt_second) > 0)
                    || (VectorMultiply(point_to_sgmt_first, vector_) < 0
                        && VectorMultiply(vector_, point_to_sgmt_second) < 0
                        && VectorMultiply(point_to_sgmt_first, point_to_sgmt_second) < 0));
        }
        std::unique_ptr<IShape> Clone() const override {
            auto ray = new Ray(first_, second_);
            std::unique_ptr<geometry::IShape> clone_ptr(ray);
            return clone_ptr;
        }
        std::string ToString() override {
            std::ostringstream ostr;
            ostr << "Ray(Point(" << first_.GetX() << ", " << first_.GetY() << "), Vector(" << vector_[0] << ", " << vector_[1] << "))";
            std::string str = ostr.str();
            return str;
        }
    };

    class Polygon: public IShape {
        std::vector<geometry::Point> points_;
    public:
        explicit Polygon(const std::vector<geometry::Point>& points) : points_(points) {
        }
        bool ContainsPoint(const Point& point) const override {
            Point point_another(point.GetX() + 1, point.GetY());
            Ray ray(point, point_another);
            int count = 0;
            for (size_t i = 0; i < points_.size() - 1; i++) {
                Segment side(points_[i], points_[i + 1]);
                if (side.ContainsPoint(point)) {
                    return true;
                }
                if (ray.CrossesSegment(side) && !(ray.ContainsPoint(side.first_) && ray.ContainsPoint(side.second_))) {
                    if (ray.ContainsPoint(side.first_)) {
                        if (side.first_.GetY() > side.second_.GetY()) {
                            count++;
                        }
                    } else if (ray.ContainsPoint(side.second_)) {
                        if (side.second_.GetY() > side.first_.GetY()) {
                            count++;
                        }
                    } else {
                        count++;
                    }
                }
            }
            Segment side(points_[points_.size() - 1], points_[0]);
            if (ray.CrossesSegment(side) && !(ray.ContainsPoint(side.first_) && ray.ContainsPoint(side.second_))) {
                if (ray.ContainsPoint(side.first_)) {
                    if (side.first_.GetY() > side.second_.GetY()) {
                        count++;
                    }
                } else if (ray.ContainsPoint(side.second_)) {
                    if (side.second_.GetY() > side.first_.GetY()) {
                        count++;
                    }
                } else {
                    count++;
                }
            }
            return (count % 2 != 0);
        }
        IShape& Move(const Vector& vector) override {
            for (size_t i = 0; i < points_.size(); i++) {
                points_[i].Move(vector);
            }
            return *this;
        }
        bool CrossesSegment(const Segment& segment) const override {
            for (size_t i = 0; i < points_.size() - 1; i++) {
                Segment side(points_[i], points_[i + 1]);
                if (segment.CrossesSegment(side)) {
                    return true;
                }
            }
            Segment side(points_[points_.size() - 1], points_[0]);
            return (segment.CrossesSegment(side));
        }
        std::unique_ptr<IShape> Clone() const override {
            auto  polygon = new Polygon(points_);
            std::unique_ptr<geometry::IShape> clone_ptr(polygon);
            return clone_ptr;
        }
        std::string ToString() override {
            std::ostringstream ostr;
            ostr << "Polygon(";
            for (size_t i = 0; i < points_.size(); i++) {
                if (i == points_.size() - 1) {
                    ostr << "Point(" << points_[i].GetX() << ", " << points_[i].GetY() << "))";
                } else {
                    ostr << "Point(" << points_[i].GetX() << ", " << points_[i].GetY() << "), ";
                }
            }
            std::string str = ostr.str();
            return str;
        }
    };

    double ToLine(const Point& centre, const Point& first, const Point& second, Vector& first_to_second) {
        double result = static_cast<double>(first_to_second[1] * centre.GetX() - first_to_second[0] * centre.GetY() + first.GetY()
                                                                                                                      * first_to_second[0] - first.GetX() * first_to_second[1])
                        / std::sqrt(first_to_second[1] * first_to_second[1] + first_to_second[0] * first_to_second[0]);
        if (result < 0) {
            return (-result);
        }
        return result;
    }

    double ToSegment(const Point& centre, const Point& first, const Point& second, Vector& first_to_second, Vector& second_to_first) {
        Vector first_to_centre(first, centre);
        Vector second_to_centre(second, centre);
        if (first_to_centre[0] * first_to_second[0] + first_to_centre[1] * first_to_second[1] < 0) {
            return std::sqrt(first_to_centre[0] * first_to_centre[0] + first_to_centre[1] * first_to_centre[1]);
        }
        if (second_to_centre[0] * second_to_first[0] + second_to_centre[1] * second_to_first[1] < 0) {
            return std::sqrt(second_to_centre[0] * second_to_centre[0] + second_to_centre[1] * second_to_centre[1]);
        }
        return ToLine(centre, first, second, first_to_second);
    }

    class Circle: public IShape {
        Point centre_;
        int radius_;
    public:
        Circle(const Point& centre, int radius) : centre_(centre), radius_(radius) {
        }
        bool ContainsPoint(const Point& point) const override {
            Vector dist(centre_, point);
            return (dist.Length() <= static_cast<double>(radius_));
        }
        IShape& Move(const Vector& vector) override {
            centre_.Move(vector);
            return *this;
        }
        bool CrossesSegment(const Segment& segment) const override {
            Vector centre_to_first(centre_, segment.first_);
            Vector centre_to_second(centre_, segment.second_);
            if (centre_to_first.Length() < static_cast<double>(radius_) && centre_to_second.Length() < static_cast<double>(radius_)) {
                return false;
            }
            if ((centre_to_first.Length() < static_cast<double>(radius_) && centre_to_second.Length() >= static_cast<double>(radius_))
                || (centre_to_first.Length() >= static_cast<double>(radius_) && centre_to_second.Length() < static_cast<double>(radius_))) {
                return true;
            }
            Vector sgmt_second_to_first(segment.second_, segment.first_);
            Vector sgmt_first_to_second(segment.first_, segment.second_);
            return (ToSegment(centre_, segment.first_, segment.second_, sgmt_first_to_second, sgmt_second_to_first) <= static_cast<double>(radius_));
        }
        std::unique_ptr<IShape> Clone() const override {
            auto  circle = new Circle(centre_, radius_);
            std::unique_ptr<geometry::IShape> clone_ptr(circle);
            return clone_ptr;
        }
        std::string ToString() override {
            std::ostringstream ostr;
            ostr << "Circle(Point(" << centre_.GetX() << ", " << centre_.GetY() << "), " << radius_ << ")";
            std::string str = ostr.str();
            return str;
        }
    };



}  // namespace geometry

template <class SmartPtrT>
void Delete(const SmartPtrT&) {
}

template <class T>
void Delete(T* ptr) {
    delete ptr;
}

void CheckFunctions(const geometry::IShape* shape_ptr, const geometry::Point& point_a, const geometry::Point& point_b) {
    std::cout << "Given shape " << (shape_ptr->ContainsPoint(point_a) ? "contains" : "does not contain")
              << " point A\n";

    const auto segment_ab = geometry::Segment(point_a, point_b);
    std::cout << "Given shape " << (shape_ptr->CrossesSegment(segment_ab) ? "crosses" : "does not cross")
              << " segment AB\n";

    const auto vector_ab = point_b - point_a;
    const auto cloned_shape_ptr = shape_ptr->Clone();  // may return either raw or smart pointer
    std::cout << cloned_shape_ptr->Move(vector_ab).ToString();

    Delete(cloned_shape_ptr);  // raw pointer compatibility
}

int main() {
    std::unique_ptr<geometry::IShape> shape_ptr;

    std::string command;
    std::cin >> command;

    int x = 0;
    int y = 0;
    int a = 0;
    int b = 0;

    if (command == "point") {
        std::cin >> x >> y;
        shape_ptr = std::make_unique<geometry::Point>(x, y);
    } else if (command == "segment") {
        std::cin >> x >> y >> a >> b;
        shape_ptr = std::make_unique<geometry::Segment>(geometry::Point(x, y), geometry::Point(a, b));
    } else if (command == "ray") {
        std::cin >> x >> y >> a >> b;
        shape_ptr = std::make_unique<geometry::Ray>(geometry::Point(x, y), geometry::Point(a, b));
    } else if (command == "line") {
        std::cin >> x >> y >> a >> b;
        shape_ptr = std::make_unique<geometry::Line>(geometry::Point(x, y), geometry::Point(a, b));
    } else if (command == "polygon") {
        size_t n_points = 0;
        std::cin >> n_points;
        std::vector<geometry::Point> points;
        points.reserve(n_points);
        for (size_t i = 0; i < n_points; ++i) {
            std::cin >> x >> y;
            points.emplace_back(x, y);
        }
        shape_ptr = std::make_unique<geometry::Polygon>(std::move(points));
    } else if (command == "circle") {
        std::cin >> x >> y;
        const auto center = geometry::Point(x, y);
        uint64_t radius = 0;
        std::cin >> radius;
        shape_ptr = std::make_unique<geometry::Circle>(center, radius);
    } else {
        std::cerr << "Undefined command" << std::endl;
        return 1;
    }

    std::cin >> x >> y;
    const auto point_a = geometry::Point(x, y);
    std::cin >> x >> y;
    const auto point_b = geometry::Point(x, y);

    CheckFunctions(shape_ptr.get(), point_a, point_b);
    return 0;
}
