//  Дана строка. Для каждого ее префикса найдите количество ее различных подстрок.

// Формат ввода
// Первая строка ввода содержит непустую строку S, состоящую из N (1≤N≤2⋅10^5) строчных латинских букв.
// Формат вывода
// Выведите N строк; i-ая из которых должна содержать число различных подстрок i-ого префикса S. 


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

struct Node {
    std::unordered_map<char, int64_t> next;
    int64_t len = 0;
    int64_t suff_link = -1;
    int64_t count_substr = 0;
};

class SuffMachine {
public:
    explicit SuffMachine(std::string &str) {
        tree_.push_back(Node{});
        cur_str_ = str;
        count_str_ = 0;
        cur_last_ = 0;
        tree_[cur_last_].count_substr = 1;
    }
    void AddSymbol(char symbol) {
        tree_.emplace_back(Node{});
        int64_t cur_node = tree_.size() - 1;
        int64_t parent = cur_last_;
        tree_[cur_node].len = tree_[parent].len + 1;
        while (parent != -1 && tree_[parent].next.find(symbol) == tree_[parent].next.end()) {
            tree_[cur_node].count_substr += tree_[parent].count_substr;
            tree_[parent].next[symbol] = cur_node;
            parent = tree_[parent].suff_link;
        }
        count_str_ += tree_[cur_node].count_substr;
        if (parent == -1) {
            tree_[cur_node].suff_link = 0;
            cur_last_ = cur_node;
            return;
        }
        int64_t next = tree_[parent].next[symbol];
        if (tree_[next].len == tree_[parent].len + 1) {
            tree_[cur_node].suff_link = next;
            cur_last_ = cur_node;
            return;
        }
        tree_.emplace_back(Node{});
        int64_t clone = tree_.size() - 1;
        tree_[clone].len = tree_[parent].len + 1;
        while (parent != -1 && tree_[parent].next[symbol] == next) {
            tree_[parent].next[symbol] = clone;
            tree_[clone].count_substr += tree_[parent].count_substr;
            tree_[next].count_substr -= tree_[parent].count_substr;
            parent = tree_[parent].suff_link;
        }
        tree_[clone].next = tree_[next].next;
        tree_[clone].suff_link = tree_[next].suff_link;
        tree_[next].suff_link = clone;
        tree_[cur_node].suff_link = clone;
        cur_last_ = cur_node;
    }
    void BuildPrint() {
        for (auto &symbol : cur_str_) {
            AddSymbol(symbol);
            std::cout << count_str_ << "\n";
        }
    }

private:
    std::vector<Node> tree_;
    int64_t cur_last_;
    std::string cur_str_;
    int64_t count_str_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string str;
    std::cin >> str;
    SuffMachine machine(str);
    machine.BuildPrint();
    return 0;
}
