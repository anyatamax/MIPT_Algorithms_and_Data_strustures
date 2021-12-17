

// Дан неориентированный граф. Требуется найти все мосты в нем.

// Формат ввода
// В первой строке входного файла два натуральных числа n и m (1 ≤ n ≤ 20000, 1 ≤ m ≤ 200000) –количество вершин и рёбер в графе соответственно. Далее в m строках перечислены рёбра графа. Каждое ребро задается парой чисел – номерами начальной и конечной вершин соответственно.
// Формат вывода
// Первая строка выходного файла должна содержать одно натуральное число b – количество мостов в заданном графе. На следующей строке выведите b чисел – номера ребер, которые являются мостами, в возрастающем порядке. Ребра нумеруются с единицы в том порядке, в котором они заданы во входном файле. 


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
    explicit Graph(int size) : adj_(size), colors_(size, WHITE), time_up(size, MAX_TIME), time_in(size, MAX_TIME) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second, int index);
    void Dfs(int v, int parent);
    std::set<int> Bridges();
    std::pair<int, int> ReturnEdges(int ver_first, int ver_second) const;

private:
    const long int MAX_TIME = 5000000000;
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::set<int> bridges_;
    std::vector<long int> time_up;
    std::vector<long int> time_in;
    std::map<std::pair<int, int>, int> number_edges_;
    std::map<std::pair<int, int>, int> count_edges_;
    long long int time_ = 1;
};

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
                bridges_.insert(number_edges_[ReturnEdges(ver, u)]);
            }
        }
    }
    colors_[ver] = BLACK;
}

std::set<int> Graph::Bridges() {
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            Dfs(v, -1);
        }
    }
    return bridges_;
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
    std::set<int> result = g.Bridges();
    std::cout << result.size() << "\n";
    for (auto & i : result) {
        std::cout << i + 1 << "\n";
    }
    return 0;
}
