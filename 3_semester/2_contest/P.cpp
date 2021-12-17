

// Вам задан ориентированный граф с N вершинами и M ребрами (1 ≤ N ≤ 20000, 1 ≤ M ≤ 200000). Найдите компоненты сильной связности заданного графа и топологически отсортируйте его конденсацию.

// Формат ввода
// Граф задан во входном файле следующим образом: первая строка содержит числа N и M. Каждая из следующих M строк содержит описание ребра — два целых числа из диапазона от 1 до N — номера начала и конца ребра.
// Формат вывода
// На первой строке выведите число K — количество компонент сильной связности в заданном графе. На следующей строке выведите N чисел — для каждой вершины выведите номер компоненты сильной связности, которой принадлежит эта вершина. Компоненты сильной связности должны быть занумерованы таким образом, чтобы для любого ребра номер компоненты сильной связности его начала не превышал номера компоненты сильной связности его конца. 


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
    explicit Graph(int size) : adj_(size), colors_(size), adj_transpose_(size) {};
    void InsertEdgeOrient(int ver_first, int ver_second);
    void DfsTopSortWithoutCycle(int ver, std::list<int>& graph_top_sort);
    std::list<int> TopSortWithoutCycle();
    bool IsCycle() const;
    void DfsKosaraju(int ver, int order_connective, std::vector<int>& scc);
    std::vector<int> Kosaraju();
    void UpdateColors();
    int GetCountConnectuve() const;

private:
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::vector<std::vector<int>> adj_transpose_;
    bool is_cycle_ = false;
    int count_connectivity_;
};

void Graph::InsertEdgeOrient(int ver_first, int ver_second) {
    adj_[ver_first].push_back(ver_second);
    adj_transpose_[ver_second].push_back(ver_first);
}

void Graph::DfsTopSortWithoutCycle(int ver, std::list<int> &graph_top_sort) {
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            DfsTopSortWithoutCycle(u, graph_top_sort);
        }
    }
    colors_[ver] = BLACK;
    graph_top_sort.push_front(ver);
}

std::list<int> Graph::TopSortWithoutCycle() {
    colors_.resize(colors_.size(), WHITE);
    std::list<int> graph_top_sort;
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            DfsTopSortWithoutCycle(v, graph_top_sort);
        }
    }
    return graph_top_sort;
}

bool Graph::IsCycle() const {
    return is_cycle_;
}

void Graph::DfsKosaraju(int ver, int order_connective, std::vector<int>& scc) {
    colors_[ver] = GREY;
    for (auto next: adj_transpose_[ver]) {
        if (colors_[next] == WHITE) {
            DfsKosaraju(next, order_connective, scc);
        }
    }
    colors_[ver] = BLACK;
    scc[ver] = order_connective;
}

std::vector<int> Graph::Kosaraju() {
    std::list<int> top_sort_graph = TopSortWithoutCycle();
    std::vector<int> scc(colors_.size());
    int order_connective = 1;
    UpdateColors();
    for (auto v: top_sort_graph) {
        if (colors_[v] == WHITE) {
            DfsKosaraju(v, order_connective, scc);
            order_connective++;
        }
    }
    count_connectivity_ = --order_connective;
    return scc;
}

void Graph::UpdateColors() {
    for (int i = 0; i < colors_.size(); ++i) {
        colors_[i] = WHITE;
    }
}

int Graph::GetCountConnectuve() const {
    return count_connectivity_;
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
    std::vector<int> res = graph.Kosaraju();
    std::cout << graph.GetCountConnectuve() << "\n";
    for (auto v: res) {
        std::cout << v << " ";
    }
    return 0;
}
