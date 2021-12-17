

// В этот раз вам предстоит решить очередную вариацию задачи RSQ. А именно, нужно разработать структуру данных в виде массива, умеющую делать следующее:

//     Взять отрезок чётной длины от x до y и поменять местами число x с x + 1, x + 2 с x + 3, и т.д.
//     Посчитать сумму чисел на произвольном отрезке от a до b.

// Формат ввода
// Во входном файле заданы один или несколько тестов.
// В первой строке каждого теста записаны число N — длина последовательности и число M — число операций (1 ≤ N, M ≤ 10^5). Во второй строке теста содержится N целых чисел, не превосходящих 106 по модулю — сама последовательность. Далее следуют M строк — запросы в формате «1 xi yi» — запрос первого типа, и «2 ai bi» — запрос второго типа. Сумма всех N и M по всему файлу не превосходит 2 ⋅ 10^5.
// Файл завершается строкой из двух нулей. Гарантируется, что запросы корректны.
// Формат вывода
// Для каждого теста выведите ответы на запросы второго типа, как показано в примере. Разделяйте ответы на тесты пустой строкой.


#include <ctime>
#include <iostream>
#include <memory>

struct BST {
    std::shared_ptr<BST> left = nullptr;
    std::shared_ptr<BST> right = nullptr;
    long int value = 0;
    int cnt = 0;
    int priority = 0;
    long int result = 0;
    BST() = default;
};

class BSTree {
public:
    BSTree() : root_(nullptr) {};
    BSTree(const BSTree& tree) = default;
    int Cnt(std::shared_ptr<BST>& node);
    long int Result(std::shared_ptr<BST>& node);
    void UpdateCnt(std::shared_ptr<BST>& node);
    void UpdateResult(std::shared_ptr<BST>& node);
    std::shared_ptr<BST> Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right);
    void Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, long long int key);
    void Insert(int position, std::shared_ptr<BST>& new_bst);
    std::shared_ptr<BST> GetRoot() const {
        return root_;
    }
    void SetRoot(const std::shared_ptr<BST>& root) {
        root_ = root;
    }

private:
    std::shared_ptr<BST> root_;
};

int BSTree::Cnt(std::shared_ptr<BST>& node) {
    if (node == nullptr) {
        return 0;
    }
    return node->cnt;
}

long int BSTree::Result(std::shared_ptr<BST>& node) {
    if (node == nullptr) {
        return 0;
    }
    return node->result;
}

void BSTree::UpdateCnt(std::shared_ptr<BST>& node) {
    if (node != nullptr) {
        node->cnt = Cnt(node->left) + Cnt(node->right) + 1;
    }
}

void BSTree::UpdateResult(std::shared_ptr<BST>& node) {
    if (node != nullptr) {
        node->result = Result(node->left) + node->value + Result(node->right);
    }
}

std::shared_ptr<BST> BSTree::Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right) {
    if (left == nullptr) {
        UpdateResult(right);
        UpdateCnt(right);
        return right;
    } else if (right == nullptr) {
        UpdateCnt(left);
        UpdateResult(left);
        return left;
    }
    if (left->priority > right->priority) {
        left->right = Merge(left->right, right);
        UpdateCnt(left);
        UpdateResult(left);
        return left;
    } else {
        right->left = Merge(left, right->left);
        UpdateResult(right);
        UpdateCnt(right);
        return right;
    }
}

void BSTree::Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, long long int key) {
    if (root == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    if (Cnt(root->left) < key) {
        Split(root->right, root->right, right, key - Cnt(root->left) - 1);
        left = root;
    } else {
        Split(root->left, left, root->left, key);
        right = root;
    }
    UpdateCnt(left);
    UpdateCnt(right);
    UpdateResult(left);
    UpdateResult(right);
}

void BSTree::Insert(int position, std::shared_ptr<BST>& new_bst) {
    std::shared_ptr<BST> left, right;
    Split(root_, left, right, position);
    root_ = Merge(Merge(left, new_bst), right);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    srand(time(nullptr));
    int n = 0;
    int m = 0;
    int count = 1;
    std::cin >> n >> m;
    std::shared_ptr<BST> less_right_first(nullptr), more_right_first(nullptr), answer_first(nullptr),
            less_right_second(nullptr), more_right_second(nullptr), answer_second(nullptr);
    while (n != 0 && m != 0) {
        BSTree bst_first;
        BSTree bst_second;
        int pos_first = 0;
        int pos_second = 0;
        for (size_t i = 0; i < n; ++i) {
            int value = 0;
            std::cin >> value;
            std::shared_ptr<BST> new_bst(new BST);
            new_bst->left = nullptr;
            new_bst->right = nullptr;
            new_bst->priority = rand() << 10 + rand();
            new_bst->value = value;
            new_bst->cnt = 1;
            if (i % 2 == 0) {
                bst_first.Insert(pos_first, new_bst);
                pos_first++;
            } else {
                bst_second.Insert(pos_second, new_bst);
                pos_second++;
            }
        }
        int task = 0;
        std::cout << "Swapper " << count << ":\n";
        long long int left, right, left_first, left_second, right_second, right_first;
        for (size_t i = 0; i < m; ++i) {
            std::cin >> task >> left >> right;
            left_second = (left % 2 == 0) ? left / 2 - 1 : left / 2;
            left_first = left / 2;
            right_first = (right % 2 == 0) ? right / 2 : right / 2 + 1;
            right_second = right / 2;
            if (task == 1) {
                bst_first.Split(bst_first.GetRoot(), less_right_first, more_right_first, right_first);
                bst_first.Split(less_right_first, less_right_first, answer_first, left_first);
                bst_second.Split(bst_second.GetRoot(), less_right_second, more_right_second, right_second);
                bst_second.Split(less_right_second, less_right_second, answer_second, left_second);
                less_right_first = bst_first.Merge(less_right_first, answer_second);
                bst_first.SetRoot(bst_first.Merge(less_right_first, more_right_first));
                less_right_second = bst_second.Merge(less_right_second, answer_first);
                bst_second.SetRoot(bst_second.Merge(less_right_second, more_right_second));
            } else {
                bst_first.Split(bst_first.GetRoot(), less_right_first, more_right_first, right_first);
                bst_first.Split(less_right_first, less_right_first, answer_first, left_first);
                bst_second.Split(bst_second.GetRoot(), less_right_second, more_right_second, right_second);
                bst_second.Split(less_right_second, less_right_second, answer_second, left_second);
                std::cout << bst_first.Result(answer_first) + bst_second.Result(answer_second) << "\n";
                less_right_first = bst_first.Merge(less_right_first, answer_first);
                bst_first.SetRoot(bst_first.Merge(less_right_first, more_right_first));
                less_right_second = bst_second.Merge(less_right_second, answer_second);
                bst_second.SetRoot(bst_second.Merge(less_right_second, more_right_second));
            }
        }
        count++;
        std::cin >> n >> m;
    }
    return 0;
}
