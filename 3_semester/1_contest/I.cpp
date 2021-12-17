

// Вы уже решали задачу RMQ, даже если есть обновления на отрезке. В этот раз границы решения этой задачи будут расширены. А именно, надо разработать структуру данных в виде массива, который умеет обрабатывать следующие запросы:

//     «? l r» — найти минимум на отрезке [l, r] в 1-индексации;
//     «+ i x» — добавить элемент x после i-го (в 1-индексации) элемента. Если i = 0, то вставка идет в начало списка.

// Изначально структура пустая.

// Формат ввода
// Первая строка входного файла содержит единственное целое число n — число операций над массивом (1 ≤ n ≤ 2 ⋅ 10^5). Следующие n строк описывают сами операции. Все операции добавления являются корректными. Все числа, хранящиеся в массиве, по модулю не превосходят 109.
// Формат вывода
// Для каждой операции в отдельной строке выведите её результат.


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
    void Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, int key);
    void Insert(int position, std::shared_ptr<BST>& new_bst);
    long int Query(int left, int right);

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

void BSTree::Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, int key) {
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

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    BSTree bst_treap;
    int n = 0;
    std::cin >> n;
    char task = ' ';
    for (size_t i = 0; i < n; ++i) {
        std::cin >> task;
        if (task == '+') {
            int index = 0;
            long int elem = 0;
            std::cin >> index >> elem;
            std::shared_ptr<BST> new_bst(new BST);
            new_bst->left = nullptr;
            new_bst->right = nullptr;
            new_bst->value = elem;
            new_bst->cnt = 1;
            new_bst->priority = std::hash<long int>{}(elem);
            new_bst->result = MAX_VALUE;
            bst_treap.Insert(index, new_bst);
        } else {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            std::cout << bst_treap.Query(left - 1, right - 1) << "\n";
        }
    }
    return 0;
}
