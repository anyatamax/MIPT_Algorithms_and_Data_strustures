// Найти все вхождения строки T в строку S.

// Формат ввода
// Первые две строки входных данных содержат строки S и T, соответственно. Длины строк больше 0 и меньше 5 ⋅ 10^4, строки содержат только латинские буквы.
// Формат вывода
// Выведите номера символов, начиная с которых строка T входит в строку S, в порядке возрастания. 


#include <iostream>
#include <vector>
#include <string>

std::vector<int> PrefixFunc(std::string& str) {
    std::vector<int> p(str.size(), 0);
    for (size_t i = 1; i < str.size(); ++i) {
        int k = p[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = p[k - 1];
        }
        if (str[i] == str[k]) {
            ++k;
        }
        p[i] = k;
    }
    return p;
}

int main() {
    std::string str;
    std::string find_str;
    std::cin >> str >> find_str;
    std::string str_for_function = find_str + '!' + str;
    std::vector<int> prefix_mas = PrefixFunc(str_for_function);
    for (size_t i = 0; i < str.size(); ++i) {
        if (prefix_mas[i + find_str.size() + 1] == static_cast<int>(find_str.size())) {
            std::cout << i - find_str.size() + 1 << "\n";
        }
    }
    return 0;
}
