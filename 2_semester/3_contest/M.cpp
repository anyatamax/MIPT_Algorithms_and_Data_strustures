// В этой задаче необходимо организовать неориентированный граф, на котором поддерживаются следующие операции:
//     AddEdge(u, v) — добавить в граф ребро между вершинами (u, v).
//     Vertex(u) — вывести список вершин, смежных с вершиной u.
// Петель и кратных ребер в графе нет. Для решения данной задачи используйте последовательные контейнеры из STL.

// Формат ввода
// В первой строке входного файла содержится целое число N (1 ≤ N ≤ 10^6) — количество вершин в графе.
// В следующей строке находится целое числоK (0 ≤ K ≤ 10^6) — число операций, затем идет описание операций — каждое в своей строке.
// Операции имеют следующий формат: «1 u v » или «2 u », обозначающие соответственно операции AddEdge(u, v) и Vertex(u).
// Гарантируется, что суммарное количество чисел, которое будет необходимо вывести при выполнении всех операций Vertex не превосходит 2⋅ 10^5.
// Формат вывода
// В выходной файл для каждой команды Vertex необходимо на отдельной строке вывести список смежных вершин указанной вершины. Вершины списка смежности нужно выводить в порядке добавления соответствующих ребер в граф.


#include <algorithm>
#include <iostream>
#include <vector>

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int size, tasks;
    std::cin >> size >> tasks;
    std::vector<std::vector<int>> graph(size + 1);
    for (size_t i = 0; i < tasks; ++i) {
        int task;
        std::cin >> task;
        if (task == 1) {
            int first_node, second_node;
            std::cin >> first_node >> second_node;
            graph[first_node].push_back(second_node);
            graph[second_node].push_back(first_node);
        } else {
            int value;
            std::cin >> value;
            for (size_t j = 0; j < graph[value].size(); ++j) {
                std::cout << graph[value][j] << " ";
            }
            std::cout << "\n";
        }
    }
    return 0;
}
