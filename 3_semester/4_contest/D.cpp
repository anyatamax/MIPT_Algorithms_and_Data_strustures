

// Подсчитайте число различных подстрок строки S.

// Формат ввода
// Дана строка S из строчных латинских символов. Ее длина не превосходит 5000 символов.
// Формат вывода
// Выведите количество различных подстрок этой строки.


#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

int ZFunc(std::string& str) {
    std::vector<int> z(str.size(), 0);
    int left = 0;
    int right = 0;
    int len = str.size();
    z[0] = len;
    int max_z = 0;
    for (int i = 1; i < len; ++i) {
        z[i] = std::max(0, std::min(right - i, z[i - left]));
        while (i + z[i] < len && str[z[i]] == str[i + z[i]]) {
            ++z[i];
        }
        if (right < i + z[i]) {
            left = i;
            right = i + z[i];
        }
        max_z = std::max(z[i], max_z);
    }
    return max_z;
}

int main() {
    std::string str;
    std::string str_cur;
    std::cin >> str;
    str_cur = str[0];
    std::string str_cur_reverse = str_cur;
    int count = 1;
    for (size_t i = 1; i < str.size(); ++i) {
        str_cur += str[i];
        str_cur_reverse = str[i] + str_cur_reverse;
        count = count + static_cast<int>(str_cur.size()) - ZFunc(str_cur_reverse);
    }
    std::cout << count;
    return 0;
}
