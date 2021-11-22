// Ваша цель — реализовать стабильную сортировку для следующей задачи.
// Выведите фамилии и имена абитуриентов, подавших документы на поступление на ПМИ ФПМИ в порядке убывания их среднего балла по ЕГЭ.
// Про каждого ученика известны их фамилии, имена и баллы ЕГЭ по следующим предметам: информатика, математика и русский язык.
// Да, легенда та же самая, но это на значит, что можно просто копипастить код из прошлой задачи. Опять же вас ждут два нововведения:
//     [-] Нельзя использовать дополнительную память при сортировке.
//     [+] Можно писать сортировку за O(N(logN)^2)

// Формат ввода
// В первой строке идет число N (1 ≤ N ≤ 10^5) — количество абитуриентов, подавших документы.
// Далее идет N строк — описания ученика в формате «surname name inf_points math_points rus_points», где «surname» и «name» — строки, а «*_points» — баллы за экзамены (целые числа от нуля до ста включительно).
// Формат вывода
// Необходимо вывести пары фамилия-имя по одной на строке, разделяя фамилию и имя одним пробелом. Выводить баллы не нужно. Если несколько учащихся имеют одинаковые средние баллы, то их нужно выводить в порядке, заданном во входных данных. 

#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

class Students {
public:
    char name[100];
    char surname[100];
    int inf;
    int math;
    int rus;
    int sum;
};

void Swap(Students &a, Students &b) {
    std::swap(a.name, b.name);
    std::swap(a.surname, b.surname);
    std::swap(a.inf, b.inf);
    std::swap(a.math, b.math);
    std::swap(a.rus, b.rus);
    std::swap(a.sum, b.sum);
}

bool operator==(const Students &a, const Students &b) {
    return (a.sum == b.sum);
}
bool operator<(const Students &a, const Students &b) {
    return (a.sum < b.sum);
}
bool operator!=(const Students &a, const Students &b) {
    return (a.sum != b.sum);
}
bool operator>(const Students &a, const Students &b) {
    return (a.sum > b.sum);
}
bool operator<=(const Students &a, const Students &b) {
    return (a.sum <= b.sum);
}
bool operator>=(const Students &a, const Students &b) {
    return (a.sum >= b.sum);
}

void ImplaceMergeSort(std::vector<Students>::iterator first_beg, std::vector<Students>::iterator first_end, std::vector<Students>::iterator second_beg, std::vector<Students>::iterator second_end);
void MergeSort(std::vector<Students> &student, std::vector<Students>::iterator beg, std::vector<Students>::iterator end);

bool Compare_l(Students a, Students b) {
    return b < a;
}

void MergeSort(std::vector<Students> &student, std::vector<Students>::iterator beg, std::vector<Students>::iterator end) {
    if (std::distance(beg, end) <= 1) {
        return;
    }
    size_t mid = std::distance(beg, end) / 2;
    auto middle = beg;
    for (size_t i = 0; i < mid; i++) {
        if (middle != end) {
            ++middle;
        }
    }
    MergeSort(student, beg, middle);
    MergeSort(student, middle, end);
    ImplaceMergeSort(beg, middle, middle, end);
}

void ImplaceMergeSort(std::vector<Students>::iterator first_beg, std::vector<Students>::iterator first_end, std::vector<Students>::iterator second_beg, std::vector<Students>::iterator second_end) {
    size_t first_size = std::distance(first_beg, first_end);
    size_t second_size = second_end - second_beg;
    if (first_size == 0 || second_size == 0) {
        return;
    }
    if (first_size == 1 && second_size == 1) {
        if (first_beg->sum < second_beg->sum) {
            Swap(*first_beg, *second_beg);
        }
        return;
    }
    auto B1_beg = first_beg;
    std::vector<Students>::iterator B1_end;
    std::vector<Students>::iterator B2_beg;
    auto B2_end = first_end;
    auto B3_beg = second_beg;
    std::vector<Students>::iterator B3_end;
    std::vector<Students>::iterator B4_beg;
    auto B4_end = second_end;
    if (first_size >= second_size) {
        B1_end = first_beg + first_size / 2;
        B2_beg = B1_end;
        auto sep = std::lower_bound(second_beg, second_end, *B2_beg, Compare_l);
        B3_end = sep;
        B4_beg = sep;
    } else {
        B3_end = second_beg + second_size / 2;
        B4_beg = B3_end;
        auto sep = std::upper_bound(first_beg, first_end, *B4_beg, Compare_l);
        B1_end = sep;
        B2_beg = sep;
    }
    std::rotate(B2_beg, B3_beg, B3_end);
    auto B3_size = std::distance(B3_beg, B3_end);
    ImplaceMergeSort(B1_beg, B1_end, B2_beg, B2_beg + B3_size);
    ImplaceMergeSort(B2_beg + B3_size, B3_end, B4_beg, B4_end);
}

int main() {
    std::vector<Students> person;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        Students student;
        std::cin >> student.surname >> student.name >> student.inf >> student.math >> student.rus;
        student.sum = student.inf + student.math + student.rus;;
        person.push_back(student);
    }
    MergeSort(person, person.begin(), person.end());
    for (int i = 0; i < n; i++) {
        std::cout << person[i].surname << " " << person[i].name << std::endl;
    }
    return 0;
}
