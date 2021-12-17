

// Группа Pink Floyd собирается дать новый концертный тур по всему миру. По предыдущему опыту группа знает, что солист Роджер Уотерс постоянно нервничает при перелетах. На некоторых маршрутах он теряет вес от волнения, а на других — много ест и набирает вес.
// Известно, что чем больше весит Роджер, тем лучше выступает группа, поэтому требуется спланировать перелеты так, чтобы вес Роджера на каждом концерте был максимально возможным. Группа должна посещать города в том же порядке, в котором она дает концерты. При этом между концертами группа может посещать промежуточные города.

// Формат ввода
// Первая строка входного файла содержит три натуральных числа n, m и k — количество городов в мире, количество рейсов и количество концертов, которые должна дать группа соответственно (n ≤ 100, m ≤ 10^4, 2 ≤ k ≤ 10^4). Города пронумерованы числами от 1 до n.
// Следующие m строк содержат описание рейсов, по одному на строке. Рейс номер i описывается тремя числами bi, ei и wi — номер начального и конечного города рейса и предполагаемое изменение веса Роджера в миллиграммах (1 ≤ bi, ei ≤ n, −10^5 ≤ wi ≤ 10^5).
// Последняя строка содержит числа a1, a2, …, ak — номера городов, в которых проводятся концерты. В начале концертного тура группа находится в городе a1.
// Гарантируется, что группа может дать все концерты.
// Формат вывода
// Первая строка выходного файла должна содержать число s — количество рейсов, которые должна сделать группа. Вторая строка должна содержать s чисел — номера используемых рейсов. Если существует такая последовательность маршрутов между концертами, что Роджер будет набирать вес неограниченно, то первая строка выходного файла должна содержать строку infinitely kind.


#include <algorithm>
#include <iostream>
#include <vector>

class Graph {
public:
    explicit Graph(int size)
        : graph_(size, std::vector<int>(size))
        , dist_(size, std::vector<int64_t>(size, max_dist_))
        , parent_(size, std::vector<std::pair<int, int>>(size, std::make_pair(-1, -1)))
        , size_(size){};
    void InsertOrientEdge(int from, int to, int64_t len, int index);
    void FloydWarshall();
    bool FindPath(int start_ver, int end_ver);
    void PrintPath() const;

private:
    const int64_t max_dist_ = std::numeric_limits<int64_t>::max();
    std::vector<std::vector<int>> graph_;
    std::vector<std::vector<int64_t>> dist_;
    std::vector<std::vector<std::pair<int, int>>> parent_;
    std::vector<std::vector<int>> path_;
    int64_t count_flight_ = 0;
    int size_;
};

void Graph::InsertOrientEdge(int from, int to, int64_t len, int index) {
    graph_[from][to] = len;
    if (dist_[from][to] > len) {
        dist_[from][to] = len;
        parent_[from][to] = std::make_pair(from, index);
    }
}

void Graph::FloydWarshall() {
    for (int i = 0; i < size_; ++i) {
        dist_[i][i] = 0;
    }
    for (int k = 0; k < size_; k++) {
        for (int i = 0; i < size_; i++) {
            for (int j = 0; j < size_; j++) {
                if (dist_[i][k] < max_dist_ && dist_[k][j] < max_dist_ && dist_[i][k] + dist_[k][j] < dist_[i][j]) {
                    dist_[i][j] = dist_[i][k] + dist_[k][j];
                    parent_[i][j] = parent_[k][j];
                }
            }
        }
    }
}

bool Graph::FindPath(int start_ver, int end_ver) {
    if (start_ver == end_ver && dist_[start_ver][start_ver] >= 0) {
        return true;
    }
    if (dist_[start_ver][start_ver] < 0 || dist_[end_ver][end_ver] < 0) {
        return false;
    }
    std::vector<int> path_this;
    int ver = parent_[start_ver][end_ver].first;
    int ver_edge = parent_[start_ver][end_ver].second;
    while (ver != start_ver) {
        if (ver == -1) {
            return false;
        }
        if (dist_[ver][ver] < 0) {
            return false;
        }
        if (ver_edge != -1) {
            path_this.push_back(ver_edge);
        }
        ver_edge = parent_[start_ver][ver].second;
        ver = parent_[start_ver][ver].first;
    }
    path_this.push_back(ver_edge);
    std::reverse(path_this.begin(), path_this.end());
    count_flight_ += path_this.size();
    path_.push_back(path_this);
    return true;
}

void Graph::PrintPath() const {
    std::cout << count_flight_ << "\n";
    for (int64_t i = 0; i < static_cast<int64_t>(path_.size()); ++i) {
        for (int64_t j = 0; j < static_cast<int64_t>(path_[i].size()); ++j) {
            std::cout << path_[i][j] << " ";
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    int k = 0;
    std::cin >> n >> m >> k;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int weight = 0;
        std::cin >> ver_first >> ver_second >> weight;
        graph.InsertOrientEdge(--ver_first, --ver_second, -weight, i + 1);
    }
    std::vector<int> concerts(k);
    for (int i = 0; i < k; ++i) {
        std::cin >> concerts[i];
        --concerts[i];
    }
    int start_ver = concerts[0];
    int end_ver = 0;
    std::vector<std::vector<int>> path;
    graph.FloydWarshall();
    for (int i = 1; i < k; ++i) {
        end_ver = concerts[i];
        if (!graph.FindPath(start_ver, end_ver)) {
            std::cout << "infinitely kind";
            return 0;
        }
        start_ver = end_ver;
    }
    graph.PrintPath();
    return 0;
}
