

// Карту местности условно разбили на квадраты, и посчитали среднюю высоту над уровнем моря для каждого квадрата.
// Когда идет дождь, вода равномерно выпадает на все квадраты. Если один из четырех соседних с данным квадратом квадратов имеет меньшую высоту над уровнем моря, то вода с текущего квадрата стекает туда (и, если есть возможность, то дальше), если же все соседние квадраты имеют большую высоту, то вода скапливается в этом квадрате.
// Разрешается в некоторых квадратах построить водостоки. Когда на каком-то квадрате строят водосток, то вся вода, которая раньше скапливалась в этом квадрате, будет утекать в водосток
// Если есть группа квадратов, имеющих одинаковую высоту и образующих связную область, то если хотя бы рядом с одним из этих квадратов есть квадрат, имеющий меньшую высоту, то вся вода утекает туда, если же такого квадрата нет, то вода стоит во всех этих квадратах. При этом достаточно построить водосток в любом из этих квадратов, и вся вода с них будет утекать в этот водосток.
// Требуется определить, какое минимальное количество водостоков нужно построить, чтобы после дождя вся вода утекала в водостоки.

// Формат ввода
// Во входном файле записаны сначала числа N и M, задающие размеры карты — натуральные числа, не превышающие 100. Далее идет N строк, по M чисел в каждой, задающих высоту квадратов карты над уровнем моря. Высота задается натуральным числом, не превышающим 10000. Считается, что квадраты, расположенные за пределами карты, имеют высоту 10001 (то есть вода никогда не утекает за пределы карты).
// Формат вывода
// В выходной файл выведите минимальное количество водостоков, которое необходимо построить.


#include <iostream>
#include <vector>

class Graph {
public:
    void BuildGraph(std::vector<std::vector<int>>& matr, std::vector<int>& colors);
    int CountDrain() const;

private:
    std::vector<std::vector<int>> edges_;
};

void Graph::BuildGraph(std::vector<std::vector<int>>& matr, std::vector<int>& colors) {
    edges_.resize(colors.size());
    for (int i = 0; i < matr.size(); ++i) {
        for (int j = 0; j < matr[i].size(); ++j) {
            if (i > 0) {
                if (matr[i - 1][j] != matr[i][j] && colors[matr[i - 1][j]] < colors[matr[i][j]]) {
                    if (edges_[matr[i][j]].empty()) {
                        edges_[matr[i][j]].push_back(matr[i - 1][j]);
                    }
                }
            }
            if (i < matr.size() - 1) {
                if (matr[i + 1][j] != matr[i][j] && colors[matr[i + 1][j]] < colors[matr[i][j]]) {
                    if (edges_[matr[i][j]].empty()) {
                        edges_[matr[i][j]].push_back(matr[i + 1][j]);
                    }
                }
            }
            if (j > 0) {
                if (matr[i][j - 1] != matr[i][j] && colors[matr[i][j - 1]] < colors[matr[i][j]]) {
                    if (edges_[matr[i][j]].empty()) {
                        edges_[matr[i][j]].push_back(matr[i][j - 1]);
                    }
                }
            }
            if (j < matr[i].size() - 1) {
                if (matr[i][j + 1] != matr[i][j] && colors[matr[i][j + 1]] < colors[matr[i][j]]) {
                    if (edges_[matr[i][j]].empty()) {
                        edges_[matr[i][j]].push_back(matr[i][j + 1]);
                    }
                }
            }
        }
    }
}

int Graph::CountDrain() const {
    int count = 0;
    for (int i = 0; i < edges_.size(); ++i) {
        if (edges_[i].empty()) {
            ++count;
        }
    }
    return count;
}

void Colors(std::vector<std::vector<int>>& matr, std::vector<std::vector<int>>& matr_new, int i, int j, int n, int m) {
    if (i > 0) {
        if (matr[i][j] == matr[i - 1][j] && matr_new[i - 1][j] != matr_new[i][j]) {
            matr_new[i - 1][j] = matr_new[i][j];
            Colors(matr, matr_new, i - 1, j, n, m);
        }
    }
    if (i < n - 1) {
        if (matr[i][j] == matr[i + 1][j] && matr_new[i + 1][j] != matr_new[i][j]) {
            matr_new[i + 1][j] = matr_new[i][j];
            Colors(matr, matr_new, i + 1, j, n, m);
        }
    }
    if (j > 0) {
        if (matr[i][j] == matr[i][j - 1] && matr_new[i][j - 1] != matr_new[i][j]) {
            matr_new[i][j - 1] = matr_new[i][j];
            Colors(matr, matr_new, i, j - 1, n, m);
        }
    }
    if (j < m - 1) {
        if (matr[i][j] == matr[i][j + 1] && matr_new[i][j + 1] != matr_new[i][j]) {
            matr_new[i][j + 1] = matr_new[i][j];
            Colors(matr, matr_new, i, j + 1, n, m);
        }
    }
}

int main() {
    std::vector<std::vector<int>> matr;
    int n, m;
    std::cin >> n >> m;
    matr.resize(n);
    for (int i = 0; i < n; ++i) {
        matr[i].resize(m);
        for (int j = 0; j < m; ++j) {
            std::cin >> matr[i][j];
        }
    }
    std::vector<std::vector<int>> matr_new;
    std::vector<int> colors;
    matr_new.resize(n);
    for (int i = 0; i < n; ++i) {
        matr_new[i].resize(m, -1);
    }
    int count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matr_new[i][j] == -1) {
                matr_new[i][j] = count;
                colors.push_back(matr[i][j]);
                ++count;
                Colors(matr, matr_new, i, j, n, m);
            }
        }
    }
    Graph g;
    g.BuildGraph(matr_new, colors);
    std::cout << g.CountDrain();
    return 0;
}
