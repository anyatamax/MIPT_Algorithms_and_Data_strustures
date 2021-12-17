

// Дан ориентированный невзвешенный граф. Необходимо его топологически отсортировать.

// Формат ввода
// В первой строке входного файла даны два целых числа N и M (1 ≤ N ≤ 100000, 0 ≤ M ≤ 100000) — количества вершин и рёбер в графе соответственно. Далее в M строках перечислены рёбра графа. Каждое ребро задаётся парой чисел - номерами начальной и конечной вершин соответственно.
// Формат вывода
// Вывести любую топологическую сортировку графа в виде последовательности номеров вершин. Если граф невозможно топологически отсортировать, вывести -1.


#include <iostream>
#include <list>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(std::vector<std::vector<int>>& list, int count_ver) : adj_(list), colors_(count_ver, WHITE) {};
    explicit Graph(int size) : adj_(size), colors_(size) {};
    void InsertEdgeOrient(int ver_first, int ver_second);
    void DfsTopSort(int v, std::list<int>& graph_top_sort);
    std::list<int> TopSort();
    bool IsCycle() const;

private:
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    bool is_cycle_ = false;
};

void Graph::InsertEdgeOrient(int ver_first, int ver_second) {
    adj_[ver_first].push_back(ver_second);
}

void Graph::DfsTopSort(int ver, std::list<int>& graph_top_sort) {
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            DfsTopSort(u, graph_top_sort);
        } else if (colors_[u] == GREY) {
            is_cycle_ = true;
            break;
        }
    }
    if (is_cycle_) {
        return;
    }
    colors_[ver] = BLACK;
    graph_top_sort.push_front(ver);
}

std::list<int> Graph::TopSort() {
    colors_.resize(colors_.size(), WHITE);
    std::list<int> graph_top_sort;
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            DfsTopSort(v, graph_top_sort);
            if (is_cycle_) {
                break;;
            }
        }
    }
    return graph_top_sort;
}

bool Graph::IsCycle() const {
    return is_cycle_;
}


int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int ver_first, ver_second;
        std::cin >> ver_first >> ver_second;
        --ver_second;
        --ver_first;
        graph.InsertEdgeOrient(ver_first, ver_second);
    }
    std::list<int> res = graph.TopSort();
    if (graph.IsCycle()) {
        std::cout << "-1";
    } else {
        for (auto v: res) {
            std::cout << ++v << " ";
        }
    }
    return 0;
}
