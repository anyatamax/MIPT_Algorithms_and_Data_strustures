// Dinitsa

// Задан ориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью. Найдите максимальный поток из вершины с номером 1 в вершину с номером n.

// В этой задаче необходимо реализовать алгоритм Диница.

// Формат ввода
// Первая строка входного файла содержит n и m — количество вершин и количество ребер графа (2 ≤ n ≤ 500, 1 ≤ m ≤ 10000). Следующие m строк содержат по три числа: номера вершин, которые соединяет соответствующее ребро графа и его пропускную способность. Пропускные способности не превосходят 109.
// Формат вывода
// В выходной файл выведите одно число - величину максимального потока из вершины с номером 1 в вершину с номером n. 


#include <iostream>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int size)
        : weights_(size, std::vector<int64_t>(size, 0)), flow_(size, std::vector<int64_t>(size, 0)){};
    void InsertNoOrientEdge(int ver_first, int ver_second, int weight) {
        if (ver_first != ver_second) {
            weights_[ver_first][ver_second] += weight;
        }
    }
    bool BuildNetwork(int start_ver) {
        std::queue<int> queue;
        queue.push(start_ver);
        dist_.assign(weights_.size(), -1);
        dist_[start_ver] = 0;
        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            for (int64_t i = 0; i < static_cast<int64_t>(weights_.size()); ++i) {
                if (dist_[i] == -1 && flow_[v][i] < weights_[v][i]) {
                    queue.push(i);
                    dist_[i] = dist_[v] + 1;
                }
            }
        }
        return dist_[weights_.size() - 1] != -1;
    }
    int64_t FindFlow(int64_t v, int64_t flow) {
        if (flow == 0) {
            return 0;
        }
        if (v == static_cast<int64_t>(weights_.size()) - 1) {
            return flow;
        }
        for (int64_t &to = ptr_[v]; to < static_cast<int64_t>(weights_.size()); ++to) {
            if (dist_[to] != dist_[v] + 1) {
                continue;
            }
            int64_t new_flow = FindFlow(to, std::min(flow, weights_[v][to] - flow_[v][to]));
            if (new_flow != 0) {
                flow_[v][to] += new_flow;
                flow_[to][v] -= new_flow;
                return new_flow;
            }
        }
        return 0;
    }
    int64_t Dinic(int start_ver) {
        int64_t flow = 0;
        for (;;) {
            if (!BuildNetwork(start_ver)) {
                break;
            }
            ptr_.assign(weights_.size(), 0);
            while (true) {
                int64_t new_flow = FindFlow(start_ver, max_flow_);
                if (new_flow == 0) {
                    break;
                }
                flow += new_flow;
            }
        }
        return flow;
    }

private:
    std::vector<std::vector<int64_t>> weights_;
    std::vector<std::vector<int64_t>> flow_;
    std::vector<int> dist_;
    std::vector<int64_t> ptr_;
    const int64_t max_flow_ = 1000000000000001;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int weight = 0;
        std::cin >> ver_first >> ver_second >> weight;
        g.InsertNoOrientEdge(--ver_first, --ver_second, weight);
    }
    std::cout << g.Dinic(0);
    return 0;
}
