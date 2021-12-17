// Edmonds Carp
// Задан ориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью. Найдите максимальный поток из вершины с номером 1 в вершину с номером n.
// В этой задаче необходимо реализовать алгоритм Эдмондса-Карпа.

// Формат ввода
// Первая строка входного файла содержит n и m — количество вершин и количество ребер графа (2 ≤ n ≤ 500, 1 ≤ m ≤ 10000). Следующие m строк содержат по три числа: номера вершин, которые соединяет соответствующее ребро графа и его пропускную способность. Пропускные способности не превосходят 109.
// Формат вывода
// В выходной файл выведите одно число - величину максимального потока из вершины с номером 1 в вершину с номером n. 


#include <iostream>
#include <map>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int size) : graph_(size), prev_(size, -1){};
    void InsertOrientEdge(int from, int to, int cap);
    int LookingForFlow(int top_first, int top_second);
    int64_t FindMaxFlow(int start, int finish);

private:
    std::vector<std::map<int, int>> graph_;
    std::vector<int> prev_;
    const int max_flow_ = 1000000;
};

void Graph::InsertOrientEdge(int from, int to, int cap) {
    graph_[from][to] = cap;
}

int Graph::LookingForFlow(int top_first, int top_second) {
    prev_.assign(prev_.size(), -1);
    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(top_first, max_flow_));
    prev_[top_first] = -2;
    while (!q.empty()) {
        auto[cur_top, flow] = q.front();
        q.pop();
        for (auto next : graph_[cur_top]) {
            if (prev_[next.first] == -1 && next.second != 0) {
                prev_[next.first] = cur_top;
                int new_flow = std::min(flow, next.second);
                if (next.first == top_second) {
                    return new_flow;
                }
                q.emplace(std::make_pair(next.first, new_flow));
            }
        }
    }
    return 0;
}

int64_t Graph::FindMaxFlow(int start, int finish) {
    int flow = LookingForFlow(start, finish);
    int64_t ans = 0;
    while (flow > 0) {
        ans += flow;
        int top = finish;
        while (top != start) {
            int p = prev_[top];
            graph_[p][top] -= flow;
            graph_[top][p] += flow;
            top = p;
        }
        flow = LookingForFlow(start, finish);
    }
    return ans;
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
    std::cout << graph.FindMaxFlow(0, n - 1);
    return 0;
}
