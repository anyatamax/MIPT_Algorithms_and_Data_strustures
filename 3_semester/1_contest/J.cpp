

// Мы опять решаем задачу RMQ, только теперь нужно обработать следующие запросы:

//     «1 l r» — развернуть отрезок [l, r].
//     «2 l r» — найти минимум на отрезке [l, r];

// Формат ввода
// Первая строка файла содержит два числа n, m. (1 ≤ n, m ≤ 10^5) Во второй строке находится n чисел ai (1-10^9 ≤ ai ≤ 10^9) — исходный массив. Остальные m строк содержат запросы в формате, описанном в условии. Для чисел l, r выполняется ограничение (1≤ l ≤ r ≤ n).
// Формат вывода
// На каждый запрос типа 2, во входной файл выведите ответ на него, в отдельной строке.


#include <iostream>
#include <memory>

const long int MAX_VALUE = 5000000001;

struct BST {
    std::shared_ptr<BST> left = nullptr;
    std::shared_ptr<BST> right = nullptr;
    long int value = 0;
    int cnt = 0;
    int priority = 0;
    long int result = 0;
    bool reverse = false;
    BST() = default;
};

class BSTree {
public:
    BSTree() : root_(nullptr) {};
    BSTree(const BSTree& tree) = default;
    int Cnt(std::shared_ptr<BST>& node) const;
    long int Result(std::shared_ptr<BST>& node) const;
    void UpdateCnt(std::shared_ptr<BST>& node);
    void UpdateResult(std::shared_ptr<BST>& node);
    void Push(std::shared_ptr<BST> node);
    std::shared_ptr<BST> Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right);
    void Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, int key);
    void Insert(int position, std::shared_ptr<BST>& new_bst);
    long int Query(int left, int right);
    void Reverse(int left, int right);

private:
    std::shared_ptr<BST> root_;
};

int BSTree::Cnt(std::shared_ptr<BST>& node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->cnt;
}

long int BSTree::Result(std::shared_ptr<BST>& node) const {
    if (node == nullptr) {
        return MAX_VALUE;
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
        node->result = std::min(std::min(Result(node->left), node->value), Result(node->right));
    }
}

void BSTree::Push(std::shared_ptr<BST> node) {
    if (node != nullptr && node->reverse != 0) {
        node->reverse = false;
        std::swap(node->left, node->right);
        if (node->left != nullptr) {
            node->left->reverse ^= true;
        }
        if (node->right != nullptr) {
            node->right->reverse ^= true;
        }
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
        Push(left);
        left->right = Merge(left->right, right);
        UpdateCnt(left);
        UpdateResult(left);
        return left;
    } else {
        Push(right);
        right->left = Merge(left, right->left);
        UpdateResult(right);
        UpdateCnt(right);
        return right;
    }
}

void BSTree::Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, int key) {
    if (root == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    Push(root);
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

long int BSTree::Query(int left, int right) {
    std::shared_ptr<BST> node_less_left(nullptr);
    std::shared_ptr<BST> node_more_left(nullptr);
    Split(root_, node_less_left, node_more_left, left);
    std::shared_ptr<BST> node_more_right(nullptr);
    std::shared_ptr<BST> node_more_left_less_right(nullptr);
    Split(node_more_left, node_more_left_less_right, node_more_right, right - left + 1);
    long int result = Result(node_more_left_less_right);
    node_more_left = Merge(node_more_left_less_right, node_more_right);
    root_ = Merge(node_less_left, node_more_left);
    return result;
}

void BSTree::Reverse(int left, int right) {
    std::shared_ptr<BST> less_left(nullptr), more_left(nullptr), more_right(nullptr);
    Split(root_, less_left, more_left, left);
    Split(more_left, more_left, more_right, right - left + 1);
    more_left->reverse ^= true;
    root_ = Merge(less_left, more_left);
    root_ = Merge(root_, more_right);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    BSTree bst_treap;
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    int task = 0;
    for (size_t i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        std::shared_ptr<BST> new_bst(new BST);
        new_bst->left = nullptr;
        new_bst->right = nullptr;
        new_bst->priority = std::hash<int>{}(value);
        new_bst->value = value;
        new_bst->cnt = 1;
        new_bst->reverse = false;
        bst_treap.Insert(i, new_bst);
    }
    for (size_t i = 0; i < m; ++i) {
        std::cin >> task;
        if (task == 1) {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            bst_treap.Reverse(left - 1, right - 1);
        } else {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            std::cout << bst_treap.Query(left - 1, right - 1) << "\n";
        }
    }
    return 0;
}
