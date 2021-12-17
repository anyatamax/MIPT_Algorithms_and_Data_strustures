
// Старый торговец арбузами Рабинович заработал уже очень много денег, пользуясь своей, без преувеличения, гениальной схемой, но тут ему в голову пришла ещё одна: арбузная лотерея. Рабинович за определенную плату выдаёт каждому желающему лотерейный билетик, участники пишут на своих билетиках некоторые K-значные числа (возможно, с ведущими нулями), после чего выбирается билет-победитель. Каждый участник выигрывает тем больше арбузов, чем больше первых цифр билета-победителя он угадал в своей записи. Однако Рабинович не был бы Рабиновичем, если бы лотерея была честной, поэтому он отдаёт участнику билет только после того, как тот напишет на нём число. Таким образом, Рабинович знает все числа, написанные участниками, и просит Вас помочь (за три арбуза и немного баллов за семестр) выбрать такой билет-победитель, чтобы пришлось отдать как можно меньше абрузов.

// Формат ввода
// В первой строке вводится число M () — количество участников лотереи, K () — длина участвующих в лотерее чисел и N () — основание системы счисления записываемых чисел. Во второй строке вводится K чисел — , ci — число арбузов, которое обещает отдать Рабинович участнику, угадавшему первые i цифр числа-победителя. В последующих M строках вводятся записанные участниками числа с ведущими нулями, все они отсортированы по неубыванию.
// Формат вывода
// В первой строке выведите число-победитель (если их несколько, выведите любое), во второй — число арбузов, с которым распрощается старый торговец.



#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

struct Node {
    std::unordered_map<char, int64_t> next;
    std::unordered_map<char, int64_t> count;
    bool is_terminal = false;
    std::string cur_str;
    int64_t prev;
    int64_t index = 0;
};

class SetString {
public:
    SetString(int l, int s, std::vector<int64_t> &costs) {
        len_ = l;
        system_ = s;
        costs_ = costs;
        if (tree_.empty()) {
            tree_.push_back(std::make_shared<Node>(Node{}));
        }
        cur_node_ = tree_[0];
    };
    void Insert(std::string &str) {
        if (cur_node_ == tree_[0]) {
            for (auto &symbol : str) {
                int64_t next_id = cur_node_->next[symbol];
                if (next_id == 0) {
                    tree_.push_back(std::make_shared<Node>(Node{}));
                    cur_node_->next[symbol] = tree_.size() - 1;
                    cur_node_->count[symbol] = 0;
                    next_id = cur_node_->next[symbol];
                    tree_[next_id]->prev = cur_node_->index;
                    tree_[next_id]->index = next_id;
                    tree_[next_id]->cur_str = cur_node_->cur_str + symbol;
                }
                ++cur_node_->count[symbol];
                cur_node_ = tree_[next_id];
            }
            cur_node_->is_terminal = true;
        } else if (cur_node_->cur_str == str) {
            std::shared_ptr<Node> node = tree_[0];
            for (auto &symbol : str) {
                ++node->count[symbol];
                node = tree_[node->next[symbol]];
            }
        } else {
            std::string copy_str = str;
            while (cur_node_->cur_str != copy_str) {
                if (static_cast<int>(cur_node_->cur_str.size()) == len_) {
                    cur_node_ = tree_[cur_node_->prev];
                    copy_str = copy_str.substr(0, copy_str.size() - 1);
                    continue;
                }
                if (static_cast<int>(cur_node_->next.size()) == system_) {
                    cur_node_ = tree_[cur_node_->prev];
                    copy_str = copy_str.substr(0, copy_str.size() - 1);
                    continue;
                }
                char symbol = ' ';
                for (int i = 0; i < system_; ++i) {
                    if (cur_node_->next.find(i + '0') == cur_node_->next.end()) {
                        symbol = static_cast<char>(i + '0');
                        break;
                    }
                }
                std::string new_str = cur_node_->cur_str;
                while (static_cast<int>(new_str.size()) != len_) {
                    new_str += symbol;
                }
                to_see_later_.push_back(new_str);
                cur_node_ = tree_[cur_node_->prev];
                copy_str = copy_str.substr(0, copy_str.size() - 1);
            }
            cur_node_ = InsertWithCount(str);
        }
    }
    std::shared_ptr<Node> InsertWithCount(std::string &str) {
        std::shared_ptr<Node> node = tree_[0];
        for (auto &symbol : str) {
            int64_t next_id = node->next[symbol];
            if (next_id == 0) {
                tree_.push_back(std::make_shared<Node>(Node{}));
                node->next[symbol] = tree_.size() - 1;
                node->count[symbol] = 0;
                next_id = node->next[symbol];
                tree_[next_id]->prev = node->index;
                tree_[next_id]->index = next_id;
                tree_[next_id]->cur_str = node->cur_str + symbol;
            }
            ++node->count[symbol];
            node = tree_[next_id];
        }
        node->is_terminal = true;
        return node;
    }
    void FindMin() {
        std::shared_ptr<Node> node = tree_[0];
        if (static_cast<int>(node->next.size()) < system_) {
            char symbol = ' ';
            for (int i = 0; i < system_; ++i) {
                if (node->next.find(i + '0') == node->next.end()) {
                    symbol = static_cast<char>(i + '0');
                    break;
                }
            }
            while (static_cast<int>(str_result_.size()) < len_) {
                str_result_ += symbol;
            }
            min_result_ = 0;
            return;
        }
        for (auto &symbol : node->next) {
            FindMinRecurse(tree_[node->next[symbol.first]], 1, node->count[symbol.first] * costs_[0]);
        }
        for (auto &str : to_see_later_) {
            int64_t cost = Cost(str);
            if (min_result_ > cost) {
                min_result_ = cost;
                str_result_ = str;
            }
        }
    }
    int64_t Cost(std::string &str) {
        std::shared_ptr<Node> node = tree_[0];
        int64_t cost = 0;
        int len = 0;
        for (auto &symbol : str) {
            int64_t next_id = node->next[symbol];
            if (next_id == 0) {
                break;
            }
            if (len > 0) {
                cost = cost + (costs_[len] - costs_[len - 1]) * node->count[symbol];
            } else {
                cost = node->count[symbol] * costs_[0];
            }
            ++len;
            node = tree_[next_id];
        }
        return cost;
    }
    void FindMinRecurse(std::shared_ptr<Node> &node, int len, int64_t cost) {
        if (node->is_terminal) {
            if (cost < min_result_) {
                min_result_ = cost;
                str_result_ = node->cur_str;
            }
            return;
        }
        for (auto &symbol : node->next) {
            FindMinRecurse(tree_[node->next[symbol.first]], len + 1,
                           cost + node->count[symbol.first] * (costs_[len] - costs_[len - 1]));
        }
    }
    void PrintAnswer() {
        std::cout << str_result_ << "\n" << min_result_;
    }

private:
    std::vector<std::shared_ptr<Node>> tree_;
    int len_;
    int system_;
    std::shared_ptr<Node> cur_node_;
    std::string str_result_;
    int64_t min_result_ = std::numeric_limits<int64_t>::max();
    std::vector<int64_t> costs_;
    std::vector<std::string> to_see_later_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int m = 0;
    int len = 0;
    int sys = 0;
    std::cin >> m >> len >> sys;
    std::vector<int64_t> costs(len);
    for (int i = 0; i < len; ++i) {
        std::cin >> costs[i];
    }
    SetString s(len, sys, costs);
    for (int i = 0; i < m; ++i) {
        std::string number;
        std::cin >> number;
        s.Insert(number);
    }
    s.FindMin();
    s.PrintAnswer();
    return 0;
}
