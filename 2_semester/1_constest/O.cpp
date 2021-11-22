// Реализуйте алгоритм бинарного поиска.

// Формат ввода
// В первой строке входных данных содержатся натуральные числа N и K (0<N,K≤ 10^5). Во второй строке задаются N элементов первого массива, отсортированного по возрастанию, а в третьей строке — K элементов второго массива. Элементы обоих массивов - целые числа, каждое из которых по модулю не превосходит 10^9.
// Формат вывода
// Требуется для каждого из K чисел вывести в отдельную строку «YES», если это число встречается в первом массиве, и «NO» в противном случае. 

#include<iostream>


bool bin_search(int, int, int, int*);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n, k, num;
    std::cin >> n >> k;
    int* mas = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> mas[i];
    }
    for (int i = 0; i < k; i++) {
        std::cin >> num;
        if (num > mas[n - 1] || num < mas[0]) {
            std::cout << "NO\n";
        } else if (num > mas[n / 2]) {
            if (bin_search(n / 2, n, num, mas)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }
        } else if (bin_search(0, n / 2, num, mas)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    delete[] mas;
    return 0;
}

bool bin_search(int left, int right, int num, int* mas) {
    while (left <= right) {
        int mid_one = (left * 2 + right) / 3;
        int mid_two = (left + right * 2) / 3;
        if (num < mas[mid_one]) {
            right = mid_one - 1;
        }
        else if (num > mas[mid_one] && num < mas[mid_two]) {
            left = mid_one + 1;
            right = mid_two - 1;
        } else if (num > mas[mid_two]) {
            left = mid_two + 1;
        } else if (num == mas[mid_one] || num == mas[mid_two]) {
            return true;
        }
    }
    return false;
}
