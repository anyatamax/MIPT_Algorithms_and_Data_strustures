

// Вам нужно распространить новость о посвяте среди всех первокурсников. Известно, что первокурсники общаются друг с другом через чаты, при этом если новость вброшена в чат, то гарантируется что все его участники прочтут новость (эх, было бы так не только в условии этой задачи).
// Так как у вас уже и так очень много своих чатов, вы хотите вступить в как можно меньшее их количество, чтобы все первокурсники узнали, что посвят уже скоро. При этом вам неизвестны составы чатов, но до вас дошли сведения, что некоторые пары первокурсников гарантированно сидят в одном чате. Более того, если студент сидит в чате и видит новость, то он пересылает сообщение с важной информации во все остальные чаты, где он есть.

// Формат ввода
// Во входном файле записано два числа N — число первокурсников и M — число известных вам пар первокурсников, сидящих в одном чате (1 ≤ N ≤ 100000, 0 ≤ M ≤ 100000). В следующих M строках записаны по два числа i и j (1 ≤ i, j ≤ N), которые означают, что первокурсники с номерами i и j в одном чате.
// Формат вывода
// В первой строчке выходного файла выведите количество чатов, куда вам надо вступить. Далее выведите группы студентов, которые узнают о новости для каждого вступления в чат: в первой строке количество студентов в такой группе в компоненте, во второй — номера студентов в произвольном порядке. 


#include <iostream>
#include <list>
#include <vector>

enum COLOR {
    WHITE,
    GREY,
    BLACK,
};

class Graph {
public:
    explicit Graph(int size) : adj_(size), colors_(size, WHITE) {};
    void InsertEdgeNoOrient(int ver_first, int ver_second);
    void Dfs(int v, std::list<int>& component);
    std::vector<std::list<int>> ComponentsConnect();

private:
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
};

void Graph::InsertEdgeNoOrient(int ver_first, int ver_second) {
    adj_[ver_first].push_back(ver_second);
    adj_[ver_second].push_back(ver_first);
}

void Graph::Dfs(int ver, std::list<int>& component) {
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            Dfs(u,component);
        }
    }
    colors_[ver] = BLACK;
    component.push_front(ver);
}

std::vector<std::list<int>> Graph::ComponentsConnect() {
    std::vector<std::list<int>> components;
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            std::list<int> component;
            Dfs(v, component);
            components.push_back(component);
        }
    }
    return components;
}


int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        std::cin >> ver_first >> ver_second;
        g.InsertEdgeNoOrient(--ver_first, --ver_second);
    }
    std::vector<std::list<int>> result = g.ComponentsConnect();
    std::cout << result.size() << "\n";
    for (auto & i : result) {
        std::cout << i.size() << "\n";
        for (auto& j: i) {
            std::cout << j + 1 << " ";
        }
        std::cout << "\n";
    }
    return 0;
}
