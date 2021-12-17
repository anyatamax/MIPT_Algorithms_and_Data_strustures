

// В стандартном потоке ввода программе подаётся строка, состоящая из строчных латинских букв, которую мы назовём исходной. На следующей строке программе подаётся число NN (1≤N≤1,000,0001 ≤ N ≤ 1,000,000), а в следующих NN строках — по уникальному слову из строчных латинских букв; эти слова мы назовём словарём. Суммарная длина слов из словая не превосходит 1,000,0001,000,000

// Формат вывода
// Программа должна вывести на стандартный поток выводика N строк. В i-ой строке программа должна вывести несколько чисел: первое число - количество вхождений строки ii из словаря в исходную строку, затем через пробел для каждого вхождения выведите индексы начал всех вхождений этой строки в исходную в отсортированном порядке. Индексы всех строк начинаются с единиц. Гарантируется, что все строки уникальны и количество вхождений не превосходит 1,000,0001,000,000.



#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <queue>

struct Node {
    std::unordered_map<char, int64_t> next;
    bool is_terminal = false;
    std::shared_ptr<Node> suff_ref;
    std::shared_ptr<Node> exit_link;
    int64_t index;
    int64_t len;
};

class SetString {
public:
    explicit SetString(int size) : result_(size){};
    void Insert(std::string &str, int64_t index) {
        if (tree_.empty()) {
            tree_.emplace_back(std::make_shared<Node>(Node{}));
        }
        std::shared_ptr<Node> cur_node = tree_[0];
        for (auto &symbol : str) {
            int64_t next_id = cur_node->next[symbol];
            if (next_id == 0) {
                tree_.emplace_back(std::make_shared<Node>(Node{}));
                cur_node->next[symbol] = tree_.size() - 1;
                next_id = cur_node->next[symbol];
            }
            cur_node = tree_[next_id];
        }
        cur_node->is_terminal = true;
        cur_node->index = index;
        cur_node->len = str.size();
    }
    void BuildSuffixReferences() {
        tree_[0]->suff_ref = nullptr;
        std::queue<std::pair<std::pair<char, std::shared_ptr<Node>>, int64_t>> q;
        for (auto &symbol : tree_[0]->next) {
            q.push(std::make_pair(std::make_pair(symbol.first, tree_[0]), symbol.second));
        }
        while (!q.empty()) {
            char s = q.front().first.first;
            std::shared_ptr<Node> parent = q.front().first.second;
            std::shared_ptr<Node> node = tree_[q.front().second];
            q.pop();
            std::shared_ptr<Node> suff = parent->suff_ref;
            while (suff != nullptr && suff->next.find(s) == suff->next.end()) {
                suff = suff->suff_ref;
            }
            node->suff_ref = (suff == nullptr) ? tree_[0] : tree_[suff->next[s]];
            for (auto &symbol : node->next) {
                q.push(std::make_pair(std::make_pair(symbol.first, node), symbol.second));
            }
        }
    }
    void BuildExitLinks() {
        tree_[0]->exit_link = nullptr;
        std::queue<int64_t> q;
        for (auto &symbol : tree_[0]->next) {
            q.push(symbol.second);
        }
        while (!q.empty()) {
            std::shared_ptr<Node> node = tree_[q.front()];
            q.pop();
            std::shared_ptr<Node> suff = node->suff_ref;
            if (suff != tree_[0]) {
                node->exit_link = (suff->is_terminal) ? suff : suff->exit_link;
            }
            for (auto &symbol : node->next) {
                q.push(symbol.second);
            }
        }
    }
    std::shared_ptr<Node> NextState(std::shared_ptr<Node> &node, char symbol) {
        while (node != nullptr && node->next.find(symbol) == node->next.end()) {
            node = node->suff_ref;
        }
        return (node == nullptr) ? tree_[0] : tree_[node->next[symbol]];
    }
    void PrintIndex(size_t index, std::shared_ptr<Node> &node) {
        if (!node->is_terminal) {
            node = node->exit_link;
        }
        while (node != nullptr) {
            result_[node->index].push_back(index - node->len + 1);
            // std::cout << index << " ";
            node = node->exit_link;
        }
    }
    void AhoCorasick(std::string &str) {
        BuildSuffixReferences();
        BuildExitLinks();
        std::shared_ptr<Node> node = tree_[0];
        for (size_t i = 0; i < str.size(); ++i) {
            std::shared_ptr<Node> cur_node = NextState(node, str[i]);
            node = cur_node;
            PrintIndex(i, cur_node);
        }
    }
    void PrintResult() {
        for (size_t i = 0; i < result_.size(); ++i) {
            std::cout << result_[i].size() << " ";
            for (size_t j = 0; j < result_[i].size(); ++j) {
                std::cout << result_[i][j] + 1 << " ";
            }
            std::cout << "\n";
        }
    }

private:
    std::vector<std::shared_ptr<Node>> tree_;
    std::vector<std::vector<int>> result_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string str;
    std::cin >> str;
    int n = 0;
    std::cin >> n;
    SetString s(n);
    for (int i = 0; i < n; ++i) {
        std::string str_cur;
        std::cin >> str_cur;
        s.Insert(str_cur, i);
    }
    s.AhoCorasick(str);
    s.PrintResult();
    return 0;
}
