// Дан текст. Выведите все слова, встречающиеся в тексте, по одному на каждую строку. Слова должны быть отсортированы по убыванию их количества появления в тексте, а при одинаковой частоте появления в лексикографическом порядке.

// Формат ввода
// Вводится текст — последовательность строк через пробел или перенос строки.
// Формат вывода
// Выведите ответ на задачу.


#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <list>

bool Comp(const std::pair<int, std::string>& lhs, const std::pair<int, std::string>& rhs) {
    if (lhs.first != rhs.first) {
        return lhs.first > rhs.first;
    }
    return lhs.second < rhs .second;
}
int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::map<std::string, int> words;
    std::string word;
    while (std::cin >> word) {
        if (word.empty()) {
            break;
        }
        auto client = words.find(word);
        if (client != words.end()) {
            client->second += 1;
        } else {
            words[word] = 1;

        }
    }
    std::list<std::pair<int, std::string>> sort_words;
    for (auto& it : words) {
        sort_words.emplace_back(std::make_pair(it.second, it.first));
    }
    sort_words.sort(Comp);
    for (auto& it : sort_words) {
        std::cout << it.second << "\n";
    }
    return 0;
}
