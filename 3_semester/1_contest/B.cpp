// Генерал Дукар в очередной раз построил своих солдат в шеренгу. В этот день у генерала было очень плохое настроение и он решил лишить некоторые взводы части наград. Самый гуманный, по мнению генерала, способ это сделать - уменьшить число наград у каждого солдата взвода в k раз. Дукар не очень силен в дробях, поэтому для каждого взвода он хочет подобрать наибольшее k, при котором количество наград каждого солдата разделится нацело. Взводом считается любой подотрезок шеренги солдат.
// Генерал поручил вам реализовать программу, которая поможет ему в этом, иначе он разделит вас.

// Формат ввода
// В первой строке вводится одно натуральное число N (1 ≤ N ≤ 100000) — число солдат в шеренге.
// Во второй строке вводятся N чисел от 0 до 100000 — количество наград у соответствующего солдата.
// В третьей строке вводится одно натуральное число M (1 ≤ M ≤ 30000) — количество запросов.
// Каждая из следующих M строк представляет собой описание запроса. Сначала вводится одна буква, кодирующая вид запроса (s — вычислить ответ для взвода, u — изменить количество наград у солдата).
// Следом за s вводятся два числа — номера левой и правой границы шеренги.
// Следом за u вводятся два числа — номер солдата и его новое количество наград.
// Формат вывода
// Для каждого запроса s выведите результат. 


#include <iostream>
#include <numeric>
#include <vector>

class SegmentTree {
public:
    void BuildSegmentTree(std::vector<int>& arr);
    void Update(int ind, int delta);
    int GetGcd(int left, int right) const;

private:
    std::vector<int> st_;
    int size_;
};

void SegmentTree::BuildSegmentTree(std::vector<int> &arr) {
    int deg = 0;
    while ((1 << deg) < arr.size()) {
        ++deg;
    }
    size_ = (1 << deg);
    st_.resize(2 * size_, 0);
    for (int i = size_; i < size_ + arr.size(); i++) {
        st_[i] = arr[i - size_];
    }
    for (int i = size_ - 1; i > 0; --i) {
        st_[i] = std::gcd(st_[i * 2], st_[i * 2 + 1]);
    }
}

int SegmentTree::GetGcd(int left, int right) const {
    left += size_ - 1;
    right += size_ - 1;
    int left_res = 0;
    int right_res = 0;
    while (left < right) {
        if ((left & 1) == 1) {
            left_res = std::gcd(left_res, st_[left]);
        }
        if ((right & 1) == 0) {
            right_res = std::gcd(right_res, st_[right]);
        }
        left = (left + 1) / 2;
        right = (right - 1) / 2;
    }
    if (left == right) {
        left_res = std::gcd(st_[left], left_res);
    }
    return std::gcd(left_res, right_res);
}

void SegmentTree::Update(int index, int delta) {
    index += size_ - 1;
    st_[index] += delta;
    while (index /= 2) {
        st_[index] = std::gcd(st_[2 * index], st_[2 * index + 1]);
    }
}

int main() {
    char task = ' ';
    int n = 0;
    int count_tasks = 0;
    int index = 0;
    int medals = 0;
    std::cin >> n;
    std::vector<int> army(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> army[i];
    }
    SegmentTree seg_t;
    seg_t.BuildSegmentTree(army);
    std::cin >> count_tasks;
    for (size_t i = 0; i < count_tasks; ++i) {
        std::cin >> task >> index >> medals;
        if (task == 's') {
            std::cout << seg_t.GetGcd(index, medals) << "\n";
        } else {
            int delta = medals - army[index - 1];
            seg_t.Update(index, delta);
            army[index - 1] = medals;
        }
    }
    return 0;
}
