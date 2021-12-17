

// На курсе алгоритмов в ФПМИ есть модуль, посвященный строкам. Преподаватель решил не ограничиться стандартными задачами и предложил своим студентам следующую задачу.
// Есть текст T, в котором ищут паттерн (тоже строку) P. Казалось бы, классическая задача, но семинарист решил ее усложнить, а именно, надо искать не только паттерн, но и все его циклические сдвиги. Нужно найти количество вхождений (возможно пересекающихся) всех циклических сдвигов паттерна в тексте.
// Отметим, что если паттерн «abс», то его циклическими сдвигами называют следующие строки: «abс», «bca», «cab».

// Формат ввода
// На первой строке вам дан паттерн P (1 ≤ |P| ≤ 10^6). На второй дан текст T длиной до 10^6 символов. Обе строки состоят из латинских строчных букв.
// Формат вывода
// Выведите ответ на задачу.


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

struct Node {
    std::unordered_map<char, int64_t> next;
    int64_t len = 0;
    int64_t suff_link = -1;
    int64_t cnt = 0;
};

struct PairHash {
    inline size_t operator()(const std::pair<int64_t, int64_t> &v) const {
        std::hash<int64_t> int_hasher;
        return int_hasher(v.first) ^ int_hasher(v.second);
    }
};

struct Comp {
    bool operator()(const int64_t &lhs, const int64_t &rhs) const {
        return lhs >= rhs;
    }
};

class SuffMachine {
public:
    explicit SuffMachine(std::string &str) {
        tree_.push_back(Node{});
        cur_str_ = str;
        cur_last_ = 0;
        tree_sort_len_[0].push_back(cur_last_);
    }
    void AddSymbol(char symbol) {
        tree_.emplace_back(Node{});
        int64_t cur_node = tree_.size() - 1;
        int64_t parent = cur_last_;
        tree_[cur_node].len = tree_[parent].len + 1;
        tree_[cur_node].cnt = 1;
        tree_sort_len_[tree_[cur_node].len].push_back(cur_node);
        while (parent != -1 && tree_[parent].next.find(symbol) == tree_[parent].next.end()) {
            tree_[parent].next[symbol] = cur_node;
            parent = tree_[parent].suff_link;
        }
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
        tree_sort_len_[tree_[clone].len].push_back(clone);
        while (parent != -1 && tree_[parent].next[symbol] == next) {
            tree_[parent].next[symbol] = clone;
            parent = tree_[parent].suff_link;
        }
        tree_[clone].next = tree_[next].next;
        tree_[clone].suff_link = tree_[next].suff_link;
        tree_[next].suff_link = clone;
        tree_[cur_node].suff_link = clone;
        cur_last_ = cur_node;
    }
    void QSort(int64_t n_start, int64_t n_end, std::vector<int64_t> &tree_sort) {
        int64_t left = 0;
        int64_t right = 0;
        int64_t elem = 0;
        if (n_start >= n_end) {
            return;
        }
        left = n_start;
        right = n_end;
        elem = tree_[tree_sort[(right - left) / 2 + left]].len;
        while (left <= right) {
            while (tree_[tree_sort[left]].len > elem) {
                left++;
            }
            while (tree_[tree_sort[right]].len < elem) {
                right--;
            }
            if (left <= right) {
                std::swap(tree_sort[left], tree_sort[right]);
                left++;
                right--;
            }
        }
        QSort(n_start, right, tree_sort);
        QSort(left, n_end, tree_sort);
    }
    void Build() {
        for (auto &symbol : cur_str_) {
            AddSymbol(symbol);
        }
        for (auto &elem : tree_sort_len_) {
            for (auto &j : elem.second) {
                int64_t node = j;
                int64_t link = tree_[node].suff_link;
                if (link != -1) {
                    tree_[link].cnt += tree_[node].cnt;
                }
            }
        }
    }
    int64_t FindStr(std::string &str) {
        used_.assign(tree_.size(), false);
        int64_t count = 0;
        int64_t cur_node = 0;
        std::string new_str = str + str;
        int64_t len = 0;
        int64_t len_new_str = 0;
        while (len_new_str < static_cast<int64_t>(new_str.size()) - 1) {
            if (len == static_cast<int64_t>(str.size()) ||
                tree_[cur_node].next.find(new_str[len_new_str]) == tree_[cur_node].next.end()) {
                if (--len < 0) {
                    break;
                }
                int64_t suf_link = tree_[cur_node].suff_link;
                if (tree_[suf_link].len >= len) {
                    cur_node = suf_link;
                }
                continue;
            }
            ++len;
            cur_node = tree_[cur_node].next[new_str[len_new_str]];
            if (len == static_cast<int64_t>(str.size())) {
                if (!used_[cur_node]) {
                    used_[cur_node] = true;
                    count += tree_[cur_node].cnt;
                }
            }
            ++len_new_str;
        }
        return count;
    }

private:
    const int64_t max_len_ = 1000001;
    std::vector<Node> tree_;
    std::map<int64_t, std::vector<int64_t>, Comp> tree_sort_len_;
    int64_t cur_last_;
    std::string cur_str_;
    std::vector<bool> used_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string str;
    std::string pattern;
    std::cin >> pattern >> str;
    SuffMachine machine(str);
    machine.Build();
    int64_t len = pattern.size();
    if (len > static_cast<int64_t>(str.size())) {
        std::cout << 0;
        return 0;
    }
    std::cout << machine.FindStr(pattern);
    return 0;
}
