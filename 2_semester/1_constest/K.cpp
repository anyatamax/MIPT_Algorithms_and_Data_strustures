// Ваша цель — реализовать стабильную сортировку для следующей задачи.
// Выведите фамилии и имена абитуриентов, подавших документы на поступление на ПМИ ФПМИ в порядке убывания их среднего балла по ЕГЭ.
// Про каждого ученика известны их фамилии, имена и баллы ЕГЭ по следующим предметам: информатика, математика и русский язык.
// Формат ввода
// В первой строке идет число N (1 ≤ N ≤ 10^5) — количество абитуриентов, подавших документы.
// Далее идет N строк — описания ученика в формате «surname name inf_points math_points rus_points», где «surname» и «name» — строки, а «*_points» — баллы за экзамены (целые числа от нуля до ста включительно).
// Формат вывода
// Необходимо вывести пары фамилия-имя по одной на строке, разделяя фамилию и имя одним пробелом. Выводить баллы не нужно. Если несколько учащихся имеют одинаковые средние баллы, то их нужно выводить в порядке, заданном во входных данных. 

#include<iostream>

struct Students {
    char name[100];
    char surname[100];
    int inf;
    int math;
    int rus;
    int sum;
};

void MergeSort(Students* student, size_t left, size_t right);
void merge(Students* left, size_t size_l, Students* right, size_t size_r, Students* result);

int main() {
    int size;
    std::cin >> size;
    Students* student = new Students[size];
    for (int i = 0; i < size; i++) {
        std::cin >> student[i].surname >> student[i].name >> student[i].inf >> student[i].math >> student[i].rus;
        student[i].sum = student[i].inf + student[i].math + student[i].rus;
    }
    MergeSort(student, 0, size);
    for (int i = 0; i < size; i++) {
        std::cout << student[i].surname << " " << student[i].name << std::endl;
    }
    delete[] student;
    return 0;
}

void merge(Students* left, size_t size_l, Students* right, size_t size_r, Students* result) {
    size_t i = 0;
    size_t j = 0;
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
            if (left[i].sum >= right[j].sum) {
                result[k] = left[i];
                i++;
            }
            else {
                result[k] = right[j];
                j++;
            }
        }
    }
}

void MergeSort(Students* student, size_t left, size_t right) {
    if (right - left <= 1) {
        return;
    }
    size_t mid = (right + left) / 2;
    MergeSort(student, left, mid);
    MergeSort(student, mid, right);
    Students* result = new Students[right - left];
    merge(student + left, (mid - left), (student + mid), (right - mid), result);
    for (size_t i = left; i < right; i++) {
        student[i] = result[i - left];
    }
    delete[] result;
}
