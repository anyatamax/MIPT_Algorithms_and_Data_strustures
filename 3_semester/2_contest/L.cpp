// У Васи есть N свинок-копилок, свинки занумерованы числами от 1 до N. Каждая копилка может быть открыта единственным соответствующим ей ключом или разбита.
// Вася положил ключи в некоторые из копилок (он помнит, какой ключ лежит в какой из копилок). Теперь Вася собрался купить машину, а для этого ему нужно достать деньги из всех копилок. При этом он хочет разбить как можно меньшее количество копилок (ведь ему еще нужно копить деньги на квартиру, дачу, вертолет...). Помогите Васе определить, какое минимальное количество копилок нужно разбить.

// Формат ввода
// В первой строке содержится число N — количество свинок-копилок (1 ≤ N ≤ 100). Далее идет N строк с описанием того, где лежит ключ от какой копилки: в i-ой из этих строк записан номер копилки, в которой находится ключ от i-ой копилки.
// Формат вывода
// Выведите единственное число: минимальное количество копилок, которые необходимо разбить.


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
    void InsertEdgeOrient(int ver_first, int ver_second);
    void Dfs(int v);
    int CountCycle();

private:
    std::vector<COLOR> colors_;
    std::vector<std::vector<int>> adj_;
    int count_cycle_ = 0;
};

void Graph::InsertEdgeOrient(int ver_first, int ver_second) {
    adj_[ver_first].push_back(ver_second);
}

void Graph::Dfs(int ver) {
    colors_[ver] = GREY;
    for(auto u: adj_[ver]) {
        if (colors_[u] == WHITE) {
            Dfs(u);
        } else if (colors_[u] == GREY) {
            count_cycle_++;
        }
    }
    colors_[ver] = BLACK;
}

int Graph::CountCycle() {
    for (int v = 0; v < colors_.size(); ++v) {
        if (colors_[v] == WHITE) {
            Dfs(v);
        }
    }
    return count_cycle_;
}


int main() {
    int n = 0;
    std::cin >> n;
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        int ver_first = 0;
        int ver_second = i;
        std::cin >> ver_first;
        g.InsertEdgeOrient(--ver_first, ver_second);
    }
    std::cout << g.CountCycle();
    return 0;
}
