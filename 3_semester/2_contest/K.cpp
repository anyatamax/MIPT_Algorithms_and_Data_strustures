// Дан ориентированный невзвешенный граф. Необходимо определить есть ли в нём циклы, и если есть, то вывести любой из них.

// Формат ввода
// В первой строке входного файла находятся два натуральных числа N и M (1 ≤ N ≤ 100000, M ≤ 100000) — количество вершин и рёбер в графе соответственно. Далее в M строках перечислены рёбра графа. Каждое ребро задаётся парой чисел — номерами начальной и конечной вершин соответственно.
// Формат вывода
// Если в графе нет цикла, то вывести NO, иначе — YES и затем перечислить все вершины в порядке обхода цикла. 


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
    explicit Graph(int size) : adj_(size), colors_(size, WHITE), parents_(size) {};
    void InsertEdgeOrient(int ver_first, int ver_second);
    void Dfs(int v);
    bool HasCycle();
    void PrintCycle() const;

private:
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> parents_;
    std::vector<int> cycle_;
    bool is_cycle_ = false;
};

void Graph::PrintCycle() const {
    int end = cycle_.back();
    int index = cycle_.size() - 2;
    while (cycle_[index] != end) {
        --index;
    }
    for (; index < cycle_.size() - 1; index++) {
        std::cout << cycle_[index] + 1 << " ";
    }
}

void Graph::InsertEdgeOrient(int ver_first, int ver_second) {
    adj_[ver_first].push_back(ver_second);
}

void Graph::Dfs(int ver) {
    if (is_cycle_) {
        return;
    }
    colors_[ver] = GREY;
    cycle_.push_back(ver);
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            Dfs(u);
        } else if (colors_[u] == GREY) {
            is_cycle_ = true;
            cycle_.push_back(u);
            return;
        }
        if (is_cycle_) {
            return;
        }
    }
    colors_[ver] = BLACK;
    cycle_.pop_back();
}

bool Graph::HasCycle() {
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            Dfs(v);
            if (is_cycle_) {
                break;;
            }
        }
    }
    return is_cycle_;
}


int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int ver_first, ver_second;
        std::cin >> ver_first >> ver_second;
        graph.InsertEdgeOrient(--ver_first, --ver_second);
    }
    if (graph.HasCycle()) {
        std::cout << "YES\n";
        graph.PrintCycle();
    } else {
        std::cout << "NO\n";
    }
    return 0;
}
