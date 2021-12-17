// Вам задан массив Z-функции некоторой строки. Ваша задача — найти лексикографически минимальную строку, массив Z-функции которой совпадет с данным. Алфавит для построения ответа: a-z.

// Формат ввода
// На входе в первой строке дано число N — размер массива Z-функции (0 ≤ N ≤ 10^6). На второй строке через пробел идут сами элементы. Гарантируется, что исходный массив является корректным массивом Z-функции.
// Формат вывода
// Выведите искомую строку.


#include <iostream>
#include <vector>
#include <string>
#include <set>

std::string ZToString(std::vector<int>& z_func) {
    std::string alphabet = "abcdefghijklmnopqrstuvxyz";
    std::string str;
    std::vector<std::set<char>> ban(z_func.size());
    if (!z_func.empty()) {
        str = 'a';
    }
    int prefix_len = 0;
    int index = 0;
    for (size_t i = 1; i < z_func.size(); ++i) {
        if (z_func[i] == 0 && prefix_len == 0) {
            if (ban[i].empty()) {
                str += 'b';
            } else {
                for (size_t s = 1; s < alphabet.size(); ++s) {
                    if (ban[i].find(alphabet[s]) == ban[i].end()) {
                        str += alphabet[s];
                        break;
                    }
                }
            }
        }
        if (z_func[i] > 0 && i + z_func[i] < z_func.size()) {
            ban[i + z_func[i]].insert(str[z_func[i]]);
        }
        if (z_func[i] > prefix_len) {
            prefix_len = z_func[i];
            index = 0;
        }
        if (prefix_len > 0) {
            str += str[index];
            ++index;
            --prefix_len;
        }
    }
    return str;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> z_func(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> z_func[i];
    }
    std::cout << ZToString(z_func);
    return 0;
}
