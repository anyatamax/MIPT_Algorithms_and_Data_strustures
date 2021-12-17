

// Чтобы поднять в свой офис на N-м этаже небоскреба новый сейф, Вите опять пришлось прибегнуть к помощи грузчиков. Но за это время система оплаты изменилась. Теперь за подъем по лестнице на один этаж требуется заплатить U рублей, за спуск по лестнице на один этаж — D рублей, за внос в лифт — I рублей, за вынос из лифта — J рублей.
// В офисе имеется L лифтов, каждый из которых останавливается лишь на определенных этажах.
// Помогите Вите разработать маршрут подъема сейфа с первого этажа, стоимость которого наименьшая.

// Формат ввода
// В первой строке входного файла записаны целые числа N, U, D, I, J, L.
// Каждая из следующих L строк описывает соответствующий лифт. Она начинается с числа Ki — количества этажей, на которых останавливается i-й лифт, за которым следует Ki натуральных чисел — этажи, на которых останавливается этот лифт (этажи для каждого лифта задаются в возрастающем порядке).
// Ограничения: 0 ≤ U ≤ 10^3, 0 ≤ D ≤ 10^3, 0 ≤ I ≤ 10^3, 0 ≤ J ≤ 10^3, 0 ≤ L ≤ 500, 1 ≤ N ≤ 10^6, 2 ≤ Ki ≤ 10^3, K1 + K2 + … +KL ≤ 10^5.
// Количество этажей в небоскребе не превосходит 106.
// Формат вывода
// В выходной файл выведите одно число — минимальную стоимость подъема сейфа.


#include <algorithm>
#include <unordered_map>
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
    for (auto& i : heap_) {
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
    explicit Graph(int size) : adj_(size), dist_(size, max_distance_), size_(size){};
    void InsertOrientEdge(int ver_first, int ver_second, int weight);
    void Dijkstra(int start_ver);
    int Result(int floor);

private:
    const int max_distance_ = std::numeric_limits<int>::max();
    std::vector<std::unordered_map<int, int>> adj_;
    std::vector<int> dist_;
    int size_;
};

void Graph::InsertOrientEdge(int ver_first, int ver_second, int weight) {
    if (adj_[ver_first].find(ver_second) != adj_[ver_first].end()) {
        adj_[ver_first][ver_second] = std::min(adj_[ver_first][ver_second], weight);
    } else {
        adj_[ver_first][ver_second] = weight;
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
        for (auto& u : adj_[ver.second]) {
            if (!used[u.first] && dist_[u.first] > dist_[ver.second] + u.second) {
                dist_[u.first] = dist_[ver.second] + u.second;
                h.Insert(std::make_pair(dist_[u.first], u.first));
            }
        }
    }
}

int Graph::Result(int floor) {
    Dijkstra(1);
    return dist_[floor];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int lift_up = 0;
    int lift_down = 0;
    int in_lift = 0;
    int out_lift = 0;
    int count_lifts = 0;
    std::cin >> n >> lift_up >> lift_down >> in_lift >> out_lift >> count_lifts;
    std::vector<std::vector<int>> lifts(count_lifts);
    int max_number_ver = n;
    for (int i = 0; i < count_lifts; ++i) {
        int floors = 0;
        std::cin >> floors;
        lifts[i].resize(floors);
        for (int j = 0; j < floors; ++j) {
            std::cin >> lifts[i][j];
            if (lifts[i][j] > max_number_ver) {
                max_number_ver = lifts[i][j];
            }
        }
    }
    Graph g(max_number_ver + 1 + count_lifts);
    for (int ver = 1; ver <= max_number_ver; ++ver) {
        if (ver == 1 && ver != max_number_ver) {
            g.InsertOrientEdge(ver, ver + 1, lift_up);
        } else if (ver != 1 && ver == max_number_ver) {
            g.InsertOrientEdge(ver, ver - 1, lift_down);
        } else if (ver != 1 && ver != max_number_ver) {
            g.InsertOrientEdge(ver, ver + 1, lift_up);
            g.InsertOrientEdge(ver, ver - 1, lift_down);
        }
    }
    for (int i = 0; i < count_lifts; ++i) {
        for (int j = 0; j < static_cast<int>(lifts[i].size()); ++j) {
            g.InsertOrientEdge(lifts[i][j], max_number_ver + 1 + i, in_lift);
            g.InsertOrientEdge(max_number_ver + 1 + i, lifts[i][j], out_lift);
        }
    }
    std::cout << g.Result(n);
    return 0;
}
