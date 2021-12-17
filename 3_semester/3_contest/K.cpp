

// # include "отказ_от_авторских_прав.h"
// Поможем Джоан Роулинг развить сюжетную дыру, связанную с маховиками времени. После битвы с тем-кого-нельзя-называть магия вышла на качественно новый уровень. Теперь маховики времени способны не только переносить назад во времени, но и в принципе перемещаться в любую заданную точку мира в любой заданный момент времени. В связи с тем, что это могло бы вызвать хаос, Министерство Магии решило наложить некоторые ограничения на них.
// А именно, есть N магических станций, в которых возможно использование маховиков, при этом любая телепортация может совершаться только в один из этих пунктов. При этом для телепортаций есть определенное расписание. Можно сравнить это с магловскими авиаперелетами, только время работает на руку волшебникам.
// Легенда закончена, теперь сама задача. В момент времени 0 вы находитесь в магической станции A. Вам дано расписание рейсов. Требуется оказаться в магической станции B как можно раньше (то есть в наименьший возможный момент времени).
// При этом разрешается делать сложные маршруты, использующие несколько магических станций. Если вы прибываете на некоторую станцию в момент времени T, то вы можете телепортироваться из него любым рейсом, который отправляется из этой станции в момент времени T или позднее (но не раньше).

// Формат ввода
// В первой строке вводится число N — количество магических станций (1 ≤ N ≤ 1000). Вторая строка содержит два числа A и B — номера начальной и конечной станций. В третьей строке задается K — количество всевозможных телепортаций (0 ≤ K ≤ 1000).
// Следующие K строк содержат описания телепортаций, по одной на строке. Каждое описание представляет собой четверку целых чисел. Первое число каждой четверки задает номер станции отправления, второе — время отправления, третье — станция назначения, четвертое — время прибытия. Номера станций — натуральные числа из диапазона от 1 до N.
// Станции назначения и отправления могут совпадать (да-да, петли в пространстве и времени). Время измеряется в некоторых магических единицах и задается целым числом, по модулю не превышающим 10^9. Поскольку телепортации совершаются с использованием маховиков времени, то время прибытия может быть как больше времени отправления, так и меньше, или равным ему.
// Гарантируется, что входные данные таковы, что добраться из станции A до станции B всегда можно.
// Формат вывода
// Выведите минимальное время, когда вы сможете оказаться в пункте B. 


#include <iostream>
#include <vector>

struct Edge {
    Edge(int f, int t, int t_f, int t_t) : from(f), to(t), time_from(t_f), time_to(t_t){};
    int from;
    int to;
    int time_from;
    int time_to;
};

class Graph {
public:
    explicit Graph(int size) : size_graph_(size){};
    void InsertOrientEdge(int ver_first, int ver_second, int time_from, int time_to);
    void FordBellman(int start_ver);
    void PrintDist(int start, int end);

private:
    int size_graph_;
    std::vector<Edge> graph_;
    std::vector<int> dist_;
    const int max_dist_ = 2009000001;
};

void Graph::InsertOrientEdge(int ver_first, int ver_second, int time_from, int time_to) {
    graph_.emplace_back(Edge(ver_first, ver_second, time_from, time_to));
}

void Graph::FordBellman(int start_ver) {
    dist_.assign(size_graph_, max_dist_);
    dist_[start_ver] = 0;
    size_graph_ = graph_.size();
    for (int ver = 0; ver < size_graph_; ++ver) {
        for (auto &edge : graph_) {
            if (dist_[edge.from] < max_dist_ && edge.time_from >= dist_[edge.from]) {
                if (dist_[edge.to] > edge.time_to) {
                    dist_[edge.to] = edge.time_to;
                }
            }
        }
    }
}

void Graph::PrintDist(int start, int end) {
    FordBellman(start);
    std::cout << dist_[end];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    int start = 0;
    int end = 0;
    std::cin >> n >> start >> end >> m;
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        int time_from = 0;
        int time_to = 0;
        std::cin >> ver_first >> time_from >> ver_second >> time_to;
        g.InsertOrientEdge(--ver_first, --ver_second, time_from, time_to);
    }
    g.PrintDist(--start, --end);
    return 0;
}
