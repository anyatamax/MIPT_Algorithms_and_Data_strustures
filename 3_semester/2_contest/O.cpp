

// Даны N точек, занумерованных числами . От каждой точки с меньшим номером к каждой точке с большим номером ведет стрелка красного или синего цвета. Раскраска стрелок называется однотонной, если нет двух таких точек A и B, что от A до B можно добраться как только по красным стрелкам, так и только по синим.
// Ваша задача — по заданной раскраске определить, является ли она однотонной.

// Формат ввода
// В первой строке входных данных содержится единственное число N (3 ≤ N ≤ 5000).
// В следующих N - 1 строках идет описание раскраски. В (i+1)-й строке записано (N - i) символов R (красный) или B (синий), соответствующих цвету стрелок, выходящих из точки i и входящих в точки (i+1), (i+2), …, N соответственно.
// Формат вывода
// Выведите YES, если приведенная раскраска является однотонной, и NO в противном случае. 


#include <iostream>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), colors_(size, WHITE) {};
    void InsertEdgeOrient(int ver_first, int ver_second);
    void Dfs(int v);
    bool HasCycle();

private:
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    bool is_cycle_ = false;
};

void Graph::InsertEdgeOrient(int ver_first, int ver_second) {
    adj_[ver_first].push_back(ver_second);
}

void Graph::Dfs(int ver) {
    if (is_cycle_) {
        return;
    }
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            Dfs(u);
        } else if (colors_[u] == GREY) {
            is_cycle_ = true;
            return;
        }
        if (is_cycle_) {
            return;
        }
    }
    colors_[ver] = BLACK;
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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    std::cin >> n;
    Graph graph(n);
    for (int i = 0; i < n - 1; ++i) {
        std::string color_edges;
        std::cin >> color_edges;
        for (int j = 0; j < color_edges.size(); ++j) {
            if (color_edges[j] == 'R') {
                graph.InsertEdgeOrient(i + j + 1, i);
            } else {
                graph.InsertEdgeOrient(i, i + 1 + j);
            }
        }
    }
    if (graph.HasCycle()) {
        std::cout << "NO\n";
    } else {
        std::cout << "YES\n";
    }
    return 0;
}
