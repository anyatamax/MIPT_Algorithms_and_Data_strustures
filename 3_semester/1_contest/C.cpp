// Генерал Дукар поднял свои архивные записи, чтобы посмотреть, какие взводы (думаю, вы уже запомнили из предыдущих задач, что взвод — любой подотрезок шеренги солдат) он награждал. Генерала достаточно часто посещают гениальные идеи, и этот день не стал исключением. По задумке генерала вам нужно написать программу, которая сможет обрабатывать записи из архива двух типов:
//     «a l r value» — награждение всех солдат из взвода с границами l и r (в 1-индексации) value медалями;
//     «g i» — запрос в штаб количества наград у солдата с номером i.
// Воплотите в жизнь идею генерала Дукара.

// Формат ввода
// В первой строке вводится одно натуральное число N (1 ≤ N ≤ 105) — количество солдат в шеренге.
// Во второй строке вводятся N чисел от 1 до 105 — число наград у каждого солдата на момент старта программы.
// В третьей строке вводится одно натуральное число M (1 ≤ M ≤ 5 ⋅ 105) — количество запросов в формате выше. Гарантируется, что индексы в запросах корректны, а (1 ≤ value ≤ 105) в запросах первого типа.
// Формат вывода
// Выведите ответы на запросы вида g, разделяя их переводами строк. 


#include <iostream>
#include <vector>

class SegmentTree {
public:
    explicit SegmentTree(std::vector<long int>& arr);
    void Update(int root, int t_left, int t_right, int left, int right, long int delta);
    long int GetSum(int index);
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
        st_[i] = (st_[i * 2] + st_[i * 2 + 1]);
    }

}

long int SegmentTree::GetSum(int index) {
    index += size_ - 1;
    long int medals = st_[index] + st_promise_[index];
    while (index /= 2) {
        medals += st_promise_[index];
    }
    return medals;
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
}

void SegmentTree::Push(int root) {
    if (root * 2 < size_ + arr_size_) {
        st_promise_[root * 2] += st_promise_[root];
    }
    if (root * 2 + 1 < size_ + arr_size_) {
        st_promise_[root * 2 + 1] += st_promise_[root];
    }
    st_promise_[root] = 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    char task = 0;
    int n = 0;
    int count_tasks = 0;
    int index = 0;
    int index_l = 0;
    int index_r = 0;
    long int medals = 0;
    std::cin >> n;
    std::vector<long int> army(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> army[i];
    }
    std::cin >> count_tasks;
    SegmentTree seg_t(army);
    for (int i = 0; i < count_tasks; ++i) {
        std::cin >> task;
        if (task == 'g') {
            std::cin >> index;
            std::cout << seg_t.GetSum(index) << "\n";
        } else {
            std::cin >> index_l >> index_r >> medals;
            seg_t.Update(1, 0, seg_t.Size() - 1, index_l - 1, index_r - 1, medals);
        }
    }
    return 0;
}
