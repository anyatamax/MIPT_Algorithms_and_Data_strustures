// Джек недавно прочитал на заборе занимательное и новое для него слово. Оно настолько понравилось Джеку, что он захотел сам придумать ещё какое-нибудь занимательное слово. Но только ничего у него не вышло: все придуманные им слова состояли из префиксов исходного слова и поэтому не приносили радости. Он стал придумывать всё более и более длинные слова, но ни одно из них не было оригинальным.
// И вот настало время Джеку сказать своё последнее слово.

// Формат ввода
// Первая строка содержит занимательное слово, которое было написано на заборе. Вторая строка содержит последнее слово Джека. Длины слов не превосходят 75000, слова непустые и состоят из строчных латинских букв.
// Формат вывода
// Если Джек так ничего и не придумал своего, выведите первой строкой No. В этом случае покажите Джеку, как разбить его последнее слово на несколько частей, каждая из которых является исходным словом или его непустым префиксом — выведите все эти части во второй строке, разделяя их пробелом. Если же такого разбиения нет, и последнее слово было за Джеком, выведите единственной строкой Yes.


#include <iostream>
#include <vector>
#include <string>

std::vector<int> ZFunc(std::string& str) {
    std::vector<int> z(str.size(), 0);
    int left = 0;
    int right = 0;
    int len = str.size();
    z[0] = len;
    for (int i = 1; i < len; ++i) {
        z[i] = std::max(0, std::min(right - i, z[i - left]));
        while (i + z[i] < len && str[z[i]] == str[i + z[i]]) {
            ++z[i];
        }
        if (right < i + z[i]) {
            left = i;
            right = i + z[i];
        }
    }
    return z;
}

void IsUniqueStr(std::string& str, std::string& last_str) {
    std::string str_for_find = str + '!' + last_str;
    std::vector<int> z_func = ZFunc(str_for_find);
    std::vector<std::pair<int, int>> cover_str;
    int left_first = static_cast<int>(str.size()) + 1;
    int right_first = left_first + z_func[left_first] - 1;
    for (size_t i = str.size() + 2; i < str_for_find.size(); ++i) {
        if (z_func[i] > 0) {
            int left_second = i;
            int right_second = left_second + z_func[i] - 1;
            if (left_second <= right_first && right_second >= right_first) {
                right_first = left_second - 1;
                cover_str.emplace_back(std::make_pair(left_first, right_first));
                left_first = left_second;
                right_first = right_second;
            } else if (left_second > right_first) {
                cover_str.emplace_back(std::make_pair(left_first, right_first));
                left_first = left_second;
                right_first = right_second;
            }
        }
        if (i == str_for_find.size() - 1) {
            cover_str.emplace_back(std::make_pair(left_first, right_first));
        }
    }
    std::string str_result;
    for (auto& p : cover_str) {
        str_result += str_for_find.substr(p.first, p.second - p.first + 1);
    }
    if (str_result != last_str) {
        std::cout << "Yes";
    } else {
        std::cout << "No\n";
        for (auto& p : cover_str) {
            std::cout << str_for_find.substr(p.first, p.second - p.first + 1) << " ";
        }
    }
}

int main() {
    std::string str;
    std::string last_str;
    std::cin >> str >> last_str;
    IsUniqueStr(str, last_str);
    return 0;
}
