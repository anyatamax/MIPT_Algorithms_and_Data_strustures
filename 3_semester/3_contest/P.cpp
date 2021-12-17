// Дан ориентированный граф. Определить, есть ли в нем цикл отрицательного веса, и если да, то вывести его.

// Формат ввода
// В первой строке содержится число N (1 ≤ N ≤ 100) — количество вершин графа. В следующих N строках находится по N чисел — матрица смежности графа. Веса ребер по модулю меньше 100000. Если ребра нет, соответствующее значение равно 100000.
// Формат вывода
// В первой строке выведите YES, если цикл существует, или NO, в противном случае.
// При наличии цикла выведите во второй строке количество вершин в нем (считая одинаковые — первую и последнюю), а в третьей строке — вершины, входящие в этот цикл, в порядке обхода. Если циклов несколько, то выведите любой из них.


#include <iostream>
#include <vector>

struct Edge {
    int from;
    int to;
    int weight;
};

class Graph {
public:
    explicit Graph(int size) : parent_(size, -1), dist_(size, max_dist_), size_(size){};
    void InsertOrientEdge(int from, int to, int len);
    int FordBellman(int s);
    std::vector<int> FindCycle(int start_ver);

private:
    std::vector<Edge> graph_;
    std::vector<int> parent_;
    std::vector<int> dist_;
    int size_;
    const int max_dist_ = std::numeric_limits<int>::max();
    const int max_weight_ = 100000;
};

void Graph::InsertOrientEdge(int from, int to, int len) {
    graph_.push_back({from, to, len});
}

int Graph::FordBellman(int s) {
    dist_.assign(size_, max_dist_);
    dist_[s] = 0;
    parent_.assign(size_, -1);
    for (int k = 0; k < size_ - 1; ++k) {
        bool relaxed = false;
        for (auto &edge : graph_) {
            if (dist_[edge.from] < max_dist_) {
                if (dist_[edge.to] > dist_[edge.from] + edge.weight) {
                    dist_[edge.to] = dist_[edge.from] + edge.weight;
                    parent_[edge.to] = edge.from;
                    relaxed = true;
                }
            }
        }
        if (!relaxed) {
            break;
        }
    }
    for (auto &edge : graph_) {
        if (dist_[edge.from] < max_dist_) {
            if (dist_[edge.to] > dist_[edge.from] + edge.weight) {
                dist_[edge.to] = dist_[edge.from] + edge.weight;
                parent_[edge.to] = edge.from;
                return edge.to;
            }
        }
    }
    return -1;
}

std::vector<int> Graph::FindCycle(int start_ver) {
    int ver = start_ver;
    for (int i = 0; i < size_ - 1; ++i) {
        ver = parent_[ver];
    }
    std::vector<int> cycle;
    cycle.push_back(ver);
    int start = parent_[ver];
    while (start != ver) {
        cycle.push_back(start);
        start = parent_[start];
    }
    cycle.push_back(ver);
    return cycle;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    std::cin >> n;
    Graph graph(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int weight = 0;
            std::cin >> weight;
            if (weight != 100000) {
                graph.InsertOrientEdge(i, j, weight);
            }
        }
    }
    int ver = -1;
    for (int s = 0; s < n; ++s) {
        ver = graph.FordBellman(s);
        if (ver != -1) {
            break;
        }
    }
    if (ver == -1) {
        std::cout << "NO";
    } else {
        std::vector<int> result = graph.FindCycle(ver);
        std::cout << "YES\n";
        int size = result.size();
        std::cout << size << "\n";
        for (int i = size - 1; i >= 0; --i) {
            std::cout << result[i] + 1 << " ";
        }
    }
    return 0;
}
