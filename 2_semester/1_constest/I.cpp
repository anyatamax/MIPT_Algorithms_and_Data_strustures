//Быстрая сортировка
//Какой русский не любит быстрой сортировки? Реализуйте её.
//Формат ввода
//В первой строке дано число N (1 ≤ N ≤ 10^5). Далее во второй строке идет N чисел через пробел, числа не превосходят по модулю 10^9.
//Формат вывода
//Вывести отсортированный по неубыванию массив.

#include<iostream>

void qSort(int nStart, int nEnd, int* mas) {
    int left, right, elem;
    if (nStart >= nEnd) {
        return;
    }
    left = nStart;
    right = nEnd;
    elem = mas[(right - left) / 2 + left];
    while (left <= right) {
        while (mas[left] < elem) {
            left++;
        }
        while (mas[right] > elem) {
            right--;
        }
        if (left <= right) {

            std::swap(mas[left], mas[right]);
            left++;
            right--;
        }
    }
    qSort(nStart, right, mas);
    qSort(left, nEnd, mas);
}

int main() {
    int size;
    std::cin >> size;
    int* mas = new int[size];
    for (int i = 0; i < size; i++) {
        std::cin >> mas[i];
    }
    qSort(0, size - 1, mas);
    for (int i = 0; i < size; i++) {
        std::cout << mas[i] << " ";
    }
    delete[] mas;
    return 0;
}
