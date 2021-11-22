// Вам дан словарь, состоящий из пар слов. Каждое слово является синонимом к парному ему слову. Все слова в словаре различны. Для каждого данного слова определите его синоним.
// Для решения данной задачи используйте std::unordered_map

// Формат ввода
// Программа получает на вход количество пар синонимов N (0 ≤ N ≤ 10^5). Далее следует N строк, каждая строка содержит ровно два слова-синонима.
// Затем идет число Q (1 ≤ Q ≤ 10^5) — количество запросов к словарю. Далее на каждой следующей из Q строк идет слово, к которому надо вывести синоним.
// Формат вывода
// Программа должна вывести синонимы к данным слову на отдельных строках.


#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::unordered_map<std::string, std::string> dict;
    int num;
    std::cin >> num;
    for (size_t i = 0; i < num; ++i) {
        std::string first, second, str;
        std::cin >> first >> second;
        dict.emplace(std::make_pair(first, second));
        dict.emplace(std::make_pair(second, first));
    }
    std::cin >> num;
    for (size_t i = 0; i < num; ++i) {
        std::string word;
        std::cin >> word;
        auto result = dict.find(word);
        std::cout << result->second << "\n";
    }
    return 0;
}
