//  Астрономы-(радио)любители лось Валера и филин Евгений пытаются поймать радиосообщения внеземных цивилизаций. Недавно они (астрономы) получили странный сигнал и записали его в виде слова, состоящего из маленьких букв английского алфавита. Теперь товарищам необходимо расшифровать сигнал. Но как подойти к расшифровке?
// После трех кругов напряженного обдумывания в МЦК, астрономы Белокаменной предположили, что сообщение состоит из слов; однако как распознать слова — все еще непонятно. Валера и Евгений называют подслово исходного сообщения потенциальным словом, если это подслово обладает хотя бы двумя непересекающимися вхождениями в исходное сообщение.
// Например, если сообщением было "abacabacaba", то "abac" — потенциальное слово, а "acaba" — нет в силу пересекаемости его вхождений.
// Дано сообщение s. Помогите лосю Валере и филину Евгению найти число потенциальных слов в s.

// Формат ввода
// На ввод подается единственная строка, состоящая из маленьких латинских букв — полученное лосем Валерой и филином Евгением сообщение. Длина строки не превосходит 10000.
// Формат вывода
// Выведите одно число — число потенциальных слов в сообщении. 


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

struct Node {
    std::unordered_map<char, int64_t> next;
    int64_t len = 0;
    int64_t suff_link = -1;
    int64_t index_first = 0;
    int64_t index_second = 0;
};

class SuffMachine {
public:
    explicit SuffMachine(std::string &str) {
        tree_.push_back(Node{});
        cur_str_ = str;
        cur_last_ = 0;
        tree_[0].index_first = 0;
        tree_[0].index_second = str.size() - 1;
    }
    void AddSymbol(char symbol, int64_t index) {
        tree_.emplace_back(Node{});
        int64_t cur_node = tree_.size() - 1;
        int64_t parent = cur_last_;
        tree_[cur_node].len = tree_[parent].len + 1;
        tree_[cur_node].index_first = index;
        tree_[cur_node].index_second = index;
        while (parent != -1 && tree_[parent].next.find(symbol) == tree_[parent].next.end()) {
            tree_[parent].next[symbol] = cur_node;
            parent = tree_[parent].suff_link;
        }
        if (parent == -1) {
            tree_[cur_node].suff_link = 0;
            cur_last_ = cur_node;
            int64_t p = cur_last_;
            while (p != -1) {
                tree_[p].index_second = index;
                p = tree_[p].suff_link;
            }
            return;
        }
        int64_t next = tree_[parent].next[symbol];
        if (tree_[next].len == tree_[parent].len + 1) {
            tree_[cur_node].suff_link = next;
            cur_last_ = cur_node;
            parent = cur_last_;
            while (parent != -1) {
                tree_[parent].index_second = index;
                parent = tree_[parent].suff_link;
            }
            return;
        }
        tree_.emplace_back(Node{});
        int64_t clone = tree_.size() - 1;
        tree_[clone].len = tree_[parent].len + 1;
        while (parent != -1 && tree_[parent].next[symbol] == next) {
            tree_[parent].next[symbol] = clone;
            parent = tree_[parent].suff_link;
        }
        tree_[clone].next = tree_[next].next;
        tree_[clone].suff_link = tree_[next].suff_link;
        tree_[clone].index_first = tree_[next].index_first;
        tree_[clone].index_second = tree_[next].index_second;
        tree_[next].suff_link = clone;
        tree_[cur_node].suff_link = clone;
        cur_last_ = cur_node;
        parent = cur_last_;
        while (parent != -1) {
            tree_[parent].index_second = index;
            parent = tree_[parent].suff_link;
        }
    }
    void Build() {
        int64_t i = 0;
        for (auto &symbol : cur_str_) {
            AddSymbol(symbol, i);
            ++i;
        }
    }
    int64_t FindCount() {
        int64_t count = 0;
        for (int64_t i = 0; i < static_cast<int64_t>(tree_.size()); ++i) {
            int64_t len = tree_[i].index_second - tree_[i].index_first;
            len = std::min(len, tree_[i].len);
            if (tree_[i].suff_link != -1) {
                if (len >= tree_[tree_[i].suff_link].len) {
                    count += len - tree_[tree_[i].suff_link].len;
                }
            }
        }
        return count;
    }

private:
    std::vector<Node> tree_;
    int64_t cur_last_;
    std::string cur_str_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string str;
    std::cin >> str;
    SuffMachine machine(str);
    machine.Build();
    std::cout << machine.FindCount();
    return 0;
}
