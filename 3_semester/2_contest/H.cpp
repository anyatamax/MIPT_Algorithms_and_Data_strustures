

// Дан ориентированный взвешенный граф. Найдите кратчайшее расстояние от одной заданной вершины до другой.

// Формат ввода
// В первой строке входного файла два числа: N и M (1 ≤ N ≤ 100, 1 ≤ M ≤ 10000), где N — количество вершин графа, а M — количество ребер.
// В следующей строке заданы числа S и F — начальная и конечная вершины.
// Далее следует M троек чисел Ai, Bi, Ti (1 ≤ Ti ≤ 10) — номера вершин соединенных ребром и вес данного ребра.
// Формат вывода
// Вывести искомое расстояние или -1, если пути между указанными вершинами не существует.


#include <iostream>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int size_n) : distance_(size_n, MAX_DISTANCE_), is_visited_(size_n, false), adj_(size_n) {
    };
    void InsertEdgeOrient(int ver_first, int ver_second, int weight);
    void MinDistance(int ver);
    int GetDistance(int ver) const;

private:
    struct Ver {
        int ver;
        int weight;
    };
    const int MAX_DISTANCE_ = 500000;
    const int MAX_WEIGHT_ = 10;
    std::vector<int> distance_;
    std::vector<bool> is_visited_;
    std::vector<std::vector<Ver>> adj_;
};

void Graph::InsertEdgeOrient(int ver_first, int ver_second, int weight) {
    Ver new_ver = {ver_second, weight};
    adj_[ver_first].push_back(new_ver);
}

void Graph::MinDistance(int start_ver) {
    distance_[start_ver] = 0;
    std::vector<std::queue<int>> q(adj_.size() * MAX_WEIGHT_ + 1);
    q[0].push(start_ver);
    for (int i = 0; i < q.size(); ++i) {
        while (!q[i].empty()) {
            int v = q[i].front();
            q[i].pop();
            if (is_visited_[v]) {
                continue;
            }
            is_visited_[v] = true;
            for (auto &u : adj_[v]) {
                int to = u.ver;
                if (distance_[to] > distance_[v] + u.weight) {
                    distance_[to] = distance_[v] + u.weight;
                    q[distance_[to]].push(to);
                 }
            }
        }
    }
}

int Graph::GetDistance(int end_ver) const {
    if (distance_[end_ver] == MAX_DISTANCE_) {
        return -1;
    } else {
        return distance_[end_ver];
    }
}

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    int start_ver = 0;
    int end_ver = 0;
    std::cin >> start_ver >> end_ver;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int weight = 0;
        std::cin >> ver_first >> ver_second >> weight;
        graph.InsertEdgeOrient(--ver_first, --ver_second, weight);
    }
    graph.MinDistance(--start_ver);
    std::cout << graph.GetDistance(--end_ver);
    return 0;
}
