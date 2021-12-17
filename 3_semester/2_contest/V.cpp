

// В городе, построенном во времена средневековья, ширина улиц стала препятствовать движению транспорта, которое изначально было двусторонним по каждой из улиц. Для решения этой проблемы было предложено сделать движение по каждой из улиц односторонним. Мэр поручил эту задачу своему первому заму. После долгих размышлений тот доложил, что на некоторых улицах движение придется оставить двусторонним, в противном случае будет невозможно проехать из любого места в городе в любое другое. По данной схеме города требуется найти все такие улицы.

// Формат ввода
// В первой строке входного файла находятся числа N — количество площадей в городе и М — количество улиц их соединяющих (1 ≤ N ≤ 20000, 1 ≤ M ≤ 200000). Площади имеют номера от 1 до N. В каждой из следующих M строк находится пара натуральных чисел, описывающая между какими двумя площадями проходит соответствующая улица (две площади соединяются не более чем одной улицей).
// Формат вывода
// На первой строке выведите число B — количество улиц, на которых организовать одностороннее движение невозможно. На следующей строке выведите B целых чисел — номера этих улиц в возрастающем порядке. Улицы нумеруются с единицы в том порядке, в котором они заданы во входном файле.


#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), colors_(size, WHITE), time_up(size, MAX_TIME), time_in(size, MAX_TIME) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second, int index);
    void Dfs(int v, int parent);
    std::set<int> Bridges();
    std::pair<int, int> ReturnEdges(int ver_first, int ver_second) const;

private:
    const long int MAX_TIME = 5000000000;
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::set<int> bridges_;
    std::vector<long int> time_up;
    std::vector<long int> time_in;
    std::map<std::pair<int, int>, int> number_edges_;
    long long int time_ = 1;
};

std::pair<int, int> Graph::ReturnEdges(int ver_first, int ver_second) const {
    if (ver_first > ver_second) {
        return std::make_pair(ver_second, ver_first);
    }
    return std::make_pair(ver_first, ver_second);
}

void Graph::InsertEdgeNoOrient(int ver_first, int ver_second, int index) {
    if (std::find(adj_[ver_first].begin(), adj_[ver_first].end(), ver_second) == adj_[ver_first].end()) {
        adj_[ver_first].push_back(ver_second);
    }
    if (std::find(adj_[ver_second].begin(), adj_[ver_second].end(), ver_first) == adj_[ver_second].end()) {
        adj_[ver_second].push_back(ver_first);
    }
    if (number_edges_.find(ReturnEdges(ver_first, ver_second)) == number_edges_.end()) {
        number_edges_[ReturnEdges(ver_first, ver_second)] = index;
    }
}

void Graph::Dfs(int ver, int parent) {
    time_in[ver] = time_;
    time_up[ver] = time_;
    ++time_;
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (u == parent) {
            continue;
        } else if (colors_[u] == GREY) {
            time_up[ver] = std::min(time_up[ver], time_in[u]);
        } else if (colors_[u] == WHITE) {
            Dfs(u, ver);
            time_up[ver] = std::min(time_up[u], time_up[ver]);
            if (time_in[ver] < time_up[u]) {
                bridges_.insert(number_edges_[ReturnEdges(ver, u)]);
            }
        }
    }
    colors_[ver] = BLACK;
}

std::set<int> Graph::Bridges() {
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            Dfs(v, -1);
        }
    }
    return bridges_;
}


int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        std::cin >> ver_first >> ver_second;
        g.InsertEdgeNoOrient(--ver_first, --ver_second, i);
    }
    std::set<int> result = g.Bridges();
    std::cout << result.size() << "\n";
    for (auto & i : result) {
        std::cout << i + 1 << "\n";
    }
    return 0;
}
