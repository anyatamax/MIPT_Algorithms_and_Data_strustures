

// Шел 2100-й год. Физтех стал огромным ВУЗом, состоящим из большого множества корпусов и гипертонеллей в качестве переходов между ними. Отныне до Тимирязевской (как и от любого другого корпуса) можно добраться за 5 минут от общежития (или любого другого здания университета) с помощью гиперлупов (напрямую или с пересадочными станциями в каждом корпусе).
// К сожалению, иногда переходы могут быть обесточены во время проведения ремонтных работ, а значит перемещаться по ним нельзя. Нужно выяснить, сколько гипертоннелей для гиперлупов надо построить, чтобы при неисправности любого из переходов физтех оставался связным.
// Учитывая то, что систему разрабатывали самые лучшие ученые страны, в ней нет бессмысленных кратных ребер и петель.

// Формат ввода
// Первая строка содержит два числа n (3 ≤ n ≤ 5000) — количество зданий на физтехе и m (1 ≤ m ≤ 100000) — количество построенных гипертоннелей. Следующие m строк содержат два числа vi, ui (1 ≤ vi, ui ≤ n) — описание гипертоннелей.
// Формат вывода
// Выведите минимальное количество гипертоннелей, которые нужно достроить, чтобы при ремонте любого из них физтех оставался связным.


#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), colors_(size, WHITE), time_up(size, MAX_TIME), time_in(size, MAX_TIME), colors_components_(size, -1) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second, int index);
    void Dfs(int v, int parent);
    void Bridges();
    std::pair<int, int> ReturnEdges(int ver_first, int ver_second) const;
    void Paint(int ver, int color);
    void PaintGraph();
    int CountEdges();

private:
    const long int MAX_TIME = 5000000000;
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::set<std::pair<int, int>> bridges_;
    std::vector<long int> time_up;
    std::vector<long int> time_in;
    std::map<std::pair<int, int>, int> number_edges_;
    std::map<std::pair<int, int>, int> count_edges_;
    long long int time_ = 1;
    std::vector<int> colors_components_;
    std::vector<int> count_edges_from_components_;
    int max_color_ = -1;
};

int Graph::CountEdges() {
    Bridges();
    PaintGraph();
    count_edges_from_components_.resize(max_color_ + 1, 0);
    for (auto &edge: bridges_) {
        count_edges_from_components_[colors_components_[edge.first]]++;
        count_edges_from_components_[colors_components_[edge.second]]++;
    }
    int count = 0;
    for (auto &i: count_edges_from_components_) {
        if (i == 1) {
            ++count;
        }
    }
    return (count + 1) / 2;
}

void Graph::Paint(int ver, int color) {
    colors_components_[ver] = color;
    for (auto &u: adj_[ver]) {
        if (colors_components_[u] == -1) {
            if (time_up[u] > time_in[ver]) {
                ++max_color_;
                Paint(u, max_color_);
            } else {
                Paint(u, color);
            }
        }
    }
}

void Graph::PaintGraph() {
    for (int i = 0; i < adj_.size(); ++i) {
        if (colors_components_[i] == -1) {
            ++max_color_;
            Paint(i, max_color_);
        }
    }
}



std::pair<int, int> Graph::ReturnEdges(int ver_first, int ver_second) const {
    if (ver_first > ver_second) {
        return std::make_pair(ver_second, ver_first);
    }
    return std::make_pair(ver_first, ver_second);
}

void Graph::InsertEdgeNoOrient(int ver_first, int ver_second, int index) {
    if (std::find(adj_[ver_first].begin(), adj_[ver_first].end(), ver_second) == adj_[ver_first].end()) {
        adj_[ver_first].push_back(ver_second);
    }
    if (std::find(adj_[ver_second].begin(), adj_[ver_second].end(), ver_first) == adj_[ver_second].end()) {
        adj_[ver_second].push_back(ver_first);
    }
    if (number_edges_.find(ReturnEdges(ver_first, ver_second)) == number_edges_.end()) {
        number_edges_[ReturnEdges(ver_first, ver_second)] = index;
        count_edges_[ReturnEdges(ver_first, ver_second)] = 1;
    } else {
        ++count_edges_[ReturnEdges(ver_first, ver_second)];
    }
}

void Graph::Dfs(int ver, int parent) {
    time_in[ver] = time_;
    time_up[ver] = time_;
    ++time_;
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (u == parent && count_edges_[ReturnEdges(ver, u)] == 1) {
            continue;
        } else if (colors_[u] == GREY) {
            time_up[ver] = std::min(time_up[ver], time_in[u]);
        } else if (colors_[u] == WHITE) {
            Dfs(u, ver);
            time_up[ver] = std::min(time_up[u], time_up[ver]);
            if (time_in[ver] < time_up[u]) {
                bridges_.insert(ReturnEdges(ver, u));
            }
        }
    }
    colors_[ver] = BLACK;
}

void Graph::Bridges() {
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            Dfs(v, -1);
        }
    }
}



int main() {
    int m = 0;
    int n = 0;
    std::cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        std::cin >> ver_first >> ver_second;
        g.InsertEdgeNoOrient(--ver_first, --ver_second, i);
    }
    std::cout << g.CountEdges();
    return 0;
}
