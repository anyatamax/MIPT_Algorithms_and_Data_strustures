

// Город будущего застроен небоскребами, для передвижения между которыми и парковки транспорта многие тройки небоскребов соединены треугольной подушкой из однополярных магнитов. Каждая подушка соединяет ровно 3 небоскреба и вид сверху на нее представляет собой треугольник, с вершинами в небоскребах. Это позволяет беспрепятственно передвигаться между соответствующими небоскребами. Подушки можно делать на разных уровнях, поэтому один небоскреб может быть соединен различными подушками с парами других, причем два небоскреба могут соединять несколько подушек (как с разными третьими небоскребами, так и с одинаковым). Например, возможны две подушки на разных уровнях между небоскребами 1, 2 и 3, и, кроме того, магнитная подушка между 1, 2, 5.
// Система магнитных подушек организована так, что с их помощью можно добираться от одного небоскреба, до любого другого в этом городе (с одной подушки на другую можно перемещаться внутри небоскреба), но поддержание каждой из них требует больших затрат энергии.
// Требуется написать программу, которая определит, какие из магнитных подушек нельзя удалять из подушечной системы города, так как удаление даже только этой подушки может привести к тому, что найдутся небоскребы из которых теперь нельзя добраться до некоторых других небоскребов, и жителям станет очень грустно.

// Формат ввода
// В первой строке входного файла находятся числа N и M — количество небоскребов в городе и количество работающих магнитных подушек соответственно (3 ≤ N ≤ 100000, 1 ≤ M ≤ 100000).
// В каждой из следующих M строк через пробел записаны три числа — номера небоскребов, соединенных подушкой. Небоскребы пронумерованы от 1 до N.
// Гарантируется, что имеющиеся воздушные подушки позволяют перемещаться от одного небоскреба до любого другого.
// Формат вывода
// Выведите в выходной файл сначала количество тех магнитных подушек, отключение которых невозможно без нарушения сообщения в городе, а потом их номера.
// Нумерация должна соответствовать тому порядку, в котором подушки перечислены во входном файле. Нумерация начинается с единицы.


#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size, int count_m) : count_ver_(size), adj_(size + count_m), colors_(size + count_m, WHITE),
                   time_up(size + count_m, MAX_TIME), time_in(size + count_m, MAX_TIME) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second, int ver_third, int index);
    void Dfs(int v, int parent);
    std::set<int> ArticulationPoints();

private:
    const int MAX_TIME = 500000;
    int count_ver_;
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::set<int> articulation_points_;
    std::vector<long int> time_up;
    std::vector<long int> time_in;
    long long int time_ = 0;
};

void Graph::InsertEdgeNoOrient(int ver_first, int ver_second, int ver_third, int index) {
    adj_[count_ver_ + index].push_back(ver_first);
    adj_[count_ver_ + index].push_back(ver_second);
    adj_[count_ver_ + index].push_back(ver_third);
    adj_[ver_first].push_back(count_ver_ + index);
    adj_[ver_second].push_back(count_ver_ + index);
    adj_[ver_third].push_back(count_ver_ + index);
}

void Graph::Dfs(int ver, int parent) {
    time_in[ver] = time_;
    time_up[ver] = time_;
    ++time_;
    long int count_children = 0;
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (u == parent) {
            continue;
        } else if (colors_[u] == GREY) {
            time_up[ver] = std::min(time_up[ver], time_in[u]);
        } else if (colors_[u] == WHITE) {
            Dfs(u, ver);
            ++count_children;
            time_up[ver] = std::min(time_up[u], time_up[ver]);
            if (parent != -1 && time_in[ver] <= time_up[u]) {
                if (ver >= count_ver_) {
                    articulation_points_.insert(ver - count_ver_);
                }
            }
        }
    }
    if (parent == -1 && count_children > 1) {
        if (ver >= count_ver_) {
            articulation_points_.insert(ver - count_ver_);
        }
    }
    colors_[ver] = BLACK;
}

std::set<int> Graph::ArticulationPoints() {
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            Dfs(v, -1);
        }
    }
    return articulation_points_;
}


int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph g(n, m);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int ver_third = 0;
        std::cin >> ver_first >> ver_second >> ver_third;
        g.InsertEdgeNoOrient(--ver_first, --ver_second, --ver_third, i);
    }
    std::set<int> result = g.ArticulationPoints();
    std::cout << result.size() << "\n";
    for (auto & i : result) {
        std::cout << i + 1 << "\n";
    }
    return 0;
}
