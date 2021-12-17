

// В славном городе Нью-Йорке в не менее славном районе Манхэттен на некоторых перекрестках установлены рестораны сети Subway. Также в этом районе на каждом перекрестке установлены офисные здания.

// Для упрощения дел, будем считать Манхэттен районом, где все дороги идут перпендикулярно, а сам он имеет вид прямоугольника размера N × M перекрестков. Расстояние между клетками с координатами (x1, y1), (x2, y2) равно числу участков дорог, которые надо проехать (или |x1 - x2| + |y1 - y2|).

// Нужно по расположению ресторанов Subway узнать для каждого перекрестка минимальное расстояние от него до одного из ресторанов. Гарантируется, что хотя бы один ресторан имеется в Манхэттене.

// Формат ввода
// В первой строке вводятся два натуральных числа N и M, не превосходящих 1000 — размеры Манхэттена. Далее идут N строк по M чисел — карта перекрестков района, где 1 означает наличие ресторана на этом перекрестке, и 0 — его отсутствие.
// Формат вывода
// Требуется вывести N строк по M чисел, где для каждого перекрестка указано расстояние до ближайшего Саба. 


#include <iostream>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int size_n, int size_m, std::vector<std::vector<int>>& map) : distance_(size_n), matrix_map_(map) {
        for (int i = 0; i < size_n; ++i) {
            distance_[i].resize(size_m, MAX_DISTANCE_);
        }
    };
    void MapDistance();
    void PrintMapDistance() const;
    void Bfs();

private:
    const int MAX_DISTANCE_ = 5000000;
    std::vector<std::vector<int>> distance_;
    std::vector<std::vector<int>> matrix_map_;
};

void Graph::Bfs() {
    std::queue<std::pair<int, int>> queue_graph;
    for (int i = 0; i < distance_.size(); ++i) {
        for (int j = 0; j < distance_[i].size(); ++j) {
            if (matrix_map_[i][j] == 1) {
                distance_[i][j] = 0;
                queue_graph.push(std::make_pair(i, j));
            }
        }
    }
    std::vector<std::vector<bool>> distance_bool(distance_.size());
    for (int i = 0; i < distance_bool.size(); ++i) {
        distance_bool[i].resize(distance_[i].size(), false);
    }
    while(!queue_graph.empty()) {
        std::pair<int, int> new_ver = queue_graph.front();
        queue_graph.pop();
        int index_i = new_ver.first;
        int index_j = new_ver.second;
        if (distance_bool[index_i][index_j]) {
            continue;
        }
        if (index_i >= 1) {
            if (distance_[index_i - 1][index_j] >= distance_[index_i][index_j] + 1) {
                distance_[index_i - 1][index_j] = std::min(distance_[index_i - 1][index_j],
                                                           distance_[index_i][index_j] + 1);
                queue_graph.push(std::make_pair(index_i - 1, index_j));
            }
        }
        if (index_i < distance_.size() - 1) {
            if (distance_[index_i + 1][index_j] >= distance_[index_i][index_j] + 1) {
                distance_[index_i + 1][index_j] = std::min(distance_[index_i + 1][index_j],
                                                           distance_[index_i][index_j] + 1);
                queue_graph.push(std::make_pair(index_i + 1, index_j));
            }
        }
        if (index_j >= 1) {
            if (distance_[index_i][index_j - 1] >= distance_[index_i][index_j] + 1) {
                distance_[index_i][index_j - 1] = std::min(distance_[index_i][index_j - 1],
                                                           distance_[index_i][index_j] + 1);
                queue_graph.push(std::make_pair(index_i, index_j - 1));
            }
        }
        if (index_j < distance_[index_i].size() - 1) {
            if (distance_[index_i][index_j + 1] >= distance_[index_i][index_j] + 1) {
                distance_[index_i][index_j + 1] = std::min(distance_[index_i][index_j + 1],
                                                           distance_[index_i][index_j] + 1);
                queue_graph.push(std::make_pair(index_i, index_j + 1));
            }
        }
        distance_bool[index_i][index_j] = true;
    }
}

void Graph::PrintMapDistance() const {
    for (auto & i : distance_) {
        for (int j : i) {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<int>> map_city(n);
    for (int i = 0; i < n; ++i) {
        map_city[i].resize(m);
        for (int j = 0; j < m; ++j) {
            std::cin >> map_city[i][j];
        }
    }
    Graph graph(n, m, map_city);
    graph.Bfs();
    graph.PrintMapDistance();
    return 0;
}
