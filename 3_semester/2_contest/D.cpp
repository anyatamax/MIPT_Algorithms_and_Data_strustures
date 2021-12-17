

// Витя хочет придумать новую игру с числами. В этой игре от игроков требуется преобразовывать четырехзначные числа не содержащие нулей при помощи следующего разрешенного набора действий:

//     Можно увеличить первую цифру числа на 1, если она не равна 9.
//     Можно уменьшить последнюю цифру на 1, если она не равна 1.
//     Можно циклически сдвинуть все цифры на одну вправо.
//     Можно циклически сдвинуть все цифры на одну влево.

// Например, применяя эти правила к числу 1234 можно получить числа 2234, 1233, 4123 и 2341 соответственно. Точные правила игры Витя пока не придумал, но пока его интересует вопрос, как получить из одного числа другое за минимальное количество операций.

// Формат ввода
// Во входном файле содержится два различных четырехзначных числа, каждое из которых не содержит нулей.
// Формат вывода
// В первой строке вывода должна быть длина оптимальной последовательности чисел, которая описана ниже.
// На второй строке необходимо вывести последовательность четырехзначных чисел, не содержащих нулей. Последовательность должна начинаться первым из данных чисел и заканчиваться вторым из данных чисел, каждое последующее число в последовательности должно быть получено из предыдущего числа применением одного из правил. Количество чисел в последовательности должно быть минимально возможным.


#include <iostream>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int size) : distance_(size, MAX_DISTANCE_), parent_(size) {};
    void Bfs(int start, int end);
    int GetDistance(int ver);
    void PrintWay(int ver);
    int AddFirst(int number) const;
    int MinusLast(int number) const;
    int ShiftLeft(int number) const;
    int ShiftRight(int number) const;

private:
    const int MAX_DISTANCE_ = 50001;
    const int FIRST_DIGITAL = 1000;
    const int LAST_DIGITAL = 10;
    std::vector<int> distance_;
    std::vector<int> parent_;
};

int Graph::AddFirst(int number) const {
    if (number / FIRST_DIGITAL != 9) {
        return number + FIRST_DIGITAL;
    }
    return number;
}
int Graph::MinusLast(int number) const {
    if (number % LAST_DIGITAL != 1) {
        return number - 1;
    }
    return number;
}
int Graph::ShiftLeft(int number) const {
    return (number % FIRST_DIGITAL) * LAST_DIGITAL + number / FIRST_DIGITAL;
}
int Graph::ShiftRight(int number) const {
    return (number % LAST_DIGITAL) * FIRST_DIGITAL + number / LAST_DIGITAL;
}

void Graph::Bfs(int start, int end) {
    std::queue<int> queue_graph;
    queue_graph.push(start);
    distance_[start] = 1;
    parent_[start] = -1;
    while(!queue_graph.empty()) {
        int new_ver = queue_graph.front();
        queue_graph.pop();
        if (new_ver == end) {
            break;
        }
        int new_number = 0;
        new_number = AddFirst(new_ver);
        if (distance_[new_number] == MAX_DISTANCE_) {
            distance_[new_number] = distance_[new_ver] + 1;
            parent_[new_number] = new_ver;
            queue_graph.push(new_number);
        }
        new_number = MinusLast(new_ver);
        if (distance_[new_number] == MAX_DISTANCE_) {
            distance_[new_number] = distance_[new_ver] + 1;
            parent_[new_number] = new_ver;
            queue_graph.push(new_number);
        }
        new_number = ShiftLeft(new_ver);
        if (distance_[new_number] == MAX_DISTANCE_) {
            distance_[new_number] = distance_[new_ver] + 1;
            parent_[new_number] = new_ver;
            queue_graph.push(new_number);
        }
        new_number = ShiftRight(new_ver);
        if (distance_[new_number] == MAX_DISTANCE_) {
            distance_[new_number] = distance_[new_ver] + 1;
            parent_[new_number] = new_ver;
            queue_graph.push(new_number);
        }
    }
}

int Graph::GetDistance(int ver) {
    return  distance_[ver];
}

void Graph::PrintWay(int ver) {
    if (parent_[ver] == -1) {
        std::cout << ver << "\n";
        return;
    }
    PrintWay(parent_[ver]);
    std::cout << ver << "\n";
}

int main() {
    int start = 0;
    int end = 0;
    std::cin >> start >> end;
    Graph graph(10000);
    graph.Bfs(start, end);
    int dist = graph.GetDistance(end);
    std::cout << dist << "\n";
    graph.PrintWay(end);
    return 0;
}
