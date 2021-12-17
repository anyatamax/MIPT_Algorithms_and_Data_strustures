

// Генерал Дукар в очередной (надеемся, последний) раз приказал своей армии построиться. Но в этой задаче построение будет не в одну шеренгу, а в прямоугольник. Для сложнейшей операции ему необходимо во взводах найти самого солдата с наименьшим количеством нарушений. Так как построение и порядки в армии генерала особенные, взводом может быть любой подпрямоугольник солдат. Вам нужно помочь генералу для каждого взвода, что он спросит, найти самого непроштрафившегося солдата.

// Формат ввода
// Первая строка входного файла содержит числа m и n — размеры прямоугольника (1 ≤ m, n ≤ 500). Следующие m строк содержат по n целых чисел каждая — количество нарушений определенного солдата. Все числа между -2^31 и 2^31 - 1.
// Далее следует q — количество запросов генерала (1 ≤ q ≤ 2 ⋅ 10^5). Следующие q строк содержат по четыре целых числа: x1, y1, x2, y2 (1 ≤ x1 ≤ x2 ≤ m, 1 ≤ y1 ≤ y2 ≤ n) — границы взвода в запросе.
// Формат вывода
// Выведите q чисел — для каждого запроса выведите минимальное значение в соответствующем прямоугольнике. 


#include <iostream>
#include <vector>

class SegmentTree {
public:
    const int MAX_VALUE = 2147483647;
    void BuildSegmentTree(int m, int n);
    void BuildY(std::vector<std::vector<int>> &matr, int root, int left, int right, int right_x);
    void Stp(std::vector<int> &ver, int root, int left, int right, int vleft, int vright) const;
    void Result(std::vector<int>& ver, int root, int x_left, int x_right, int left, int right, int &ans);

private:
    std::vector<std::vector<int>> tree_;

    void BuildX(std::vector<int>& arr, int tree, int root, int left, int right);
    std::vector<int> Connect(std::vector<int> &lhs, std::vector<int> &rhs) const;
};

void SegmentTree::BuildSegmentTree(int m, int n) {
    tree_.resize(4 * m);
    for (int i = 0; i < 4 * m; ++i) {
        tree_[i].resize(4 * n, MAX_VALUE);
    }
}

void SegmentTree::BuildX(std::vector<int>& arr, int tree, int root, int left, int right) {
    if (left == right - 1) {
        tree_[tree][root] = arr[left];
    } else {
        int m = (left + right) / 2;
        BuildX(arr, tree, 2 * root + 1, left , m);
        BuildX(arr, tree, 2 * root + 2, m, right);
        tree_[tree][root] = std::min(tree_[tree][2 * root + 1], tree_[tree][2 * root + 2]);
    }
}

std::vector<int> SegmentTree::Connect(std::vector<int> &lhs, std::vector<int> &rhs) const {
    std::vector<int> c(lhs.size());
    for (int i = 0; i < lhs.size(); ++i) {
        c[i] = std::min(lhs[i], rhs[i]);
    }
    return c;
}

void SegmentTree::BuildY(std::vector<std::vector<int>> &matr, int root, int left, int right, int right_x) {
    if (left == right - 1) {
        BuildX(matr[left], root, 0, 0, right_x);
    } else {
        int m = (left + right) / 2;
        BuildY(matr, 2 * root + 1, left, m, right_x);
        BuildY(matr, 2 * root + 2, m, right, right_x);
        tree_[root] = Connect(tree_[2 * root + 1], tree_[2 * root + 2]);
    }
}

void SegmentTree::Stp(std::vector<int> &ver, int root, int left, int right, int vleft, int vright) const {
    if (left >= vleft && right <= vright) {
        ver.push_back(root);
    } else if (left >= vright || right <= vleft) {
        return;
    } else {
        int m = (left + right) / 2;
        Stp(ver, 2 * root + 1, left, m, vleft, std::min(m, vright));
        Stp(ver, 2 * root + 2, m, right, std::max(m, vleft), vright);
    }
}

void SegmentTree::Result(std::vector<int>& ver, int root, int x_left, int x_right, int left, int right, int &ans) {
    if (left >= x_left && right <= x_right) {
        for (auto it : ver) {
            ans = std::min(tree_[root][it], ans);
        }
    } else if (left >= x_right || right <= x_left) {
        return;
    } else {
        int m = (right + left) / 2;
        Result(ver, 2 * root + 1, x_left, std::min(m, x_right), left, m, ans);
        Result(ver, 2 * root + 2, std::max(m, x_left), x_right, m, right, ans);
    }
}

int main() {
    int n = 0;
    int m = 0;
    std::cin >> m >> n;
    std::vector<std::vector<int>> matrix(m, std::vector<int>(n));
    SegmentTree tree;
    tree.BuildSegmentTree(m, n);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int elem = 0;
            std::cin >> elem;
            matrix[i][j] = elem;
        }
    }
    tree.BuildY(matrix, 0, 0, m, n);
    int count_task = 0;
    std::cin >> count_task;
    int left_first = 0;
    int left_second = 0;
    int right_first = 0;
    int right_second = 0;
    for (int i = 0; i < count_task; ++i) {
        std::cin >> left_first >> right_first >> left_second >> right_second;
        std::vector<int> ver;
        tree.Stp(ver, 0, 0, n, right_first - 1, right_second);
        int ans = tree.MAX_VALUE;
        tree.Result(ver, 0, left_first - 1, left_second, 0, m, ans);
        std::cout << ans << "\n";
    }
}
