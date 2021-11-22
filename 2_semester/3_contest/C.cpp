// Реализуйте статическую хеш-таблицу на известном фиксированном наборе ключей с константным поиском в худшем случае.

// Формат ввода
// На вход программе подается N (1 ≤ N ≤ 10^5) чисел — элементы статической таблицы, а затем K (1 ≤ K ≤ 3 ⋅ 10^6) запросов поиска числа.
// Формат вывода
// На каждый из K запросов необходимо ответить YES, если число присутствует в таблице, и NO — иначе. 


#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <optional>

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

template <class KeyT, class HashT, class EqualT = std::equal_to<KeyT>>
class FrozenSet {
private:
    std::vector<std::vector<std::optional<KeyT>>> frozen_set_;
    size_t size_;
    HashT hash_func_;

public:
    FrozenSet() : frozen_set_(0), size_(0) {
    }
    template <typename ForwardIterator>
    FrozenSet(ForwardIterator beg, ForwardIterator end, std::mt19937_64 generator) {
        size_ = std::distance(beg, end);
        frozen_set_.resize(size_);
        hash_func_ = UniversalHash::GenerateHash(generator);
        for (auto it = beg; it != end; ++it) {
            frozen_set_[hash_func_(*it) % size_].push_back(*it);
        }
        for (size_t i = 0; i < frozen_set_.size(); ++i) {
            std::vector<std::optional<KeyT>> hash_table;
            bool right_hash = true;
            while (true) {
                hash_table.resize(frozen_set_[i].size() * frozen_set_[i].size());
                HashT hash_i = UniversalHash::GenerateHash(generator);
                for (size_t j = 0; j < frozen_set_[i].size(); ++j) {
                    KeyT value = frozen_set_[i][j].value();
                    if (!hash_table[hash_i(value) % hash_table.size()].has_value()) {
                        hash_table[hash_i(value) % hash_table.size()] = value;
                    } else {
                        hash_table.clear();
                        right_hash = false;
                        break;
                    }
                }
                if (right_hash) {
                    break;
                }
                right_hash = true;
            }
            frozen_set_[i] = hash_table;
        }
    }

    template <typename ForwardIterator>
    void Assign(ForwardIterator beg, ForwardIterator end, std::mt19937_64 generator) {
        frozen_set_.clear();
        size_ = std::distance(beg, end);
        frozen_set_.resize(size_);
        hash_func_ = UniversalHash::GenerateHash(generator);
        for (auto it = beg; it != end; ++it) {
            frozen_set_[hash_func_(*it) % size_].push_back(*it);
        }
        for (size_t i = 0; i < frozen_set_.size(); ++i) {
            std::vector<std::optional<KeyT>> hash_table;
            bool right_hash = true;
            while (true) {
                hash_table.resize(frozen_set_[i].size() * frozen_set_[i].size());
                HashT hash_i = UniversalHash::GenerateHash(generator);
                for (size_t j = 0; j < frozen_set_[i].size(); ++j) {
                    KeyT value = frozen_set_[i][j].value();
                    if (!hash_table[hash_i(value) % hash_table.size()].has_value()) {
                        hash_table[hash_i(value) % hash_table.size()] = value;
                    } else {
                        hash_table.clear();
                        right_hash = false;
                        break;
                    }
                }
                if (right_hash) {
                    break;
                }
                right_hash = true;
            }
            frozen_set_[i] = hash_table;
        }
    }
    size_t Size() const {
        return size_;
    }
    bool Empty() const {
        return (size_ == 0);
    }
    void Clear() {
        frozen_set_.clear();
        size_ = 0;
    }
    bool Find(KeyT key) const {
        if (size_ == 0) {
            return false;
        }
        std::vector<std::optional<KeyT>> hash_table = frozen_set_[hash_func_(key) % size_];
        for (auto& it : hash_table) {
            if (it.has_value()) {
                if (it.value() == key) {
                    return true;
                }
            }
        }
        return false;
    }
};

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int num;
    std::cin >> num;
    std::vector<int> numbers;
    for (size_t i = 0; i < num; ++i) {
        int digit;
        std::cin >> digit;
        numbers.push_back(digit);
    }
    std::mt19937_64 generator;
    FrozenSet<uint32_t, UniversalHash> fs(numbers.begin(), numbers.end(), generator);
    int num_questions;
    std::cin >> num_questions;
    for (size_t i = 0; i < num_questions; ++i) {
        int question;
        std::cin >> question;
        if (fs.Find(question)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    return 0;
}
