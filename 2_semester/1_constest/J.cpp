// Вам даны два отсортированных массива. Нужно слить их за линейное время в один большой отсортированный.
// Формат ввода
// В первой строке дано число N (1 ≤ N ≤ 5 ⋅ 10^5) — длина первого массива. На второй строке идет N целых чисел, не превосходящих по модулю 10^9 через пробел.
// В третьей строке дано число M (1 ≤ M ≤ 5 ⋅ 10^5) — длина второго массива. На четвертой строке идет M целых чисел, не превосходящих по модулю 10^9 через пробел.
// Формат вывода
// Вывести M + N чисел через пробел в порядке неубывания.

#include<iostream>

int* merge(int* left, size_t size_l, int* right, size_t size_r) {
    size_t i = 0;
    size_t j = 0;
    int* result = new int[size_l + size_r];
    for (size_t k = 0; k < size_l + size_r; k++) {
        if (i >= size_l) {
            result[k] = right[j];
            j++;
        }
        else if (j >= size_r) {
            result[k] = left[i];
            i++;
        }
        else {
            if (left[i] < right[j]) {
                result[k] = left[i];
                i++;
            }
            else {
                result[k] = right[j];
                j++;
            }
        }
    }
    return result;
}

int main() {
    int size_mas, size_mas_other;
    std::cin >> size_mas;
    int* mas = new int[size_mas];
    for (int i = 0; i < size_mas; i++) {
        std::cin >> mas[i];
    }
    std::cin >> size_mas_other;
    int* mas_other = new int[size_mas_other];
    for (int i = 0; i < size_mas_other; i++) {
        std::cin >> mas_other[i];
    }
    int* result = new int[size_mas + size_mas_other];
    result = merge(mas, size_mas, mas_other, size_mas_other);
    for (int i = 0; i < size_mas + size_mas_other; i++) {
        std::cout << result[i] << " ";
    }
    delete[] mas;
    delete[] mas_other;
    delete[] result;
    return 0;
}
