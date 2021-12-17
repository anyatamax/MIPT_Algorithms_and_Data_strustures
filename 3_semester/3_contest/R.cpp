

// В одной известной игре нужно смоделировать распространение вируса по земному шару. Студенты ФПМИ разработали альтернативную версию, где вы играете за человечество, а за вирус играет ИИ.
// Игру нужно уметь дебажить в ходе разработки, поэтому вам поручили написание генератора тестов и решения для частного случая конфигурации игры. Генератор тестов вы уже написали, теперь вам предстоит подумать над решением.
// В общем случае игра слишком сложна: имеется множество стран, между которыми есть транспортные линии. Вирус распространяется по достаточно сложному закону с элементами случайности, что мешает процессу отладки. Поэтому в данной конфигурации вирус распространяется с постоянной скоростью во всех направлениях.
// Изначальная конфигурация задана номером страны, где находятся люди, за которых вы играете, количеством уже зараженных стран и длинами транспортных линий между ними (вирус распространяется только вдоль них), скорость вируса равна человеческой скорости. Будем считать, что по транспортным линиям человечество перемещается со скоростью один игровой метр в единицу времени.
// Таким образом, транспортная линия длиной k игровых метров, смежная с уже зараженной страной, окажется заражена через k единиц времени, при этом страна, в которую ведет эта линия, тут же заражена. Также вам задана страна, где находится космопорт. Туда стремятся люди, чтобы покинуть зараженную планету.
// Перемещаться по транспортным линиям можно в обе стороны. Если люди прибывают в страну одновременно с ее заражением, то вы проиграли (это верно и для той страны, где расположен космопорт). Гарантируется, что люди начинают с здоровой страны.

// Формат ввода
// В первой строке входных данных содержится три целых числа N, M и K — количество стран, транспортных линий и уже зараженных стран соответственно (2 ≤ N ≤ 2 ⋅ 10^5, 0 ≤ M ≤ 2 ⋅ 10^5, 1 ≤ K < N).
// Во второй строке содержится K различных чисел ai — номера зараженных стран в изначальной конфигурации (1 ≤ ai ≤ N).
// Следующие M строк описывают транспортные линии. Каждое описание состоит из трех чисел xi, yi и li, обозначающих номера стран, которые соединяет i-я транспортная линия, и ее длину в игровых метрах (1 ≤ xi, yi ≤ N, 1 ≤ li ≤ 10^9, xi ≠ yi).
// В последней строке входных данных содержится 2 числа S и F — номера стран, где игра начинается и где находится космопорт (1 ≤ S, F ≤ N).
// Формат вывода
// Выведите единственное число — минимальное количество игровых единиц времени, которое требуется людям для того, чтобы добраться до космопорта, либо «-1», если игра будет проиграна, как бы вы не старались играть за людей.


#include <iostream>
#include <map>
#include <set>
#include <vector>

class Heap {
public:
    void Insert(std::pair<int64_t, int> new_ver);
    void ShiftUp(int index);
    void ShiftDown(int index);
    std::pair<int64_t, int> ExtractMin();
    bool Empty() const;
    bool Find(int ver, int dist) const;

private:
    std::vector<std::pair<int64_t, int>> heap_;  //<dist[ver], ver>
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

void Heap::Insert(std::pair<int64_t, int> new_ver) {
    ++size_;
    heap_.push_back(new_ver);
    ShiftUp(size_ - 1);
}

std::pair<int64_t, int> Heap::ExtractMin() {
    --size_;
    std::pair<int64_t, int> min = heap_[0];
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
    explicit Graph(int size) : adj_(size), dist_infection_(size, max_distance_), dist_people_(size, max_distance_){};
    void InsertNoOrientEdge(int ver_first, int ver_second, int weight);
    void DijkstraInfection(std::vector<int> &infected);
    int64_t DijkstraPeople(int start_ver, int end);

private:
    const int64_t max_distance_ = std::numeric_limits<int64_t>::max();
    std::vector<std::vector<Edge>> adj_;
    std::vector<int64_t> dist_infection_;
    std::vector<int64_t> dist_people_;
};

void Graph::InsertNoOrientEdge(int ver_first, int ver_second, int weight) {
    adj_[ver_first].push_back({ver_first, ver_second, weight});
    adj_[ver_second].push_back({ver_second, ver_first, weight});
}
void Graph::DijkstraInfection(std::vector<int> &infected) {
    Heap h;
    for (int64_t i = 0; i < static_cast<int64_t>(infected.size()); ++i) {
        dist_infection_[infected[i]] = 0;
        h.Insert(std::make_pair(dist_infection_[infected[i]], infected[i]));
    }
    std::vector<bool> used(dist_infection_.size(), false);
    while (!h.Empty()) {
        std::pair<int64_t, int> ver = h.ExtractMin();
        if (dist_infection_[ver.second] != ver.first) {
            continue;
        }
        used[ver.second] = true;
        for (auto &u : adj_[ver.second]) {
            if (!used[u.to] && dist_infection_[u.to] > dist_infection_[ver.second] + u.weight) {
                dist_infection_[u.to] = dist_infection_[ver.second] + u.weight;
                h.Insert(std::make_pair(dist_infection_[u.to], u.to));
            }
        }
    }
}

int64_t Graph::DijkstraPeople(int start_ver, int end) {
    dist_people_[start_ver] = 0;
    std::vector<bool> used(dist_people_.size(), false);
    Heap h;
    h.Insert(std::make_pair(dist_people_[start_ver], start_ver));
    while (!h.Empty()) {
        std::pair<int64_t, int> ver = h.ExtractMin();
        if (dist_people_[ver.second] != ver.first) {
            continue;
        }
        used[ver.second] = true;
        for (auto &u : adj_[ver.second]) {
            if (!used[u.to] && dist_people_[u.to] > dist_people_[ver.second] + u.weight &&
                dist_people_[ver.second] + u.weight < dist_infection_[u.to]) {
                dist_people_[u.to] = dist_people_[ver.second] + u.weight;
                h.Insert(std::make_pair(dist_people_[u.to], u.to));
            }
        }
    }
    if (dist_people_[end] == max_distance_) {
        return -1;
    }
    return dist_people_[end];
}

int main() {
    int n = 0;
    int m = 0;
    int k = 0;
    std::cin >> n >> m >> k;
    std::vector<int> infected(k);
    for (int i = 0; i < k; ++i) {
        std::cin >> infected[i];
        --infected[i];
    }
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int len = 0;
        std::cin >> ver_first >> ver_second >> len;
        g.InsertNoOrientEdge(--ver_first, --ver_second, len);
    }
    int start = 0;
    int end = 0;
    std::cin >> start >> end;
    g.DijkstraInfection(infected);
    std::cout << g.DijkstraPeople(--start, --end);
    return 0;
}
