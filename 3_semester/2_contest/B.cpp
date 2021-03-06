

// Во время контрольной работы профессор Флойд заметил, что некоторые студенты обмениваются записками. Сначала он хотел поставить им всем двойки, но в тот день профессор был добрым, а потому решил разделить студентов на две группы: списывающих и дающих списывать, и поставить двойки только первым.
// У профессора записаны все пары студентов, обменявшихся записками. Требуется определить, сможет ли он разделить студентов на две группы так, чтобы любой обмен записками осуществлялся от студента одной группы студенту другой группы.

// Формат ввода
// В первой строке находятся два числа N и M — количество студентов и количество пар студентов, обменивающихся записками (1 ≤ N ≤ 100, 0 ≤ M ≤ N ⋅ (N - 1) / 2). Далее в M строках расположены описания пар студентов: два числа, соответствующие номерам студентов, обменивающихся записками (нумерация студентов идёт с 1). Каждая пара студентов перечислена не более одного раза.
// Формат вывода
// Необходимо вывести ответ на задачу профессора Флойда. Если возможно разделить студентов на две группы — выведите YES; иначе выведите NO. 


#include <algorithm>
#include <iostream>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), colors_(size, WHITE), set_color_(size) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second);
    void IsAbleTwoColors();
    bool IsTwoToneGraph() const;

private:
    bool can_two_tone = true;
    std::vector<std::vector<int>> adj_;
    std::vector<COLOR> colors_;
    std::vector<int> set_color_;
    void Dfs(int ver, int color);
};

void Graph::InsertEdgeNoOrient(int ver_first, int ver_second) {
    if (std::find(adj_[ver_first].begin(), adj_[ver_first].end(), ver_second) == adj_[ver_first].end()) {
        adj_[ver_first].push_back(ver_second);
    }
    if (std::find(adj_[ver_second].begin(), adj_[ver_second].end(), ver_first) == adj_[ver_second].end()) {
        adj_[ver_second].push_back(ver_first);
    }
}

void Graph::Dfs(int ver, int color) {
    if (!can_two_tone) {
        return;
    }
    colors_[ver] = GREY;
    set_color_[ver] = color;
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            Dfs(u, 3 - color);
        } else if (set_color_[ver] == set_color_[u]) {
            can_two_tone = false;
        }
    }
    colors_[ver] = BLACK;
}
void Graph::IsAbleTwoColors() {
    for (int i = 0; i < adj_.size(); ++i) {
        if (colors_[i] == WHITE) {
            Dfs(i, 1);
        }
    }
}
bool Graph::IsTwoToneGraph() const {
    return can_two_tone;
}


int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int ver_first, ver_second;
        std::cin >> ver_first >> ver_second;
        --ver_second;
        --ver_first;
        graph.InsertEdgeNoOrient(ver_first, ver_second);
    }
    graph.IsAbleTwoColors();
    if (graph.IsTwoToneGraph()) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
