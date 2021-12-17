

// Вам задан массив префикс-функции некоторой строки. Ваша задача — найти лексикографически минимальную строку, массив префикс-функции которой совпадет с данным. Алфавит для построения ответа: a-z.

// Формат ввода
// На входе в первой строке дано число N — размер массива префикс-функции (0 ≤ N ≤ 10^6). На второй строке через пробел идут сами элементы. Гарантируется, что исходный массив является корректным массивом префикс-функции.
// Формат вывода
// Выведите искомую строку.


#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

std::string PrefixToString(std::vector<int>& prefix_func) {
    std::string alphabet = "abcdefghijklmnopqrstuvxyz";
    std::string str;
    if (!prefix_func.empty()) {
        str = 'a';
    }
    for (size_t i = 1; i < prefix_func.size(); ++i) {
        if (prefix_func[i] > 0) {
            str += str[prefix_func[i] - 1];
        } else {
            std::unordered_set<char> ban;
            int k = prefix_func[i - 1];
            while (k > 0) {
                ban.insert(str[k]);
                k = prefix_func[k - 1];
            }
            for (size_t s = 1; s < alphabet.size(); ++s) {
                if (ban.find(alphabet[s]) == ban.end()) {
                    str += alphabet[s];
                    break;
                }
            }
        }
    }
    return str;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> prefix_func(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> prefix_func[i];
    }
    std::cout << PrefixToString(prefix_func);
    return 0;
}
