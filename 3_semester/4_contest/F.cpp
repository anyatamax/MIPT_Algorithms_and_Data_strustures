

// Ваша цель — передать сверхсекретное сообщение, состоящее из n слов. Так как сообщение секретное, вам необходимо его зашифровать так, чтобы в итоге все предложение состояло из одного слова. А вот и сам алгоритм:
// При склеивании двух слов в одно, удаляется самый длинный префикс второго слова, который совпадает с суффиксом первого слова. Например, слова «крошка» и «картошка» склеиваются в «крошкартошка».
// Сообщение склеивается слева направо (т.е. сначала склеиваются первые два слова, потом результат с третьим, и так далее). Напишите программу, которая зашифрует заданное сообщение.

// Формат ввода
// Первая строка содержит одно целое число n (1 ≤ n ≤ 10^5) — количество слов в сообщении. Вторая строка содержит n слов, разделенных единичными пробелами. Каждое слово непусто и состоит из строчных латинских букв. Суммарная длина слов не превосходит 10^6.
// Формат вывода
// Выведите в выходной файл зашифрованное сообщение.


#include <iostream>
#include <vector>
#include <string>
#include <string_view>

int PrefixFunc(std::string& str) {
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
    return p.back();
}

int main() {
    int n = 0;
    std::cin >> n;
    std::string str;
    std::cin >> str;
    for (int i = 1; i < n; ++i) {
        std::string new_str;
        std::cin >> new_str;
        std::string new_string_find;
        if (str.size() > new_str.size()) {
            new_string_find = str.substr(str.size() - new_str.size(), new_str.size());
        } else {
            new_string_find = str;
        }
        std::string str_find = new_str + '!' + new_string_find;
        int count_repeated = PrefixFunc(str_find);
        str += new_str.substr(count_repeated, new_str.size());
    }
    std::cout << str;
    return 0;
}
