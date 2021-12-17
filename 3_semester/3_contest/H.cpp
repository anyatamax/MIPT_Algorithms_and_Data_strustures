

// Вася и Сережа играют в следующую игру. В некоторых клетках клетчатого листка Сережа рисует один из символов H, O, N или C, после чего Вася должен провести между некоторыми находящимися в соседних клетках символами линии так, чтобы получилось корректное изображение химической молекулы. К сожалению, Сережа любит рисовать много символов, и Вася не может сразу определить, возможно ли вообще нарисовать линии нужным способом. Помогите ему написать программу, которая даст ответ на этот вопрос.
// В этой задаче проведенные между символами химических элементов линии будем считать корректным изображением молекулы, если они удовлетворяют следующим условиям:

//     каждая линия соединяет символы, нарисованные в соседних (по стороне) клетках;
//     между каждой парой символов проведено не более одной линии;
//     от каждого элемента отходит ровно столько линий, какова валентность этого элемента (1 для H, 2 для O, 3 для N, 4 для C);
//     пустые клетки ни с чем не соединены;
//     хотя бы в одной клетке нарисован какой-то символ.

// Формат ввода
// Первая строка входного файла содержит два натуральных числа n и m (1 ≤ n, m ≤ 50) — размеры листочка, на котором рисует Сережа.
// Далее следуют n строк по m символов в каждой, задающих конфигурацию химических элементов, которую нарисовал Сережа; пустые клетки задаются символом «.».
// Формат вывода
// В выходной файл выведите одно слово Valid, если линии провести требуемым образом можно, и Invalid, если нельзя. 



#include <iostream>
#include <vector>

struct Edge {
    Edge(int f, int t, int64_t fl, int64_t cap, int b) : from(f), to(t), flow(fl), capacity(cap), back(b){};
    int from;
    int to;
    int64_t flow;
    int64_t capacity;
    int back;
};

class Graph {
public:
    explicit Graph(int size) : graph_(size){};
    void InsertOrientEdge(int from, int to, int cap);
    int64_t FindFlow(int start, int finish, int64_t cur_flow, std::vector<bool> &status);
    int64_t MaxFlow(int start, int finish);

private:
    std::vector<std::vector<Edge>> graph_;
    const int max_flow_ = 1000000;
};

void Graph::InsertOrientEdge(int from, int to, int cap) {
    graph_[from].emplace_back(Edge(from, to, 0, cap, graph_[to].size()));
    graph_[to].emplace_back(Edge(to, from, 0, 0, graph_[from].size() - 1));
}

int64_t Graph::FindFlow(int start, int finish, int64_t cur_flow, std::vector<bool> &status) {
    if (start == finish) {
        return cur_flow;
    }
    status[start] = true;
    for (auto &cur_edge : graph_[start]) {
        if (!status[cur_edge.to] && cur_edge.capacity - cur_edge.flow > 0) {
            int64_t final_flow =
                FindFlow(cur_edge.to, finish, std::min(cur_flow, cur_edge.capacity - cur_edge.flow), status);
            if (final_flow > 0) {
                cur_edge.flow += final_flow;
                graph_[cur_edge.to][cur_edge.back].flow -= final_flow;
                return final_flow;
            }
        }
    }
    return 0;
}

int64_t Graph::MaxFlow(int start, int finish) {
    int64_t max_flow = 0;
    while (true) {
        std::vector<bool> status(graph_.size(), false);
        int64_t flow = FindFlow(start, finish, max_flow_, status);
        max_flow += flow;
        if (flow == 0) {
            return max_flow;
        }
    }
}

int CapacityEdge(char s) {
    if (s == 'H') {
        return 1;
    }
    if (s == 'O') {
        return 2;
    }
    if (s == 'N') {
        return 3;
    }
    if (s == 'C') {
        return 4;
    }
    return 0;
}

void AddEdgeFromMatr(Graph &graph, std::vector<std::vector<char>> &matr, std::vector<std::vector<int>> &matr_num,
                     int64_t &flow_first, int64_t &flow_second, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (((i & 1) == 1 && (j & 1) == 1) || ((i & 1) == 0 && (j & 1) == 0)) {
                graph.InsertOrientEdge(0, matr_num[i][j], CapacityEdge(matr[i][j]));
                flow_first += CapacityEdge(matr[i][j]);
                if (i > 0) {
                    graph.InsertOrientEdge(matr_num[i][j], matr_num[i - 1][j], 1);
                }
                if (i < n - 1) {
                    graph.InsertOrientEdge(matr_num[i][j], matr_num[i + 1][j], 1);
                }
                if (j > 0) {
                    graph.InsertOrientEdge(matr_num[i][j], matr_num[i][j - 1], 1);
                }
                if (j < m - 1) {
                    graph.InsertOrientEdge(matr_num[i][j], matr_num[i][j + 1], 1);
                }
            } else {
                graph.InsertOrientEdge(matr_num[i][j], n * m + 1, CapacityEdge(matr[i][j]));
                flow_second += CapacityEdge(matr[i][j]);
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
    std::cin >> n >> m;
    std::vector<std::vector<char>> matr(n, std::vector<char>(m));
    std::vector<std::vector<int>> matr_num(n, std::vector<int>(m));
    int count_start = 0;
    int count_end = n * m + 1;
    for (int i = 0; i < n; ++i) {
        std::string str;
        std::cin >> str;
        for (int j = 0; j < m; ++j) {
            matr[i][j] = str[j];
            if (((i & 1) == 1 && (j & 1) == 1) || ((i & 1) == 0 && (j & 1) == 0)) {
                matr_num[i][j] = ++count_start;
            } else {
                matr_num[i][j] = --count_end;
            }
        }
    }
    int64_t flow_first = 0;
    int64_t flow_second = 0;
    Graph graph(n * m + 2);
    AddEdgeFromMatr(graph, matr, matr_num, flow_first, flow_second, n, m);
    int64_t max_flow = graph.MaxFlow(0, n * m + 1);
    if (flow_first == 0 && flow_second == 0) {
        std::cout << "Invalid";
    } else if (max_flow != flow_first || flow_first != flow_second) {
        std::cout << "Invalid";
    } else {
        std::cout << "Valid";
    }
    return 0;
}
