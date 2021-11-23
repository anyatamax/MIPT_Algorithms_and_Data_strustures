// После успешных учений генерал построил своих солдат в шеренгу, чтобы определить лучший взвод (подотрезок шеренги). Качество взвода генерал определяет по числу наград у солдата с наибольшим их количеством. Учения прошли настолько успешно, что периодически генерал дает новые приказы о награждении того или иного взвода дополнительным числом наград.
// Необходимо помочь генералу определить качество каждого взвода и разобраться в своих приказах.

// Формат ввода
// В первой строке вводится одно натуральное число N (1 ≤ N ≤ 100000) — количество солдат в шеренге.
// Во второй строке вводятся N чисел от 0 до 100000 — число наград на момент построения.
// В третьей строке вводится одно натуральное число M (1 ≤ M ≤ 30000) — количество запросов генерала.
// Каждая из следующих M строк представляет собой описание запроса. Сначала вводится одна буква, кодирующая вид запроса (m — найти наибольшее число наград во взводе, a — увеличить число наград у взвода).
// Следом за m вводятся два числа — левая и правая граница шеренги.
// Следом за a вводятся три числа — левый и правый концы шеренги и число add, на которое нужно увеличить количество наград у каждого солдата данного взвода (0 ≤ add ≤ 100000).
// Формат вывода
// Выведите в одну строку через пробел ответы на каждый запрос m. 


#include <iostream>
#include <vector>

class SegmentTree {
public:
    explicit SegmentTree(std::vector<long int>& arr);
    void Update(int root, int t_left, int t_right, int left, int right, long int delta);
    long int GetSum(int root, int t_left, int t_right, int left, int right);
    void Push(int root);
    int Size() const {
        return size_;
    }

private:
    std::vector<long int> st_;
    std::vector<long int> st_promise_;
    int arr_size_;
    int size_;
};

SegmentTree::SegmentTree(std::vector<long int> &arr) {
    int deg = 0;
    while ((1 << deg) < arr.size()) {
        ++deg;
    }
    size_ = (1 << deg);
    arr_size_ = arr.size();
    st_.resize(2 * size_, 0);
    st_promise_.resize(2 * size_, 0);
    for (int i = size_; i < size_ + arr.size(); i++) {
        st_[i] = arr[i - size_];
    }
    for (int i = size_ - 1; i > 0; --i) {
        st_[i] = std::max(st_[i * 2], st_[i * 2 + 1]);
    }
}

long int SegmentTree::GetSum(int root, int t_left, int t_right, int left, int right) {
    if (t_right < left || right < t_left || t_left > t_right) {
        return 0;
    }
    if (t_left >= left && t_right <= right) {
        return st_[root] + st_promise_[root];
    }
    Push(root);
    long int left_res = GetSum(root * 2, t_left, (t_left + t_right) / 2, left, right);
    long int right_res = GetSum(root * 2 + 1, (t_left + t_right) / 2 + 1, t_right, left, right);
    return std::max(left_res, right_res);
}

void SegmentTree::Update(int root, int t_left, int t_right, int left, int right, long int delta) {
    if (t_right < left || right < t_left || t_left > t_right) {
        return;
    }
    if (t_left >= left && t_right <= right) {
        st_promise_[root] += delta;
        return;
    }
    Push(root);
    Update(root * 2, t_left, (t_left + t_right) / 2, left, right, delta);
    Update(root * 2 + 1, (t_left + t_right) / 2 + 1, t_right, left, right, delta);
    st_[root] = std::max(st_[root * 2] + st_promise_[root * 2], st_[root * 2 + 1] + st_promise_[root * 2 + 1]);
}

void SegmentTree::Push(int root) {
    if (root * 2 < size_ + arr_size_) {
        st_promise_[root * 2] += st_promise_[root];
    }
    if (root * 2 + 1 < size_ + arr_size_) {
        st_promise_[root * 2 + 1] += st_promise_[root];
    }
    st_[root] += st_promise_[root];
    st_promise_[root] = 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    char task = 0;
    int n = 0;
    int count_tasks = 0;
    int index_l = 0;
    int index_r = 0;
    long int medals = 0;
    std::cin >> n;
    std::vector<long int> army(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> army[i];
    }
    SegmentTree seg_t(army);
    std::cin >> count_tasks;
    for (int i = 0; i < count_tasks; ++i) {
        std::cin >> task;
        if (task == 'm') {
            std::cin >> index_l >> index_r;
            std::cout << seg_t.GetSum(1, 0, seg_t.Size() - 1, index_l - 1, index_r - 1) << " ";
        } else {
            std::cin >> index_l >> index_r >> medals;
            seg_t.Update(1, 0, seg_t.Size() - 1, index_l - 1, index_r - 1, medals);
        }
    }
    return 0;
}
