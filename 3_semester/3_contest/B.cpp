

// В неориентированный взвешенный граф добавляют ребра. Напишите программу, которая в некоторые моменты находит сумму весов ребер в компоненте связности.

// Формат ввода
// В первой строке записано два числа n и m (1 ≤ n, m ≤ 10^6) — количество вершин в графе и количество производимых добавлений и запросов. Далее следует m строк с описанием добавления или запроса. Каждая строка состоит из двух или четырех чисел.
// Первое из чисел обозначает код операции. Если первое число 1, то за ним следует еще три числа x, y, w. Это означает, что в граф добавляется ребро xy веса w (1 ≤ x < y ≤ n, 1 ≤ w ≤ 1000). Кратные ребра допустимы.
// Если первое число 2, то за ним следует ровно одно число x. Это означает, что необходимо ответить на вопрос, какова сумма весов ребер в компоненте связности, которой принадлежит вершина x (1 ≤ x ≤ n).
// Формат вывода
// Для каждой операции с кодом 2 выведите ответ на поставленную задачу. Ответ на каждый запрос выводите на отдельной строке.


#include <iostream>
#include <vector>

class DSU {
public:
    explicit DSU(int size);
    void MakeSet(int ver);
    int FindSet(int ver);
    int64_t FindSetWeight(int ver);
    void UnionSets(int ver_first, int ver_second, int weight);

private:
    std::vector<int> parent_;
    std::vector<int> size_;
    std::vector<int64_t> weights_;
};

void DSU::UnionSets(int ver_first, int ver_second, int weight) {
    ver_first = FindSet(ver_first);
    ver_second = FindSet(ver_second);
    if (ver_second != ver_first) {
        if (size_[ver_first] < size_[ver_second]) {
            std::swap(ver_first, ver_second);
        }
        parent_[ver_second] = ver_first;
        size_[ver_first] += size_[ver_second];
        weights_[ver_first] += weights_[ver_second];
        weights_[ver_first] += weight;
    } else {
        weights_[ver_first] += weight;
    }
}

DSU::DSU(int size) : parent_(size), size_(size, 1), weights_(size, 0) {
    for (int i = 0; i < size; ++i) {
        parent_[i] = i;
    }
}

void DSU::MakeSet(int ver) {
    parent_[ver] = ver;
    size_[ver] = 1;
    weights_[ver] = 0;
}

int DSU::FindSet(int ver) {
    if (ver == parent_[ver]) {
        return ver;
    }
    return parent_[ver] = FindSet(parent_[ver]);
}

int64_t DSU::FindSetWeight(int ver) {
    ver = FindSet(ver);
    return weights_[ver];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    DSU sets(n);
    int task = 0;
    for (int i = 0; i < m; ++i) {
        std::cin >> task;
        if (task == 1) {
            int ver_first = 0;
            int ver_second = 0;
            int weight = 0;
            std::cin >> ver_first >> ver_second >> weight;
            sets.UnionSets(--ver_first, --ver_second, weight);
        } else {
            int ver = 0;
            std::cin >> ver;
            std::cout << sets.FindSetWeight(--ver) << "\n";
        }
    }
    return 0;
}
