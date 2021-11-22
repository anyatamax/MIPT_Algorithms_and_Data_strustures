// В аудитории есть N парт, расположенных вдоль прямой. Необходимо рассадить за них K студентов. Но, так как социальную дистанцию никто не отменял, необходимо максимизировать минимальное расстояние между студентами.

// Формат ввода
// В первой строке идут числа N (3 ≤ N ≤ 10^4) — количество парт и K (1 < K < N) — количество студентов. На следующей строке через пробел идут N натуральных чисел в порядке возрастания — координаты парт на прямой, не превосходящие 109.
// Формат вывода
// Выведите одно число — максимально возможное минимальное расстояние между студентами.

#include <iostream>

bool Correct(int x, int *mas, int size, int k) {
    int count = 1;
    int last_student = mas[0];
    for (int i = 0; i < size; i++) {
        if (mas[i] - last_student >= x) {
            count++;
            last_student = mas[i];
        }
    }
    return (count >= k);
}

int main() {
    int n, k;
    std::cin >> n >> k;
    int *mas = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> mas[i];
    }
    int left = 0;
    int right = mas[n - 1] - mas[0] + 1;
    while (right - left != 1) {
        int mid = (left + right) / 2;
        if (Correct(mid, mas, n, k)) {
            left = mid;
        } else {
            right = mid;
        }
    }
    std::cout << left;
    delete[] mas;
    return 0;
}
