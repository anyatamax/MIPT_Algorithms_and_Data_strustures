

// Как Вы, возможно, знаете, любой телефонный номер в России устроен так: это некоторый набор чисел фиксированного размера, часть из которых (первые n) является кодом города. Провайдер МТФИ-телеком выходит на новый для себя рынок — рынок телефонной связи. Важные дяди, сидящие в совете директоров думают о будущем и мечтают, чтобы МТФИ-телеком стал в этом секторе монополистом во всей стране, а не только на территории студенческого городка МТФИ. Чтобы примерно оценить прибыль в таком случае, они прикидывают, сколько телефонных номеров они смогут подключить в каждом городе, если их будут ограничивать только коды.
// Однако задача не так проста, как может показаться. Дело в том, что телефонные коды могут пересекаться. Например, один город может иметь код 803, а другой — 8030, тогда все телефонные номера, начинающиеся на 8030 (при условии, что не существует больше города с кодом, имеюшим такой префикс) будут принадлежать второму городу а номера, начинающиеся с 8031, 8032, …, 8039 (при том же условии) — первому городу. В связи с этим, важные дяди решили обратиться к Вам с вопросом.

// Формат ввода
// В первой строке вводятся два числа — N () — количество интересующих дядь городов и M () — количество цифр в телефонном номере. В следующих N строках вводится код i-го города и название этого города, длина названия города не превосходит 20 символов. Считайте, что нет занятых кодов, помимо введенных, а также что любой телефонный номер возможен (любой из 10M).
// Формат вывода
// Для каждого города выведите число телефонных номеров, которые могут быть подключены в этом городе. Выводить города можно в любом порядке.



#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <cmath>

struct Node {
    std::unordered_map<char, int64_t> next;
    bool is_terminal = false;
};

class SetString {
public:
    explicit SetString(int l) : len_(l){};

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

    int64_t FindCount(std::string &str) {
        std::shared_ptr<Node> cur_node = tree_[0];
        for (auto &symbol : str) {
            cur_node = tree_[cur_node->next[symbol]];
        }
        int len = str.size();
        int64_t sum = 0;
        for (auto &symbol : cur_node->next) {
            sum += CountTaken(tree_[cur_node->next[symbol.first]], len + 1);
        }
        return std::pow(10, len_ - len) - sum;
    }

    int64_t CountTaken(std::shared_ptr<Node> &cur_node, int len) {
        int64_t sum = 0;
        if (cur_node->is_terminal) {
            return std::pow(10, len_ - len);
        }
        for (auto &symbol : cur_node->next) {
            sum += CountTaken(tree_[cur_node->next[symbol.first]], len + 1);
        }
        return sum;
    }

private:
    std::vector<std::shared_ptr<Node>> tree_;
    int len_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int m = 0;
    int len = 0;
    std::cin >> m >> len;
    std::unordered_map<std::string, std::string> telephones;
    SetString s(len);
    for (int i = 0; i < m; ++i) {
        std::string number;
        std::string city;
        std::cin >> number >> city;
        telephones[city] = number;
        s.Insert(number);
    }
    for (auto &cities : telephones) {
        std::cout << cities.first << " " << s.FindCount(cities.second) << "\n";
    }
    return 0;
}
