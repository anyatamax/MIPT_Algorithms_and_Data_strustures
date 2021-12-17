

// На шахматной доске N × N в клетке (x1, y1) стоит голодный шахматный конь. Он хочет попасть в клетку (x2, y2), где растет вкусная шахматная трава. Какое наименьшее количество ходов он должен для этого сделать?

// Формат ввода
// На вход программы поступает пять чисел: N, x1, y1, x2, y2 (5 ≤ N ≤ 20, 1 ≤ x1, y1, x2, y2 ≤ N). Левая верхняя клетка доски имеет координаты (1, 1), правая нижняя — (N, N).
// Формат вывода
// В первой строке выведите единственное число K — наименьшее необходимое число ходов коня. В каждой из следующих K+1 строк должно быть записано 2 числа — координаты очередной клетки в пути коня. 


#include <iostream>
#include <queue>
#include <vector>

struct Ver {
    int x;
    int y;
};

class Graph {
public:
    explicit Graph(int size) : distance_(size), parent_(size) {
        for (int i = 0; i < size; ++i) {
            distance_[i].resize(size, MAX_DISTANCE_);
            parent_[i].resize(size);
        }
    };
    void Bfs(Ver &ver);
    int GetDistance(Ver &ver);
    void PrintWay(Ver & ver);

private:
    const int MAX_DISTANCE_ = 50001;
    std::vector<std::vector<int>> distance_;
    std::vector<std::vector<Ver>> parent_;
};

void Graph::Bfs(Ver &ver) {
    std::queue<Ver> queue_graph;
    queue_graph.push(ver);
    distance_[ver.x][ver.y] = 0;
    Ver parent_ver = {-1, -1};
    parent_[ver.x][ver.y] = parent_ver;
    while(!queue_graph.empty()) {
        Ver new_ver = queue_graph.front();
        queue_graph.pop();
        int index_i = new_ver.x;
        int index_j = new_ver.y;
        if (index_i >= 2) {
            if (index_j >= 1) {
                if (distance_[index_i - 2][index_j - 1] == MAX_DISTANCE_) {
                    distance_[index_i - 2][index_j - 1] = distance_[index_i][index_j] + 1;
                    parent_[index_i - 2][index_j - 1] = new_ver;
                    queue_graph.push({index_i - 2, index_j - 1});
                }
            }
            if (index_j < distance_.size() - 1) {
                if (distance_[index_i - 2][index_j + 1] == MAX_DISTANCE_) {
                    distance_[index_i - 2][index_j + 1] = distance_[index_i][index_j] + 1;
                    parent_[index_i - 2][index_j + 1] = new_ver;
                    queue_graph.push({index_i - 2, index_j + 1});
                }
            }
        }
        if (index_i < distance_.size() - 2) {
            if (index_j >= 1) {
                if (distance_[index_i + 2][index_j - 1] == MAX_DISTANCE_) {
                    distance_[index_i + 2][index_j - 1] = distance_[index_i][index_j] + 1;
                    parent_[index_i + 2][index_j - 1] = new_ver;
                    queue_graph.push({index_i + 2, index_j - 1});
                }
            }
            if (index_j < distance_.size() - 1) {
                if (distance_[index_i + 2][index_j + 1] == MAX_DISTANCE_) {
                    distance_[index_i + 2][index_j + 1] = distance_[index_i][index_j] + 1;
                    parent_[index_i + 2][index_j + 1] = new_ver;
                    queue_graph.push({index_i + 2, index_j + 1});
                }
            }
        }
        if (index_j >= 2) {
            if (index_i >= 1) {
                if (distance_[index_i - 1][index_j - 2] == MAX_DISTANCE_) {
                    distance_[index_i - 1][index_j - 2] = distance_[index_i][index_j] + 1;
                    parent_[index_i - 1][index_j - 2] = new_ver;
                    queue_graph.push({index_i - 1, index_j - 2});
                }
            }
            if (index_i < distance_.size() - 1) {
                if (distance_[index_i + 1][index_j - 2] == MAX_DISTANCE_) {
                    distance_[index_i + 1][index_j - 2] = distance_[index_i][index_j] + 1;
                    parent_[index_i + 1][index_j - 2] = new_ver;
                    queue_graph.push({index_i + 1, index_j - 2});
                }
            }
        }
        if (index_j < distance_.size() - 2) {
            if (index_i >= 1) {
                if (distance_[index_i - 1][index_j + 2] == MAX_DISTANCE_) {
                    distance_[index_i - 1][index_j + 2] = distance_[index_i][index_j] + 1;
                    parent_[index_i - 1][index_j + 2] = new_ver;
                    queue_graph.push({index_i - 1, index_j + 2});
                }
            }
            if (index_i < distance_.size() - 1) {
                if (distance_[index_i + 1][index_j + 2] == MAX_DISTANCE_) {
                    distance_[index_i + 1][index_j + 2] = distance_[index_i][index_j] + 1;
                    parent_[index_i + 1][index_j + 2] = new_ver;
                    queue_graph.push({index_i + 1, index_j + 2});
                }
            }
        }
    }
}

int Graph::GetDistance(Ver &ver) {
    return  distance_[ver.x][ver.y];
}

void Graph::PrintWay(Ver &ver) {
    Ver parent_ver = {-1, -1};
    if (parent_[ver.x][ver.y].x == parent_ver.x && parent_[ver.x][ver.y].y == parent_ver.y) {
        std::cout << ver.x + 1 << " " << ver.y+ 1 << "\n";
        return;
    }
    PrintWay(parent_[ver.x][ver.y]);
    std::cout << ver.x + 1 << " " << ver.y + 1 << "\n";
}

int main() {
    int n = 0;
    std::cin >> n;
    Graph graph(n);
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    std::cin >> x1 >> y1 >> x2 >> y2;
    Ver start_ver = {--x1, --y1};
    Ver end_ver = {--x2, --y2};
    graph.Bfs(start_ver);
    int dist = graph.GetDistance(end_ver);
    std::cout << dist << "\n";
    graph.PrintWay(end_ver);
    return 0;
}
