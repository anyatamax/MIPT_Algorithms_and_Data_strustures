

// Для решения транспортной проблемы в некотором городе до недавнего времени использовались N автобусных маршрутов. Каждый маршрут начинался на одной из M площадей и там же заканчивался. В процессе проезда по маршруту автобус мог несколько раз проезжать одну и ту же площадь, и даже мог проезжать более одного раза по одной и той же улице в одном и том же направлении. В определенный момент местные власти решили сократить количество автобусных маршрутов в городе до одного. По их мнению, должен был остаться лишь один маршрут, который проходил бы по всем улицам, по которым раньше проходили автобусные маршруты, причем в том же направлении (но не обязательно в том же порядке). Если по каким-либо улицам автобусы ездили в обоих направлениях, то и новый маршрут должен проходить по этим улицам в обоих направлениях. По тем улицам и в тех направлениях, по которым раньше автобусы не ездили, новый маршрут проходить не должен. Однако так как контролеров увольнять нельзя, власти решили, что по каждой улице в каждом направлении новый маршрут должен проходить столько раз, сколько по ней проходили все старые маршруты, вместе взятые. Требуется написать программу, которая для заданных исходных данных определяет требуемый местным властям автобусный маршрут.

// Формат ввода
// Входной файл состоит из следующей последовательности строк. Первая строка содержит число N (N ≤ 10^5) — количество автобусных маршрутов, M (M ≤ 10^5) — количество площадей. Каждая из последующих N строк служит для описания соответствующего автобусного маршрута и содержит сначала число k (k ≤ 10^5), определяющее количество элементов маршрута, а затем k + 1 чисел, задающих номера площадей, которые последовательно проезжает автобус на этом маршруте. Общая длина маршрутов не более 10^5 улиц. При описании маршрута всегда задаются номера первой и последней площади маршрута, причем они всегда совпадают.
// Формат вывода
// В строку через пробел выведите число площадей в найденном маршруте (стартовая и конечная площади совпадают, но учитываются по отдельности) и саму последовательность площадей. Если организовать требуемый маршрут не удастся, вывод должен содержать одно число - 0. A


#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <stack>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), colors_(size, WHITE), deg_in_(size, 0), deg_out_(size, 0) {};
    void InsertEdgeOrient(int ver_first, int ver_second);
    void Dfs(int v);
    bool IsEulerGraph();
    std::vector<int> EulerPath();

private:
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> euler_path_;
    std::set<std::pair<int, int>> edges_;
    std::map<std::pair<int, int>, long long int> count_each_edge_;
    std::vector<long int> deg_in_;
    std::vector<long int> deg_out_;
    int ver_start_ = -1;
};

bool Graph::IsEulerGraph() {
    for (int i = 0; i < adj_.size(); ++i) {
        if (!adj_[i].empty() && ver_start_ == -1) {
            ver_start_ = i;
        }
        if (deg_in_[i] != deg_out_[i]) {
            return false;
        }
    }
    if (ver_start_ == -1) {
        return false;
    }
    Dfs(ver_start_);
    for (int i = 0; i < adj_.size(); ++i) {
        if (!adj_[i].empty() && colors_[i] == WHITE) {
            return false;
        }
    }
    return true;
}
std::vector<int> Graph::EulerPath() {
    std::stack<int> s;
    s.push(ver_start_);
    while(!s.empty()) {
        int ver = s.top();
        bool found_edge = false;
        for (auto &i : adj_[ver]) {
            std::pair<int, int> pair_ver = std::make_pair(ver, i);
            if (count_each_edge_.find(pair_ver) != count_each_edge_.end() && count_each_edge_[pair_ver] != 0) {
                s.push(i);
                --count_each_edge_[pair_ver];
                if (count_each_edge_[pair_ver] == 0) {
                    edges_.erase(pair_ver);
                }
                found_edge = true;
                break;
            }
        }
        if (!found_edge) {
            s.pop();
            euler_path_.push_back(ver);
        }
    }
    return euler_path_;
}

void Graph::InsertEdgeOrient(int ver_first, int ver_second) {
    deg_out_[ver_first]++;
    deg_in_[ver_second]++;
    std::pair<int, int> pair_ver = std::make_pair(ver_first, ver_second);
    if (edges_.find(pair_ver) != edges_.end()) {
        ++count_each_edge_[pair_ver];
    } else {
        adj_[ver_first].push_back(ver_second);
        edges_.insert(pair_ver);
        count_each_edge_[pair_ver] = 1;
    }
}

void Graph::Dfs(int ver) {
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            Dfs(u);
        }
    }
    colors_[ver] = BLACK;
}

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph g(m);
    for (int i = 0; i < n; ++i) {
        int count_ver = 0;
        std::cin >> count_ver;
        int ver_first = 0;
        int ver_second = 0;
        std::cin >> ver_first;
        --ver_first;
        for (int j = 1; j <= count_ver; ++j) {
            std::cin >> ver_second;
            --ver_second;
            g.InsertEdgeOrient(ver_first, ver_second);
            ver_first = ver_second;
        }
    }
    if (g.IsEulerGraph()) {
        std::vector<int> result = g.EulerPath();
        std::cout << result.size() << " ";
        for (auto & i : result) {
            std::cout << i + 1 << " ";
        }
    } else {
        std::cout << 0;
    }
    return 0;
}
