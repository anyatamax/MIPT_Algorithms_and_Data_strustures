

// На день рождения юному технику Мише подарили машинку с радиоуправлением. Мише быстро наскучило гонять машинку туда-сюда по комнате, и он соорудил специальную трассу. Для этого он разбил комнату на квадратные ячейки, некоторые из них оставив пустыми, а в некоторые поставив препятствия. Целую неделю Миша каждый день улучшал свой рекорд по прохождению трассы. Но каково же было разочарование Миши, когда к нему в гости пришел Тима со своей машинкой и побил его рекорд. Стало понятно, что машинку необходимо модернизировать.

// На пробных испытаниях, которые были произведены через день, Миша обнаружил, что машинка действительно ездит лучше, однако ее поведение несколько изменилось. На пульте теперь функционируют только четыре кнопки: вперед, назад, вправо, влево. При нажатии на них машинка едет по направлению к соответствующей стене комнаты, являющейся одновременно границей трассы, точно перпендикулярно ей. Машинка разгоняется до такой скорости, что перестает реагировать на другие команды, врезается в ближайшее препятствие или стену и отскакивает от нее на половину пройденного расстояния, то есть если между машинкой и стеной было x пустых клеток, то после отскока она остановится на клетке, от которой x/2 клеток до стены.

// Теперь Мише интересно, какое минимальное количество раз необходимо нажать на кнопку пульта, чтобы машинка, начав в клетке старта, остановилась в клетке финиша.

// Формат ввода
// Первая строка входного файла содержит два целых числа n и m — размеры трассы (2 ≤ m, n ≤ 20). Следующие n строк содержат по m символов каждая: символ «.» соответствует пустой клетке, «#» — препятствию, а «S» и «T» — клетке старта и клетке финиша соответственно.
// Формат вывода
// В выходной файл выведите минимальное количество нажатий на кнопки пульта для проведения машинки по трассе от старта до финиша.
// Если доехать от старта до финиша невозможно, выведите -1.


#include <iostream>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int size_n, int size_m, std::vector<std::vector<char>>& map) : distance_(size_n), matrix_map_(map) {
        for (int i = 0; i < size_n; ++i) {
            distance_[i].resize(size_m, MAX_DISTANCE_);
        }
    };
    int GetDistance(std::pair<int, int> ver) const;
    void Bfs(std::pair<int, int> ver);

private:
    const int MAX_DISTANCE_ = 50001;
    std::vector<std::vector<int>> distance_;
    std::vector<std::vector<char>> matrix_map_;
};

void Graph::Bfs(std::pair<int, int> ver) {
    std::queue<std::pair<int, int>> queue_graph;
    queue_graph.push(ver);
    distance_[ver.first][ver.second] = 0;
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
            int count_to_wall = 0;
            int index_wall = 0;
            for (int i = index_i - 1; i >= 0; --i) {
                if (matrix_map_[i][index_j] != '#') {
                    ++count_to_wall;
                }
                if (matrix_map_[i][index_j] == '#') {
                    index_wall = i;
                    break;
                } else if (i == 0) {
                    index_wall = -1;
                }
            }
            int new_index_i = index_wall + 1 + count_to_wall / 2;
            distance_[new_index_i][index_j] = std::min(distance_[new_index_i][index_j], distance_[index_i][index_j] + 1);
            queue_graph.push(std::make_pair(new_index_i, index_j));
        }
        if (index_i < distance_.size() - 1) {
            int count_to_wall = 0;
            int index_wall = 0;
            for (int i = index_i + 1; i < distance_.size(); ++i) {
                if (matrix_map_[i][index_j] != '#') {
                    ++count_to_wall;
                }
                if (matrix_map_[i][index_j] == '#') {
                    index_wall = i;
                    break;
                } else if (i == distance_.size() - 1) {
                    index_wall = distance_.size();
                }
            }
            int new_index_i = index_wall - 1 - count_to_wall / 2;
            distance_[new_index_i][index_j] = std::min(distance_[new_index_i][index_j], distance_[index_i][index_j] + 1);
            queue_graph.push(std::make_pair(new_index_i, index_j));
        }
        if (index_j >= 1) {
            int count_to_wall = 0;
            int index_wall = 0;
            for (int j = index_j - 1; j >= 0; --j) {
                if (matrix_map_[index_i][j] != '#') {
                    ++count_to_wall;
                }
                if (matrix_map_[index_i][j] == '#') {
                    index_wall = j;
                    break;
                } else if (j == 0) {
                    index_wall = -1;
                }
            }
            int new_index_j = index_wall + 1 + count_to_wall / 2;
            distance_[index_i][new_index_j] = std::min(distance_[index_i][new_index_j], distance_[index_i][index_j] + 1);
            queue_graph.push(std::make_pair(index_i, new_index_j));
        }
        if (index_j < distance_[index_i].size() - 1) {
            int count_to_wall = 0;
            int index_wall = 0;
            for (int j = index_j + 1; j < distance_[index_i].size(); ++j) {
                if (matrix_map_[index_i][j] != '#') {
                    ++count_to_wall;
                }
                if (matrix_map_[index_i][j] == '#') {
                    index_wall = j;
                    break;
                } else if (j == distance_[index_i].size() - 1) {
                    index_wall = distance_[index_i].size();
                }
            }
            int new_index_j = index_wall - 1 - count_to_wall / 2;
            distance_[index_i][new_index_j] = std::min(distance_[index_i][new_index_j], distance_[index_i][index_j] + 1);
            queue_graph.push(std::make_pair(index_i, new_index_j));
        }
        distance_bool[index_i][index_j] = true;
    }
}

int Graph::GetDistance(std::pair<int, int> ver) const {
    if (distance_[ver.first][ver.second] == MAX_DISTANCE_) {
        return -1;
    }
    return distance_[ver.first][ver.second];
}

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<char>> trace(n);
    std::pair<int, int> start_ver;
    std::pair<int ,int> end_ver;
    for (int i = 0; i < n; ++i) {
        trace[i].resize(m);
        for (int j = 0; j < m; ++j) {
            std::cin >> trace[i][j];
            if (trace[i][j] == 'S') {
                start_ver.first = i;
                start_ver.second = j;
            }
            if (trace[i][j] == 'T') {
                end_ver.first = i;
                end_ver.second = j;
            }
        }
    }
    Graph graph(n, m, trace);
    graph.Bfs(start_ver);
    std::cout << graph.GetDistance(end_ver);
    return 0;
}
