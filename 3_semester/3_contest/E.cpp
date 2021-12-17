
// Ford Falkerson
// Задан ориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью. Найдите максимальный поток из вершины с номером 1 в вершину с номером n.
// В этой задаче необходимо реализовать алгоритм Форда-Фалкерсона.

// Формат ввода
// Первая строка входного файла содержит n и m — количество вершин и количество ребер графа (2 ≤ n ≤ 100, 1 ≤ m ≤ 1000). Следующие m строк содержат по три числа: номера вершин, которые соединяет соответствующее ребро графа и его пропускную способность. Пропускные способности не превосходят 105.
// Формат вывода
// В выходной файл выведите одно число — величину максимального потока из вершины с номером 1 в вершину с номером n.


#include <iostream>
#include <vector>

struct Edge {
    Edge(int f, int t, int64_t fl, int64_t cap, int b) : from(f), to(t), flow(fl), capacity(cap), back(b){};
    int from;
    int to;
    int64_t flow;
    int64_t capacity;
    int back;
};

class Graph {
public:
    explicit Graph(int size) : graph_(size){};
    void InsertOrientEdge(int from, int to, int cap);
    int64_t FindFlow(int start, int finish, int64_t cur_flow, std::vector<bool> &status);
    int64_t MaxFlow(int start, int finish);

private:
    std::vector<std::vector<Edge>> graph_;
    const int max_flow_ = 1000000;
};

void Graph::InsertOrientEdge(int from, int to, int cap) {
    graph_[from].emplace_back(Edge(from, to, 0, cap, graph_[to].size()));
    graph_[to].emplace_back(Edge(to, from, 0, 0, graph_[from].size() - 1));
}

int64_t Graph::FindFlow(int start, int finish, int64_t cur_flow, std::vector<bool> &status) {
    if (start == finish) {
        return cur_flow;
    }
    status[start] = true;
    for (auto &cur_edge : graph_[start]) {
        if (!status[cur_edge.to] && cur_edge.capacity - cur_edge.flow > 0) {
            int64_t final_flow =
                FindFlow(cur_edge.to, finish, std::min(cur_flow, cur_edge.capacity - cur_edge.flow), status);
            if (final_flow > 0) {
                cur_edge.flow += final_flow;
                graph_[cur_edge.to][cur_edge.back].flow -= final_flow;
                return final_flow;
            }
        }
    }
    return 0;
}

int64_t Graph::MaxFlow(int start, int finish) {
    int64_t max_flow = 0;
    while (true) {
        std::vector<bool> status(graph_.size(), false);
        int64_t flow = FindFlow(start, finish, max_flow_, status);
        max_flow += flow;
        if (flow == 0) {
            return max_flow;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int from = 0;
        int to = 0;
        int cap = 0;
        std::cin >> from >> to >> cap;
        graph.InsertOrientEdge(--from, --to, cap);
    }
    std::cout << graph.MaxFlow(0, n - 1);
    return 0;
}
