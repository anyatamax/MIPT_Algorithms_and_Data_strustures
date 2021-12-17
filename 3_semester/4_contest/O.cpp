

// В 2046 году наконец-то была введена автоматическая система учета платы за общежитие. Больше нет необходимости ждать, когда отвественный человек руками уберет студента из списка должников, однако необходимость вести бумажную отчетность для налоговой осталась. К сожалению, остальные технологии ВУЗа не ушли также далеко, в частности, в распоряжении общежитий всё ещё находится всего один принтер использующий movable-type printing. Для его работы каждое слово, которое планируется напечатать, сначала собирается из металических кусочков, каждый кусочек содержит одну букву, потом эти кусочки одновременно вдавливаются в бумагу, в итоге на листе печатается одно слово. Принтер позволяет выпонять над собой следующие операции:
//     Добавить букву в конец слова, которое в данный момент составлено на принтере.
//     Удалить последнюю букву из слова на принтере.
//     Напечатать слово, составленное на принтере.
// Изначально на принтере пустое слово, в конце можно оставить непустое, чтобы напакостить другим пользователям принтера. Вам выдают список фамилий, все фамилии записаны с маленькой буквы, потому что это ещё не финальная версия системы, длина каждой фамилии не превосходит 20 символов. Выполнение каждой из трёх операций занимает одну секунду, Вам нужно составить порядок операций, выполнение которого занимает минимальное время и печатает все поданные на вход фамилии (в любом порядке).

// Формат ввода
// На первой строке вводится число фамилий (), на N последующих строках вводятся фамилии.
// Формат вывода
// В первой строке выведите M — число операций, которые Вы собирааетесь проделать. На следующих M строках выведите операции:
//     Операция добавления символа в принтер обозначается добавляемым символом.
//     Операция удаления последнего символа из принтера обозначается символом «-».
//     Печать слова на принтере обозначается символом «P».


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <queue>
#include <algorithm>

struct Node {
    std::unordered_map<char, int64_t> next;
    bool is_terminal = false;
};

class SetString {
public:
    void Insert(std::string &str) {
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
    }
    int64_t BFS() {
        dist_.assign(tree_.size(), 100);
        parent_.assign(tree_.size(), -1);
        dist_[0] = 0;
        int64_t max_dist = 0;
        int64_t max_ver = 0;
        std::queue<int64_t> q;
        q.push(0);
        while (!q.empty()) {
            int64_t ver = q.front();
            q.pop();
            std::shared_ptr<Node> cur_node = tree_[ver];
            for (auto &symbol : cur_node->next) {
                if (dist_[cur_node->next[symbol.first]] == 100) {
                    dist_[cur_node->next[symbol.first]] = dist_[ver] + 1;
                    if (dist_[cur_node->next[symbol.first]] > max_dist) {
                        max_dist = dist_[cur_node->next[symbol.first]];
                        max_ver = cur_node->next[symbol.first];
                    }
                    parent_[cur_node->next[symbol.first]] = ver;
                    q.push(cur_node->next[symbol.first]);
                }
            }
        }
        return max_ver;
    }
    void Print() {
        int64_t end_ver = BFS();
        std::string max_name;
        int64_t prev = parent_[end_ver];
        while (prev != -1) {
            for (auto &i : tree_[prev]->next) {
                if (tree_[prev]->next[i.first] == end_ver) {
                    max_name += i.first;
                    break;
                }
            }
            end_ver = prev;
            prev = parent_[prev];
        }
        std::reverse(max_name.begin(), max_name.end());
        std::shared_ptr<Node> cur_node = tree_[0];
        for (auto &symbol : cur_node->next) {
            if (symbol.first == max_name[0]) {
                continue;
            }
            str_result_ += symbol.first;
            PrintNode(cur_node->next[symbol.first]);
            str_result_ += "-";
        }
        str_result_ += max_name[0];
        PrintNodeWithMinLen(cur_node->next[max_name[0]], 1, max_name);
        std::cout << str_result_.size() << "\n";
        for (auto &i : str_result_) {
            std::cout << i << "\n";
        }
    }
    void PrintNodeWithMinLen(int64_t ver, int index, std::string max_name) {
        if (tree_[ver]->next.empty()) {
            str_result_ += "P";
            return;
        }
        if (tree_[ver]->is_terminal) {
            str_result_ += "P";
        }
        for (auto &symbol : tree_[ver]->next) {
            if (symbol.first == max_name[index]) {
                continue;
            }
            str_result_ += symbol.first;
            PrintNode(tree_[ver]->next[symbol.first]);
            str_result_ += "-";
        }
        str_result_ += max_name[index];
        PrintNodeWithMinLen(tree_[ver]->next[max_name[index]], index + 1, max_name);
    }
    void PrintNode(int64_t ver) {
        if (tree_[ver]->next.empty()) {
            str_result_ += "P";
            return;
        }
        if (tree_[ver]->is_terminal) {
            str_result_ += "P";
        }
        for (auto &symbol : tree_[ver]->next) {
            str_result_ += symbol.first;
            PrintNode(tree_[ver]->next[symbol.first]);
            str_result_ += "-";
        }
    }

private:
    std::vector<std::shared_ptr<Node>> tree_;
    std::vector<int64_t> dist_;
    std::vector<int64_t> parent_;
    std::string str_result_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int m = 0;
    std::cin >> m;
    SetString s;
    for (int i = 0; i < m; ++i) {
        std::string name;
        std::cin >> name;
        s.Insert(name);
    }
    s.Print();
    return 0;
}
