// Отсортируйте K отсортированных массивов в один очень большой отсортированный массив. Пользоваться архивами прошлого семестра запрещено, вам же не зря новые знания дают.

// Формат ввода
// В первой строке идет число K (1 ≤ K ≤ 1000). Далее идет K блоков — описания массивов для слияния в следующем формате.
// На первой строке идет число Ni — размер i-го массива. На второй строке через пробел идут Ni целых чисел, не превосходящих по модулю 109.
// Формат вывода
// Выведите S целых чисел в порядке неубывания — массив, полученный в результате слияния K отсортированных массивов. 

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
        if (*first_beg > *second_beg) {
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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::vector<int> sorted_mas;
    std::vector<int>::iterator first_end;
    std::vector<int>::iterator second_beg;
    int n;
    int sum_size = 0;
    std::cin >> n;
    int size;
    std::cin >> size;
    sum_size += size;
    for (size_t j = 0; j < size; j++) {
        int elem;
        std::cin >> elem;
        sorted_mas.push_back(elem);
    }
    for (size_t i = 1; i < n; i++) {
        int size_next;
        std::cin >> size_next;
        for (size_t j = 0; j < size_next; j++) {
            int elem;
            std::cin >> elem;
            sorted_mas.push_back(elem);
        }
        first_end = sorted_mas.begin();
        second_beg = sorted_mas.begin();
        for (size_t j = 0; j < sum_size; j++) {
            ++first_end;
            ++second_beg;
        }
        ImplaceMergeSort(sorted_mas.begin(), first_end, second_beg, sorted_mas.end());
        sum_size += size_next;
    }
    for (size_t i = 0; i < sorted_mas.size(); i++) {
        std::cout << sorted_mas[i] << " ";
    }
    return 0;
}
