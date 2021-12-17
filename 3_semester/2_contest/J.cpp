

// В столице одной небольшой страны очень сложная ситуация. Многокилометровые пробки буквально парализовали движение в городе, и власти на многих улицах ввели одностороннее движение, не анализируя, можно ли будет теперь проехать из любого места в городе в любое другое, не нарушая правила. Транспортная система столицы представляет собой N площадей, соединенных M полосами для движения, в том числе круговыми полосами, проходящими по площади. Каждая полоса предназначена для движения только в одну определенную сторону. При этом на магистралях есть полосы, направленные как в одну, так и в другую сторону. По круговой полосе можно двигаться только внутри площади и только против часовой стрелки.
// Власти города на каждой полосе разместили видеокамеру, поэтому если Иннокентий едет по встречной полосе (при ее наличии) или, в случае одностороннего движения, в сторону противоположную предписанной знаками, то после поездки против правил по каждой из полос ему придется заплатить штраф в размере одной тысячи тугриков этой страны.
// Иннокентий, который торопится купить кафельную плитку со скидкой, решился доехать до магазина в любом случае, даже если для этого придется нарушать правила. Но он хочет выбрать такой маршрут движения, суммарный штраф на котором минимален.
// Иннокентий еще не решил, откуда именно и в какой магазин он собирается ехать, поэтому ему необходимо ответить на несколько вопросов вида «Какой минимальный штраф надо заплатить, чтобы добраться из пункта A в пункт B?». Отвечая на потребности жителей столицы, известная поисковая система Индекс разрабатывает соответствующий сервис.
// Так как многие из вас рано или поздно будут проходить собеседование на работу в эту фирму, продемонстрируйте, что вы тоже умеете решать эту задачу.

// Формат ввода
// В первой строке входных данных содержатся два числа N и M — количество площадей и полос движения в городе соответственно (1 ≤ N ≤ 5000, 1 ≤ M ≤ 10 000). Далее содержатся описания полос, по которым движение разрешено. Каждая полоса описывается номерами двух площадей, которые она соединяет. Движение разрешено в направлении от первой из указанных площадей ко второй.
// В следующей строке содержится одно число K — количество вопросов у Иннокентия (1 ≤ K ≤ 10 000, N ⋅ K ≤ 2 ⋅ 10^7). В следующих строках описываются вопросы, каждый вопрос описывается номерами двух площадей, между которыми требуется найти самый дешевый путь. Путь необходимо проложить от первой из указанных площадей ко второй.
// Формат вывода
// Для каждого вопроса выведите одно число — искомый минимальный размер штрафа в тысячах тугриков. В случае, если пути между выбранной парой площадей не существует, выведите - 1. 


#include <algorithm>
#include <iostream>
#include <deque>
#include <vector>

class Graph {
public:
    explicit Graph(int size_n) : distance_(size_n, MAX_DISTANCE_), adj_(size_n) {
    };
    void InsertEdgeOrient(int ver_first, int ver_second);
    void MinDistance(int ver);
    int GetDistance(int ver) const;
    struct Ver {
        int ver;
        int weight;
        friend bool operator==(const Ver& lhs, const Ver& rhs);
    };


private:
    const int MAX_DISTANCE_ = 9000001;
    std::vector<int> distance_;
    std::vector<std::vector<Ver>> adj_;
};

bool operator==(const Graph::Ver& lhs, const Graph::Ver& rhs) {
    return (lhs.ver == rhs.ver && lhs.weight == rhs.weight);
}

void Graph::InsertEdgeOrient(int ver_first, int ver_second) {
    if (ver_second == ver_first) {
        return;
    }
    Ver new_ver_first = {ver_second, 0};
    Ver new_ver_second = {ver_first, 1};
    auto ver = std::find(adj_[ver_first].begin(), adj_[ver_first].end(), new_ver_first);
    if (ver == adj_[ver_first].end()) {
        adj_[ver_first].push_back(new_ver_first);
        ver = std::find(adj_[ver_second].begin(), adj_[ver_second].end(), Ver{ver_first, 0});
        if (ver == adj_[ver_second].end()) {
            adj_[ver_second].push_back(new_ver_second);
        }
    }
}

void Graph::MinDistance(int start_ver) {
    for (auto &i : distance_) {
        i = MAX_DISTANCE_;
    }
    distance_[start_ver] = 0;
    std::deque<int> deque;
    deque.push_back(start_ver);
    while (!deque.empty()) {
        int ver = deque.front();
        deque.pop_front();
        for (auto &u : adj_[ver]) {
            int to = u.ver;
            if (distance_[to] > distance_[ver] + u.weight) {
                distance_[to] = distance_[ver] + u.weight;
                if (u.weight == 0) {
                    deque.push_front(to);
                } else {
                    deque.push_back(to);
                }
            }
        }
    }
}

int Graph::GetDistance(int end_ver) const {
    if (distance_[end_ver] == MAX_DISTANCE_) {
        return -1;
    } else {
        return distance_[end_ver];
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        std::cin >> ver_first >> ver_second;
        graph.InsertEdgeOrient(--ver_first, --ver_second);
    }
    int count_task = 0;
    std::cin >> count_task;
    for (int i = 0; i < count_task; ++i) {
        int start_ver = 0;
        int end_ver = 0;
        std::cin >> start_ver >> end_ver;
        graph.MinDistance(--start_ver);
        std::cout << graph.GetDistance(--end_ver) << "\n";
    }
    return 0;
}
