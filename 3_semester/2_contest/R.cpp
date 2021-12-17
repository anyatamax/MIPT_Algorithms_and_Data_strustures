

// Найдите точки сочленения в неориентированном графе.

// Формат ввода
// Первая строка входного файла содержит два натуральных числа n и m — количества вершин и рёбер графа соответственно (1 ≤ n ≤ 20000, 1 ≤ m ≤ 200000).
// Ребро номер i описывается двумя натуральными числами bi, ei — номерами концов ребра (1 ≤ bi, ei ≤ n).
// Формат вывода
// Первая строка выходного файла должна содержать одно натуральное число b — количество точек сочленения в заданном графе. На следующих b строках выведите b целых чисел — номера вершин, которые являются точками сочленения, в возрастающем порядке. 



#include <algorithm>
#include <iostream>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), colors_(size, WHITE), time_up(size, MAX_TIME), time_in(size, MAX_TIME) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second);
    void Dfs(int v, int parent);
    std::vector<int> ArticulationPoints();

private:
    const int MAX_TIME = 500000;
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> articulation_points_;
    std::vector<long int> time_up;
    std::vector<long int> time_in;
    long long int time_ = 0;
};

void Graph::InsertEdgeNoOrient(int ver_first, int ver_second) {
    adj_[ver_first].push_back(ver_second);
    adj_[ver_second].push_back(ver_first);
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
                if (std::find(articulation_points_.begin(), articulation_points_.end(), ver) == articulation_points_.end()) {
                    articulation_points_.push_back(ver);
                }
            }
        }
    }
    if (parent == -1 && count_children > 1) {
        if (std::find(articulation_points_.begin(), articulation_points_.end(), ver) == articulation_points_.end()) {
            articulation_points_.push_back(ver);
        }
    }
    colors_[ver] = BLACK;
}

std::vector<int> Graph::ArticulationPoints() {
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
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        std::cin >> ver_first >> ver_second;
        g.InsertEdgeNoOrient(--ver_first, --ver_second);
    }
    std::vector<int> result = g.ArticulationPoints();
    std::cout << result.size() << "\n";
    std::sort(result.begin(), result.end());
    for (auto & i : result) {
        std::cout << i + 1 << "\n";
    }
    return 0;
}
