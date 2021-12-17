

// Урбанист Варлам Ильинов каждый день катается на своем велосипеде по городу, оценивая качество велосипедных дорог, а также доступность его любимых кофеен. За годы своих трудов он смог составить подробную карту каждого района и выяснить за какое время он сможет добраться от одной кофейни до другой. Кроме того, если от одной кофейни до другой есть трамвайная линия, то он впадает в неистовый восторг и считает, что время на этом пути не тратится, а наоборот приобретается.
// Теперь он хочет измерить удобность транспортной инфраструктуры. Чем меньше максимальное расстояние между кофейнями, тем более удобным считается город. При этом, если от одной кофейни нельзя добраться до другой ни по велосипедным дорожкам, ни с помощью трамвая, то такая (упорядоченная) пара игнорируется при подсчете удобности.
// Необходимо найти пару кофеен, кратчайшее расстояние от одной из которых до другой максимально среди всех пар, между которыми есть какой-либо путь.

// Формат ввода
// В первой строке вводятся числа N и M (1 ≤ N ≤ 10^3, 1 ≤ M ≤ 5 ⋅ N) — количество кофеен и общее количество велосипедных дорожек и трамвайных путей. В следующих M строках задаются числа a, b, w, что означает наличие пути от кофейни a до кофейни b длины w ∈ [-100, 100] (но не наоборот!). Кофейни нумеруются с 0. Кольцевых трамвайных линий в городе не предусмотрено.
// Формат вывода
// Выведите искомое максимальное конечное кратчайшее расстояние между кофейнями.


#include <iostream>
#include <map>
#include <set>
#include <vector>

class Heap {
public:
    void Insert(std::pair<int, int> new_ver);
    void ShiftUp(int index);
    void ShiftDown(int index);
    std::pair<int, int> ExtractMin();
    bool Empty() const;
    bool Find(int ver, int dist) const;

private:
    std::vector<std::pair<int, int>> heap_;  //<dist[ver], ver>
    int size_ = 0;
};

bool Heap::Empty() const {
    return size_ == 0;
}

bool Heap::Find(int ver, int dist) const {
    for (auto &i : heap_) {
        if (i.second == ver && i.first == dist) {
            return true;
        }
    }
    return false;
}

void Heap::ShiftUp(int index) {
    while ((index - 1) / 2 >= 0 && index > 0 && heap_[index].first < heap_[(index - 1) / 2].first) {
        std::swap(heap_[index], heap_[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void Heap::ShiftDown(int index) {
    while (static_cast<int64_t>(2 * index + 1) < static_cast<int64_t>(heap_.size())) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int j = left;
        if (static_cast<int64_t>(right) < static_cast<int64_t>(heap_.size()) &&
            heap_[right].first < heap_[left].first) {
            j = right;
        }
        if (heap_[index].first <= heap_[j].first) {
            break;
        }
        std::swap(heap_[index], heap_[j]);
        index = j;
    }
}

void Heap::Insert(std::pair<int, int> new_ver) {
    ++size_;
    heap_.push_back(new_ver);
    ShiftUp(size_ - 1);
}

std::pair<int, int> Heap::ExtractMin() {
    --size_;
    std::pair<int, int> min = heap_[0];
    heap_[0] = heap_.back();
    heap_.pop_back();
    ShiftDown(0);
    return min;
}

struct Edge {
    int from;
    int to;
    int weight;
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), dist_(size, max_distance_), function_(size), size_(size){};
    void InsertOrientEdge(int ver_first, int ver_second, int weight);
    void Dijkstra(int start_ver);
    void FordBellman(int start_ver);
    std::vector<std::vector<int>> Johnson();
    int Result();

private:
    const int max_distance_ = std::numeric_limits<int>::max();
    std::vector<std::vector<Edge>> adj_;
    std::vector<int> dist_;
    std::vector<int> function_;
    std::vector<std::vector<Edge>> adj_new_;
    int size_;
};

void Graph::InsertOrientEdge(int ver_first, int ver_second, int weight) {
    adj_[ver_first].push_back({ver_first, ver_second, weight});
}

std::vector<std::vector<int>> Graph::Johnson() {
    for (int i = 1; i < size_; ++i) {
        adj_[0].push_back({0, i, 0});
    }
    adj_new_ = adj_;
    FordBellman(0);
    function_ = dist_;
    for (int v = 0; v < size_; ++v) {
        for (auto &edge : adj_new_[v]) {
            edge.weight = edge.weight + function_[edge.from] - function_[edge.to];
        }
    }
    std::vector<std::vector<int>> distance_result(size_ - 1, std::vector<int>(size_ - 1));
    for (int i = 1; i < size_; ++i) {
        Dijkstra(i);
        for (int j = 1; j < size_; ++j) {
            if (dist_[j] == max_distance_) {
                distance_result[i - 1][j - 1] = dist_[j];
            } else {
                distance_result[i - 1][j - 1] = dist_[j] + function_[j] - function_[i];
            }
        }
    }
    return distance_result;
}

void Graph::FordBellman(int start_ver) {
    dist_.assign(size_, max_distance_);
    dist_[start_ver] = 0;
    for (int ver = 0; ver < size_; ++ver) {
        for (int i = 0; i < size_; ++i) {
            for (auto &edge : adj_[i]) {
                if (dist_[edge.from] < max_distance_) {
                    if (dist_[edge.to] > dist_[edge.from] + edge.weight) {
                        dist_[edge.to] = dist_[edge.from] + edge.weight;
                    }
                }
            }
        }
    }
}

void Graph::Dijkstra(int start_ver) {
    dist_.assign(size_, max_distance_);
    dist_[start_ver] = 0;
    std::vector<bool> used(dist_.size(), false);
    Heap h;
    h.Insert(std::make_pair(dist_[start_ver], start_ver));
    while (!h.Empty()) {
        std::pair<int, int> ver = h.ExtractMin();
        if (dist_[ver.second] != ver.first) {
            continue;
        }
        used[ver.second] = true;
        for (auto &u : adj_new_[ver.second]) {
            if (!used[u.to] && dist_[u.to] > dist_[ver.second] + u.weight) {
                dist_[u.to] = dist_[ver.second] + u.weight;
                h.Insert(std::make_pair(dist_[u.to], u.to));
            }
        }
    }
}

int Graph::Result() {
    std::vector<std::vector<int>> result = Johnson();
    int max = -max_distance_;
    for (int i = 0; i < size_ - 1; ++i) {
        for (int j = 0; j < size_ - 1; ++j) {
            if (result[i][j] != max_distance_ && result[i][j] > max && i != j) {
                max = result[i][j];
            }
        }
    }
    return max;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph g(n + 1);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int weight = 0;
        std::cin >> ver_first >> ver_second >> weight;
        g.InsertOrientEdge(++ver_first, ++ver_second, weight);
    }
    std::cout << g.Result();
    return 0;
}
