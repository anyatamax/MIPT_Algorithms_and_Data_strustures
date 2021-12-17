

// Вы — глава агенства, в котором работает N сотрудников — шпионы с кодовыми номерами от 1 до N. Шпионы были посланы в различные страны и получили кусочки важной информации. Ваша задача состоит в следующем:
//     Организовать встречи между некоторыми шпионами. На каждой такой встрече ровно два шпиона оказываются в одном месте и обмениваются всей информацией, которую получили сами или узнали от других шпионов на предыдущих встречах. Каждую из возможных встреч сложно и затратно организовать, поэтому вам известны все стоимости.
//     После того, как все встречи состоялись, выбирается группа шпионов, и они посылаются на задание по спасению мира. Для каждого шпиона известно, сколько денег он изведет, если его включить в эту группу. Задание должно быть выполнено, поэтому очень важно, чтобы выбранные шпионы все вместе обладали всей информацией, полученной агенством.
// Найдите минимальную цену, за которую можно спасти мир.

// Формат ввода
// Первая строка содержит натуральное число N — количество шпионов (2 ≤ N ≤ 1000). Каждая из следующих строк содержит N натуральных чисел, не превосходящих 106. Число в k-й строке и m-м столбце обозначает стоимость встречи между шпионами k и m и совпадает с числом в m-й строке и k-м столбце (если k = m, то соответствующее число будет равно 0). Следующая строка содержит n чисел, i-е из них равно стоимости послать i-го шпиона на задание. Все стоимости являются положительными целыми числами.
// Формат вывода
// Выведите одно число — минимальную стоимость спасения мира.


#include <algorithm>
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

class Graph {
public:
    explicit Graph(int size) : adj_(size), dist_(size, max_distance_), prev_(size, -1){};
    std::pair<int, int> ReturnEdges(int ver_first, int ver_second) const;
    void InsertNoOrientEdge(int ver_first, int ver_second, int weight);
    void InsertOrientEdge(int ver_first, int ver_second, int weight);
    int64_t Prima();

private:
    const int max_distance_ = 100000000;
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

void Graph::InsertOrientEdge(int ver_first, int ver_second, int weight) {
    adj_[ver_first].push_back(ver_second);
    weights_[ReturnEdges(ver_first, ver_second)] = weight;
}

int64_t Graph::Prima() {
    std::set<std::pair<int, int>> weight_mst;
    dist_[adj_.size() - 1] = 0;
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
        if (prev_[ver.second] != -1) {
            weight_mst.insert(ReturnEdges(prev_[ver.second], ver.second));
        }
        used[ver.second] = true;
        for (auto &u : adj_[ver.second]) {
            if (!used[u] && dist_[u] > weights_[ReturnEdges(ver.second, u)]) {
                prev_[u] = ver.second;
                dist_[u] = weights_[ReturnEdges(ver.second, u)];
                h.Insert(std::make_pair(dist_[u], u));
            }
        }
    }
    int64_t weight_sum = 0;
    for (auto &i : weight_mst) {
        weight_sum += weights_[i];
    }
    return weight_sum;
}

std::pair<int, int> Graph::ReturnEdges(int ver_first, int ver_second) const {
    if (ver_first > ver_second) {
        return std::make_pair(ver_second, ver_first);
    }
    return std::make_pair(ver_first, ver_second);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<std::vector<int>> matr(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> matr[i][j];
        }
    }
    std::vector<int> costs(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> costs[i];
    }
    Graph g(n + 1);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            g.InsertNoOrientEdge(i, j, matr[i][j]);
        }
    }
    for (int i = 0; i < n; ++i) {
        g.InsertOrientEdge(n, i, costs[i]);
    }
    std::cout << g.Prima();
    return 0;
}
