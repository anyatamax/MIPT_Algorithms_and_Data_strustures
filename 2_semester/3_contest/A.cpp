// Реализовать универсальное семейство хеш функций для целых чисел от 0 до 10^9. Воспользоваться реализованным семейством (с произвольно выбранными параметрами) и std::unordered_set для решения следующей задачи.
// Нужно реализовать структуру данных множество, способную быстро выполнять следующие операции:
//     «+ N» — добавить число N в множество. Не гарантируется, что N отсутствует в множестве.
//     «- N» — удалить число N из множества. Не гарантируется, что N имеется в множестве.
//     «? N» — узнать, есть ли число N в множестве.

// Формат ввода
// В первой строке идет число N (1 ≤ N ≤ 10^6) — число запросов к множеству. Далее идет N запросов на N строках в формате выше.
// Формат вывода
// Для каждого запроса третьего типа вывести YES, если ответ положителен, и NO — иначе. 


#include <iostream>
#include <string>
#include <unordered_set>
#include <random>

class UniversalHash {
private:
    int64_t first_;
    int64_t second_;

public:
    static const int64_t kMaxValue = 1000000008;
    explicit UniversalHash(int64_t first = 1, int64_t second = 0) : first_(first), second_(second) {
    }
    int64_t operator()(const int64_t number) const {
        return (first_ * number + second_) % (kMaxValue + 1);
    }
    static UniversalHash GenerateHash(std::mt19937_64 generator) {
        std::random_device device;
        auto seed = (static_cast<uint64_t>(device()) << 32) | device();
        generator.seed(seed);
        std::uniform_int_distribution<uint64_t> uniform(0, kMaxValue);
        return UniversalHash(uniform(generator), uniform(generator));
    }
};

int main () {
    std::unordered_set<int, UniversalHash> func;
    int num;
    std::cin >> num;
    for (size_t i = 0; i < num; ++i) {
        char type;
        int digit;
        std::cin >> type >> digit;
        if (type == '+') {
            func.insert(digit);
        }
        if (type == '-') {
            func.erase(digit);
        }
        if (type == '?') {
            if (func.find(digit) != func.end()) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }
        }
    }
    return 0;
}
