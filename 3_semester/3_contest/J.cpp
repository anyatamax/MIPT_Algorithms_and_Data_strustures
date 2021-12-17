

// Профессору Форду необходимо попасть на международную конференцию. Он хочет потратить на дорогу наименьшее количество денег, поэтому решил, что будет путешествовать исключительно ночными авиарейсами (чтобы не тратиться на ночевку в отелях), а днем будет осматривать достопримечательности тех городов, через которые он будет проезжать транзитом. Он внимательно изучил расписание авиаперелетов и составил набор подходящих авиарейсов, выяснив, что перелеты на выбранных направлениях совершаются каждую ночь и за одну ночь он не сможет совершить два перелета.
// Теперь профессор хочет найти путь наименьшей стоимости, учитывая что до конференции осталось K ночей (то есть профессор может совершить не более K перелетов).

// Формат ввода
// В первой строке находятся числа N (2 ≤ N ≤ 100) (количество городов), M (1 ≤ M ≤ 10^5) (количество авиарейсов), K (1 ≤ K ≤ 100) (количество оставшихся ночей), S (1 ≤ S ≤ N) (номер города, в котором живет профессор), F (1 ≤ F ≤ N) (номер города, в котором проводится конференция).
// Далее идет M строк, задающих расписание авиарейсов. i-я строка содержит три натуральных числа: Si, Fi и Pi, где Si — номер города, из которого вылетает i-й рейс, Fi — номер города, в который прилетает i-й рейс, Pi  — стоимость перелета i-м рейсом. Гарантируется, что 1 ≤ Si ≤ N, 1 ≤ Fi ≤ N, 1 ≤ Pi ≤ 10^6.
// Формат вывода
// Выведите одно число — минимальную стоимость пути, подходящего для профессора. Если профессор не сможет за K ночей добраться до конференции, выведите число -1.


#include <iostream>
#include <vector>

struct Edge {
    Edge(int f, int t, int w) : from(f), to(t), weight(w){};
    int from;
    int to;
    int weight;
};

class Graph {
public:
    explicit Graph(int size, int k) : size_graph_(size), count_days_(k){};
    void InsertOrientEdge(int ver_first, int ver_second, int w);
    void FordBellman(int start_ver);
    void PrintDist(int start_ver, int end_ver);

private:
    int size_graph_;
    std::vector<Edge> graph_;
    std::vector<std::vector<int64_t>> dist_;
    int count_days_;
    const int64_t max_dist_ = 100000000000;
};

void Graph::InsertOrientEdge(int ver_first, int ver_second, int w) {
    graph_.emplace_back(Edge(ver_first, ver_second, w));
}

void Graph::FordBellman(int start_ver) {
    dist_.assign(size_graph_, std::vector<int64_t>(count_days_ + 1, max_dist_));
    dist_[start_ver][0] = 0;
    for (int k = 1; k <= count_days_; ++k) {
        for (auto &edge : graph_) {
            if (dist_[edge.from][k - 1] < max_dist_) {
                if (dist_[edge.to][k] > dist_[edge.from][k - 1] + edge.weight) {
                    dist_[edge.to][k] = dist_[edge.from][k - 1] + edge.weight;
                }
            }
        }
    }
}

void Graph::PrintDist(int start_ver, int end_ver) {
    FordBellman(start_ver);
    int64_t min = max_dist_;
    for (int k = 0; k <= count_days_; ++k) {
        if (dist_[end_ver][k] < min) {
            min = dist_[end_ver][k];
        }
    }
    if (min == max_dist_) {
        std::cout << -1;
    } else {
        std::cout << min;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    int k = 0;
    int start = 0;
    int end = 0;
    std::cin >> n >> m >> k >> start >> end;
    Graph g(n, k);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int weight = 0;
        std::cin >> ver_first >> ver_second >> weight;
        g.InsertOrientEdge(--ver_first, --ver_second, weight);
    }
    g.PrintDist(--start, --end);
    return 0;
}
