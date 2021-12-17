

// Дан ориентированный граф, рёбрам которого приписаны некоторые неотрицательные веса (длины). Найти длину кратчайшего пути из вершины s в вершину t.

// Формат ввода
// В первой строке заданы три числа: число вершин в графе N (N ≤ 2 ⋅ 10^3), номера вершин s и t. Далее идёт матрица смежности графа, то есть N строк, в каждой из которых записано N чисел. j-е число в i-й строке матрицы смежности задает длину ребра, ведущего из i-й вершину в j-ю. Длины могут принимать любые значения от 0 до 106, число -1 означает отсутствие соответствующего ребра. Гарантируется, что на главной диагонали матрицы стоят нули.
// Формат вывода
// Выведите одно число — минимальную длину пути. Если пути не существует, выведите -1. 


#include <iostream>
#include <unordered_map>
#include <vector>

struct Edge {
    int from;
    int to;
    int weight;
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), dist_(size, max_distance_), size_(size){};
    void InsertOrientEdge(int ver_first, int ver_second, int weight);
    void Dijkstra(int start_ver);
    void PrintDistance(int start_ver, int end_ver);

private:
    const int max_distance_ = 2009000999;
    std::vector<std::vector<Edge>> adj_;
    std::vector<int> dist_;
    int size_;
};

void Graph::InsertOrientEdge(int ver_first, int ver_second, int weight) {
    adj_[ver_first].push_back({ver_first, ver_second, weight});
}
void Graph::Dijkstra(int start_ver) {
    dist_[start_ver] = 0;
    std::vector<bool> used(dist_.size(), false);
    for (int i = 0; i < size_; ++i) {
        int ver = -1;
        for (int j = 0; j < size_; ++j) {
            if (!used[j] && (ver == -1 || dist_[j] < dist_[ver])) {
                ver = j;
            }
        }
        if (dist_[ver] == max_distance_) {
            break;
        }
        used[ver] = true;
        for (auto& u : adj_[ver]) {
            if (dist_[ver] + u.weight < dist_[u.to]) {
                dist_[u.to] = dist_[ver] + u.weight;
            }
        }
    }
}

void Graph::PrintDistance(int start_ver, int end_ver) {
    Dijkstra(start_ver);
    if (dist_[end_ver] == max_distance_) {
        std::cout << -1;
    } else {
        std::cout << dist_[end_ver];
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int start = 0;
    int end = 0;
    std::cin >> n >> start >> end;
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int weight = 0;
            std::cin >> weight;
            if (i == j || weight == -1) {
                continue;
            }
            g.InsertOrientEdge(i, j, weight);
        }
    }
    g.PrintDistance(--start, --end);
    return 0;
}
