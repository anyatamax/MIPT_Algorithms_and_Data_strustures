

// Дан ориентированный граф, в котором могут быть кратные ребра и петли. Каждое ребро имеет вес, выражающийся целым числом (возможно, отрицательным). Гарантируется, что циклы отрицательного веса отсутствуют.
// Требуется посчитать длины кратчайших путей от вершины номер 1 до всех остальных вершин.

// Формат ввода
// Программа получает сначала число N (1 ≤ N ≤ 100) — количество вершин графа и число M (0 ≤ M ≤ 10000) — количество ребер. В следующих строках идет M троек чисел, описывающих ребра: начало ребра, конец ребра и вес (вес — целое число от -100 до 100).
// Формат вывода
// Программа должна вывести N чисел — расстояния от вершины номер 1 до всех вершин графа. Если пути до соответствующей вершины не существует, вместо длины пути выведите число 30000.



#include <iostream>
#include <vector>

struct Edge {
    Edge(int f, int t, int w) : from(f), to(t), weight(w){};
    int from;
    int to;
    int weight;
};

class Graph {
public:
    explicit Graph(int size) : size_graph_(size){};
    void InsertOrientEdge(int ver_first, int ver_second, int w);
    void FordBellman(int start_ver);
    void PrintDist();

private:
    int size_graph_;
    std::vector<Edge> graph_;
    std::vector<int> dist_;
    const int max_dist_ = 1000000000;
};

void Graph::InsertOrientEdge(int ver_first, int ver_second, int w) {
    graph_.emplace_back(Edge(ver_first, ver_second, w));
}

void Graph::FordBellman(int start_ver) {
    dist_.assign(size_graph_, max_dist_);
    dist_[start_ver] = 0;
    for (int ver = 0; ver < size_graph_; ++ver) {
        for (auto &edge : graph_) {
            if (dist_[edge.from] < max_dist_) {
                if (dist_[edge.to] > dist_[edge.from] + edge.weight) {
                    dist_[edge.to] = dist_[edge.from] + edge.weight;
                }
            }
        }
    }
}

void Graph::PrintDist() {
    FordBellman(0);
    for (int ver = 0; ver < size_graph_; ++ver) {
        if (dist_[ver] == max_dist_) {
            std::cout << 30000 << " ";
        } else {
            std::cout << dist_[ver] << " ";
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
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int weight = 0;
        std::cin >> ver_first >> ver_second >> weight;
        g.InsertOrientEdge(--ver_first, --ver_second, weight);
    }
    g.PrintDist();
    return 0;
}
