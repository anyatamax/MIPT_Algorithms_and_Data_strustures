

// Полный ориентированный взвешенный граф задан матрицей смежности. Постройте матрицу кратчайших путей между его вершинами. Гарантируется, что в графе нет циклов отрицательного веса.

// Формат ввода
// В первой строке вводится единственное число N (1 ≤ N ≤ 100) — количество вершин графа. В следующих N строках по N чисел задается матрица смежности графа (j-е число в i-й строке соответствует весу ребра из вершины i в вершину j). Все числа по модулю не превышают 100. На главной диагонали матрицы всегда нули.
// Формат вывода
// Выведите N строк по N чисел — матрицу кратчайших расстояний между парами вершин. j-е число в i-й строке должно быть равно весу кратчайшего пути из вершины i в вершину j.


#include <iostream>
#include <vector>

class Graph {
public:
    explicit Graph(int size) : graph_(size, std::vector<int>(size)), size_(size){};
    void InsertOrientEdge(int from, int to, int len);
    void FloydWarshall();
    void PrintDist();

private:
    std::vector<std::vector<int>> graph_;
    int size_;
};

void Graph::InsertOrientEdge(int from, int to, int len) {
    graph_[from][to] = len;
}

void Graph::FloydWarshall() {
    for (int k = 0; k < size_; k++) {
        for (int i = 0; i < size_; i++) {
            for (int j = 0; j < size_; j++) {
                graph_[i][j] = std::min(graph_[i][k] + graph_[k][j], graph_[i][j]);
            }
        }
    }
}

void Graph::PrintDist() {
    FloydWarshall();
    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j) {
            std::cout << graph_[i][j] << " ";
        }
        std::cout << "\n";
    }
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
            graph.InsertOrientEdge(i, j, weight);
        }
    }
    graph.PrintDist();
    return 0;
}
