//  В сверхсекретной организации фпМИ-6 завелись двойные агенты, причастные к внесению одного столичного ВУЗа в санкционный список. Для их выявления командование придумало задание-проверку: каждому агенту было поручено выяснить хотя бы часть секретного пароля от секретного чемодана, в котором хранятся секретные секреты. Позже эти данные планировалось проанализировать и выявить предателей. Само командование, конечно, секретный пароль знает целиком.
// Первая часть прошла без проишествий, все агенты сообщили в штаб найденные части пароля и индексы-границы, в которых эта часть лежит (границы могут быть неточны, но найденная часть определенно лежит в них, если агент хороший).
// Теперь самое интересное: почему Вас посвятили в столь секретные материалы? Дело в том, что именно к Вам командование обратилось с просьбой выявить двойных агентов, проанализировав правильный пароль и отчёты всех агентов. Проверка довольно проста: допустим, агент сообщил в штаб индексы-границы l и r и строку <str>, а правильный пароль — это строка <password>, если <str> — подстрока строки <password>[l:r] (включая обе границы), то агент хороший и его отпускают, если нет, его ждёт особая беседа. Помогите комадованию избавиться от предателей.

// Формат ввода
// В первой строке вводится пароль — строка длиной не более 10^5 символов. Во второй вводится N — число агентов. В следующих N строках вводятся последовательно два индекса-границы, заявленные i-м агентом, и заявленная им же подстрока пароля. Суммарная длина заявленных частей пароля не превосходит 10^5.
// Формат вывода
// Для каждого агента выведите «+», если он хороший, и «-», если с ним нужно поговорить отдельно.


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
    std::vector<int64_t> index;
    int64_t len;
};

class SetString {
public:
    explicit SetString(int size) : res_(size, '-'){};
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
        cur_node->index.push_back(index);
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
    void PrintIndex(std::string &str, std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> &answers,
                    size_t index, std::shared_ptr<Node> &node) {
        if (!node->is_terminal) {
            node = node->exit_link;
        }
        while (node != nullptr) {
            for (size_t i = 0; i < node->index.size(); ++i) {
                int64_t id = node->index[i];
                int64_t start_pos = index - node->len + 1;
                if (answers[id].second.first > 0 && answers[id].second.first <= start_pos + 1 &&
                    answers[id].second.second >= start_pos + answers[id].first &&
                    start_pos + answers[i].first <= static_cast<int64_t>(str.size())) {
                    if (res_[id] == '-') {
                        res_[id] = '+';
                    }
                }
            }
            node = node->exit_link;
        }
    }
    void AhoCorasick(std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> &answers, std::string &str) {
        BuildSuffixReferences();
        BuildExitLinks();
        std::shared_ptr<Node> node = tree_[0];
        for (size_t i = 0; i < str.size(); ++i) {
            std::shared_ptr<Node> cur_node = NextState(node, str[i]);
            node = cur_node;
            PrintIndex(str, answers, i, cur_node);
        }
    }
    void CheckAnswers(std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> &answers, std::string &str) {
        AhoCorasick(answers, str);
        for (auto &i : res_) {
            std::cout << i;
        }
    }

private:
    std::vector<std::shared_ptr<Node>> tree_;
    std::vector<char> res_;
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
    std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> answers(n);
    for (int i = 0; i < n; ++i) {
        int64_t left = 0;
        int64_t right = 0;
        std::string cur_str;
        std::cin >> left >> right >> cur_str;
        answers[i] = std::make_pair(cur_str.size(), std::make_pair(left, right));
        s.Insert(cur_str, i);
    }
    s.CheckAnswers(answers, str);
    return 0;
}
