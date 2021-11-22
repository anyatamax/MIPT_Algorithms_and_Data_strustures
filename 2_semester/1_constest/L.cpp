// Вам предстоит в очередной раз слить два отсортированных массива в один большой отсортированный. Чем эта задача отличается от задачи B? Тем, что у вас в решении должно быть задействовано лишь O(1) дополнительной памяти. В качестве послабления, можно делать слияние за O((N+M)log(N+M)).
// Формат ввода
// В первой строке дано число N (1 ≤ N ≤ 10^5) — длина первого массива. На второй строке идет N целых чисел, не превосходящих по модулю 10^9 через пробел.
// В третьей строке дано число M (1 ≤ M ≤ 10^5) — длина второго массива. На четвертой строке идет M целых чисел, не превосходящих по модулю 10^9 через пробел.
// Формат вывода
// Вывести M + N чисел через пробел в порядке неубывания. 

#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

void ImplaceMergeSort(std::vector<int>::iterator first_beg, std::vector<int>::iterator first_end, std::vector<int>::iterator second_beg, std::vector<int>::iterator second_end) {
    size_t first_size = std::distance(first_beg, first_end);
    size_t second_size = second_end - second_beg;
    if (first_size == 0 || second_size == 0) {
        return;
    }
    if (first_size == 1 && second_size == 1) {
        if (*first_beg> *second_beg) {
            std::swap(*first_beg, *second_beg);
        }
        return;
    }
    auto B1_beg = first_beg;
    std::vector<int>::iterator B1_end;
    std::vector<int>::iterator B2_beg;
    auto B2_end = first_end;
    auto B3_beg = second_beg;
    std::vector<int>::iterator B3_end;
    std::vector<int>::iterator B4_beg;
    auto B4_end = second_end;
    if (first_size >= second_size) {
        B1_end = first_beg + first_size / 2;
        B2_beg = B1_end;
        auto sep = std::lower_bound(second_beg, second_end, *B2_beg);
        B3_end = sep;
        B4_beg = sep;
    }
    else {
        B3_end = second_beg + second_size / 2;
        B4_beg = B3_end;
        auto sep = std::upper_bound(first_beg, first_end, *B4_beg);
        B1_end = sep;
        B2_beg = sep;
    }
    std::rotate(B2_beg, B3_beg, B3_end);
    auto B3_size = std::distance(B3_beg, B3_end);
    ImplaceMergeSort(B1_beg, B1_end, B2_beg, B2_beg + B3_size);
    ImplaceMergeSort(B2_beg + B3_size, B3_end, B4_beg, B4_end);
}

int main() {
    std::vector<int> vec;
    int n, m;
    std::cin >> n;
    std::vector<int>::iterator first_end;
    std::vector<int>::iterator second_beg;
    for (int i = 0; i < n; i++) {
        int elem;
        std::cin >> elem;
        vec.push_back(elem);
    }
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        int elem;
        std::cin >> elem;
        vec.push_back(elem);
    }
    first_end = vec.begin();
    second_beg = vec.begin();
    for (int i = 0; i < n; i ++) {
        ++first_end;
        ++second_beg;
    }
    ImplaceMergeSort(vec.begin(), first_end, second_beg, vec.end());
    for (int i = 0; i < n + m; i++) {
        std::cout << vec[i] << " ";
    }
    return 0;
}
