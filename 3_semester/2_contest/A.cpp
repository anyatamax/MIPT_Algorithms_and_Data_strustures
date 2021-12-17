

// Вам дан неориентированный граф. Найдите кратчайший путь от вершины a до вершины b.

// Формат ввода
// В первой строке входного файла идут целые числа n и m (1 ≤ n ≤ 50 000, 1 ≤ m ≤ 100 000) — количества вершин и рёбер соответственно. Во второй строке идут целые числа a и b — стартовая и конечная вершины соответственно. Далее идут m строк, описывающих рёбра.
// Формат вывода
// Если пути между a и b нет, выведите единственное число -1. Иначе выведите в первой строке число l — длину кратчайшего пути между этими двумя вершинами в рёбрах, а во второй строке выведите l + 1 число — вершины этого пути. 


#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int size) : adj_(size), distance_(size, MAX_DISTANCE_), parent_(size) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second);
    void Bfs(int ver);
    int GetDistance(int ver);
    void PrintWay(int ver);

private:
    const int MAX_DISTANCE_ = 50001;
    std::vector<std::vector<int>> adj_;
    std::vector<int> distance_;
    std::vector<int> parent_;
};

void Graph::Bfs(int ver) {
    std::queue<int> queue_graph;
    queue_graph.push(ver);
    distance_[ver] = 0;
    parent_[ver] = -1;
    while(!queue_graph.empty()) {
        int new_ver = queue_graph.front();
        queue_graph.pop();
        for (auto u: adj_[new_ver]) {
            if (distance_[u] == MAX_DISTANCE_) {
                distance_[u] = distance_[new_ver] + 1;
                parent_[u] = new_ver;
                queue_graph.push(u);
            }
        }
    }
}
int Graph::GetDistance(int ver) {
    if (distance_[ver] == MAX_DISTANCE_) {
        return -1;
    }
    return  distance_[ver];
}
void Graph::PrintWay(int ver) {
    if (parent_[ver] == -1) {
        std::cout << ver + 1 << " ";
        return;
    }
    PrintWay(parent_[ver]);
    std::cout << ver + 1 << " ";
}

void Graph::InsertEdgeNoOrient(int ver_first, int ver_second) {
    if (std::find(adj_[ver_first].begin(), adj_[ver_first].end(), ver_second) == adj_[ver_first].end()) {
        adj_[ver_first].push_back(ver_second);
    }
    if (std::find(adj_[ver_second].begin(), adj_[ver_second].end(), ver_first) == adj_[ver_second].end()) {
        adj_[ver_second].push_back(ver_first);
    }
}


int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph graph(n);
    int start_ver = 0;
    int end_ver = 0;
    std::cin >> start_ver >> end_ver;
    --start_ver;
    --end_ver;
    for (int i = 0; i < m; ++i) {
        int ver_first, ver_second;
        std::cin >> ver_first >> ver_second;
        --ver_second;
        --ver_first;
        graph.InsertEdgeNoOrient(ver_first, ver_second);
    }
    graph.Bfs(start_ver);
    int dist = graph.GetDistance(end_ver);
    std::cout << dist << "\n";
    if (dist != -1) {
        graph.PrintWay(end_ver);
    }
    return 0;
}
