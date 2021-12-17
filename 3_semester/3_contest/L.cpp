

// Напишите программу, которая будет находить расстояния в неориентированном взвешенном графе с неотрицательными длинами ребер, от указанной вершины до всех остальных. Программа должна работать быстро для больших разреженных графов.

// Формат ввода
// В первой строке входных данных задано число NUM — количество различных запусков алгоритма Дейкстры (на разных графах). Далее следуют NUM блоков, каждый из которых имеет следующую структуру.
// Первая строка блока содержит два числа N и M, разделенные пробелом — количество вершин и количество ребер графа. Далее следуют M строк, каждая из которых содержит по три целых числа, разделенные пробелами. Первые два из них в пределах от 0 до N - 1 каждое и обозначают концы соответствующего ребра, третье — в пределах от 0 до 20000 и обозначает длину этого ребра. Далее, в последней строке блока, записанное единственное число от 0 до N - 1 — вершина, расстояния от которой надо искать.
// Количество различных графов в одном тесте NUM не превышает 5. Количество вершин не превышает 60000, рёбер — 200000.
// Формат вывода
// Выведите в стандартный вывод NUM строк, в каждой из которых по Ni чисел, разделенных пробелами — расстояния от указанной начальной вершины взвешенного неориентированного графа до его 0-й, 1-й, 2-й и т. д. вершин (допускается лишний пробел после последнего числа). Если некоторая вершина недостижима от указанной начальной, вместо расстояния выводите число 2009000999 (гарантировано, что все реальные расстояния меньше).



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
    ShiftUp(heap_.size() - 1);
}

std::pair<int, int> Heap::ExtractMin() {
    --size_;
    std::pair<int, int> min = heap_[0];
    heap_[0] = heap_.back();
    heap_.pop_back();
    ShiftDown(0);
    return min;
}

class Graph {
public:
    explicit Graph(int size) : adj_(size), dist_(size, max_distance_), prev_(size, -1){};
    std::pair<int, int> ReturnEdges(int ver_first, int ver_second) const;
    void InsertNoOrientEdge(int ver_first, int ver_second, int weight);
    void Dijkstra(int start_ver);
    void PrintDistance(int start_ver);

private:
    const int max_distance_ = 2009000999;
    std::vector<std::vector<int>> adj_;
    std::vector<int> dist_;
    std::vector<int> prev_;
    std::map<std::pair<int, int>, int> weights_;
};

void Graph::InsertNoOrientEdge(int ver_first, int ver_second, int weight) {
    if (weights_.find(ReturnEdges(ver_first, ver_second)) == weights_.end()) {
        weights_[ReturnEdges(ver_first, ver_second)] = weight;
        adj_[ver_first].push_back(ver_second);
        adj_[ver_second].push_back(ver_first);
    } else {
        weights_[ReturnEdges(ver_first, ver_second)] = std::min(weights_[ReturnEdges(ver_first, ver_second)], weight);
    }
}
void Graph::Dijkstra(int start_ver) {
    dist_[start_ver] = 0;
    std::vector<bool> used(dist_.size(), false);
    Heap h;
    for (int64_t i = 0; i < static_cast<int64_t>(adj_.size()); ++i) {
        h.Insert(std::make_pair(dist_[i], i));
    }
    while (!h.Empty()) {
        std::pair<int, int> ver = h.ExtractMin();
        if (dist_[ver.second] != ver.first) {
            continue;
        }
        used[ver.second] = true;
        for (auto &u : adj_[ver.second]) {
            if (!used[u] && dist_[u] > dist_[ver.second] + weights_[ReturnEdges(ver.second, u)]) {
                prev_[u] = ver.second;
                dist_[u] = dist_[ver.second] + weights_[ReturnEdges(ver.second, u)];
                h.Insert(std::make_pair(dist_[u], u));
            }
        }
    }
}

std::pair<int, int> Graph::ReturnEdges(int ver_first, int ver_second) const {
    if (ver_first > ver_second) {
        return std::make_pair(ver_second, ver_first);
    }
    return std::make_pair(ver_first, ver_second);
}

void Graph::PrintDistance(int start_ver) {
    Dijkstra(start_ver);
    for (int i : dist_) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

int main() {
    int num = 0;
    std::cin >> num;
    for (int i = 0; i < num; ++i) {
        int n = 0;
        int m = 0;
        std::cin >> n >> m;
        Graph g(n);
        for (int j = 0; j < m; ++j) {
            int ver_first = 0;
            int ver_second = 0;
            int len = 0;
            std::cin >> ver_first >> ver_second >> len;
            g.InsertNoOrientEdge(ver_first, ver_second, len);
        }
        int start_ver = 0;
        std::cin >> start_ver;
        g.PrintDistance(start_ver);
    }
    return 0;
}
