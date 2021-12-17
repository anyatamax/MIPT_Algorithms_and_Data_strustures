

// Абитуриент Василий решил подать документы в ФПМИ. Так как Василий живёт достаточно далеко, он решил спланировать своё путешествие заранее.
// В результате долгих размышлений он пришёл к выводу, что на дорогу у него есть не более T минут, так как приёмная комиссия скоро закроется. Бюджет будущего студента также ограничен, поэтому он хочет ещё и потратить на своё путешествие поменьше денег.
// После ещё более долгих размышлений Василий составил карту из всех возможных путей с промежуточными пунктами (станции общественного транспорта или другие значимые для него объекты, до которых он гарантированно может добраться из дома). Карта представляет собой список из N пунктов, где дом Василия — это пункт с номером 1, а N -й пункт — приёмная комиссия. Между пунктами существует M переходов. Для каждого из них он написал стоимость Pi и время Ti. При этом гарантируется, что он всегда может добраться до приёмной комиссии.
// Между любыми двумя пунктами в карте Василия имеется не более одного прямого перехода. Некоторые из переходов могут быть бесплатными или занимать пренебрежительно малое время. При этом любой переход двусторонний, то есть стоимость перехода и время не зависят от того, движется ли Василий из пункта с номером A в пункт с номером B, или в обратном направлении.
// Помогите Василию найти минимальную стоимость маршрута, благодаря которому он сможет успеть подать документы и на который он потратит меньше всего денег.

// Формат ввода
// В первой строке даны три числа: N — число пунктов (1 ≤ N ≤ 10^3), M — число переходов (1 ≤ M ≤ 5 ⋅ 10^4), T — максимальное время (0 ≤ T ≤ 10^3).
// Далее идут M строк, содержащих по четыре целых неотрицательных числа — описание перехода:

//     Ai — номер пункта, откуда идёт переход;
//     Bi — номер пункта, куда приведёт переход (1 ≤ Ai, Bi ≤ N; );
//     Pi — стоимость перехода (0 ≤ Pi ≤ 10^6);
//     Ti — продолжительность перехода (0 ≤ Ti ≤ 10^3).

// Формат вывода
// Если добраться за отведённое время до приёмной комиссии невозможно, выведите единственное число −1.
// В противном случае, в первой строке выведите минимальное количество денег, требуемое для того, чтобы Василий успел подать документы.
// Во второй строке выведите L — число переходов, по которым Василию нужно пройти, чтобы успеть.
// В третьей строке выведите L чисел, которые задают порядок пунктов, по которому должен пройти Василий, чтобы успеть и потратить меньше всего денег.


#include <algorithm>
#include <iostream>
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
    int cost;
    int time;
};

class Graph {
public:
    explicit Graph(int size, int time)
        : size_graph_(size)
        , max_time_(time)
        , graph_(size)
        , parent_(time + 1, std::vector<std::pair<int, int>>(size, std::make_pair(-1, -1)))
        , dp_(time + 1, std::vector<int64_t>(size, max_dist_)){};
    void InsertOrientEdge(int ver_first, int ver_second, int w, int t);
    void Dijkstra(int time);
    void GetAnswer();

private:
    int size_graph_;
    int max_time_;
    const int64_t max_dist_ = std::numeric_limits<int64_t>::max();
    std::vector<std::vector<Edge>> graph_;
    std::vector<std::vector<std::pair<int, int>>> parent_;
    std::vector<std::vector<int64_t>> dp_;
};

void Graph::GetAnswer() {
    dp_[0][0] = 0;
    for (int t = 0; t <= max_time_; ++t) {
        Dijkstra(t);
        for (int i = 0; i < size_graph_; ++i) {
            for (auto &edge : graph_[i]) {
                if (t + edge.time <= max_time_ && dp_[t][edge.from] != max_dist_ &&
                    dp_[t][edge.from] + edge.cost < dp_[t + edge.time][edge.to]) {
                    dp_[t + edge.time][edge.to] = dp_[t][edge.from] + edge.cost;
                    parent_[t + edge.time][edge.to].first = edge.from;
                    parent_[t + edge.time][edge.to].second = edge.time;
                }
            }
        }
    }
    int64_t min = max_dist_;
    int min_time = max_dist_;
    for (size_t i = 0; i < dp_.size(); ++i) {
        if (dp_[i][size_graph_ - 1] < min) {
            min = dp_[i][size_graph_ - 1];
            min_time = i;
        }
    }
    if (min == max_dist_) {
        std::cout << -1;
        return;
    }
    std::vector<int> path;
    int time_cur = min_time;
    int cur = size_graph_ - 1;
    while (cur != -1 && time_cur >= 0) {
        path.push_back(cur);
        int ver = cur;
        cur = parent_[time_cur][cur].first;
        time_cur -= parent_[time_cur][ver].second;
    }
    std::reverse(path.begin(), path.end());
    std::cout << min << "\n" << path.size() << "\n";
    for (auto &i : path) {
        std::cout << i + 1 << " ";
    }
}

void Graph::InsertOrientEdge(int ver_first, int ver_second, int w, int t) {
    if (ver_first != ver_second) {
        graph_[ver_first].push_back({ver_first, ver_second, w, t});
        graph_[ver_second].push_back({ver_second, ver_first, w, t});
    }
}

void Graph::Dijkstra(int time) {
    std::vector<bool> used(size_graph_, false);
    Heap h;
    for (int i = 0; i < size_graph_; ++i) {
        if (dp_[time][i] != max_dist_) {
            h.Insert(std::make_pair(dp_[time][i], i));
        }
    }
    while (!h.Empty()) {
        std::pair<int64_t, int> ver = h.ExtractMin();
        if (dp_[time][ver.second] != ver.first) {
            continue;
        }
        used[ver.second] = true;
        for (auto &u : graph_[ver.second]) {
            if (u.time == 0) {
                if (dp_[time][ver.second] != max_dist_ && !used[u.to] &&
                    dp_[time][u.to] > dp_[time][ver.second] + u.cost) {
                    dp_[time][u.to] = dp_[time][ver.second] + u.cost;
                    parent_[time][u.to].first = ver.second;
                    parent_[time][u.to].second = 0;
                    h.Insert(std::make_pair(dp_[time][u.to], u.to));
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    int max_time = 0;
    std::cin >> n >> m >> max_time;
    Graph g(n, max_time);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int cost = 0;
        int time = 0;
        std::cin >> ver_first >> ver_second >> cost >> time;
        g.InsertOrientEdge(--ver_first, --ver_second, cost, time);
    }
    g.GetAnswer();
    return 0;
}
