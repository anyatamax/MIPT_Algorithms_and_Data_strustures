// Генерал Дукар готовится к битве, продумывая лучшую расстановку войск. По мнению генерала, расстановка будет наилучшей, если все его силы будут расположены вдоль одной прямой. Теперь Дукар ищет, а как лучше распределить силы вдоль линии фронта. Он попросил вас, как лучшего программиста армии генерала Дукара, написать программу, которая сможет отвечать на два типа запросов:
//     «s l r» — узнать суммарную мощь сил, расположенных на отрезке [l, r] в 1-индексации;
//     «u i v» — перераспределить силы армии так, чтобы в позиции с индексом i мощь стала v. Считайте, что он просто заменяет мощь на позиции i на мощь v. Не в ваших интересах задумываться о плане генерала и о том, как он это делает.

// Формат ввода
// В первой строке вводится одно натуральное число N (1 ≤ N ≤ 10^5) — длина линии фронта.
// Во второй строке вводятся N чисел от 0 до 10^5 — изначальная расстановка сил в армии генерала Дукара.
// В третьей строке вводится одно натуральное число M (1 ≤ M ≤ 3 ⋅ 10^4) — количество запросов.
// Каждая из следующих M строк представляет собой описание запроса в формате выше.
// Формат вывода
// Для каждого запроса s выведите результат. Все числа выводите в одну строку через пробел. 


#include <iostream>
#include <vector>

class FenwickTree {
public:
    void BuildFenwickTree(std::vector<long int>& arr);
    long int Sum(long int index) const;
    long int GetSum(long int left, long int right) const;
    void Update(int index, long int delta);

private:
    std::vector<long int> ft_;
};

long int FenwickTree::Sum(long int index) const {
    if (index < 0) {
        return 0;
    }
    long int res = 0;
    for (size_t i = index; i + 1 > 0; i = (i & (i + 1)) - 1) {
        res += ft_[i];
    }
    return res;
}

void FenwickTree::Update(int index, long int delta) {
    for (size_t i = index; i < ft_.size(); i |= (i + 1)) {
        ft_[i] += delta;
    }
}

void FenwickTree::BuildFenwickTree(std::vector<long int> &arr) {
    ft_.resize(arr.size(), 0);
    for (size_t i = 0; i < ft_.size(); ++i) {
        Update(i, arr[i]);
    }
}

long int FenwickTree::GetSum(long int left, long int right) const {
    return Sum(right) - Sum(left - 1);
}

int main() {
    char task = ' ';
    int n = 0;
    int count_tasks = 0;
    int index = 0;
    long int power = 0;
    std::cin >> n;
    std::vector<long int> army(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> army[i];
    }
    FenwickTree fenw_t;
    fenw_t.BuildFenwickTree(army);
    std::cin >> count_tasks;
    for (int i = 0; i < count_tasks; ++i) {
        std::cin >> task >> index >> power;
        if (task == 's') {
            std::cout << fenw_t.GetSum(index - 1, power - 1) << " ";
        } else {
            long int delta = power - army[index - 1];
            fenw_t.Update(index - 1, delta);
            army[index - 1] = power;
        }
    }
    return 0;
}
