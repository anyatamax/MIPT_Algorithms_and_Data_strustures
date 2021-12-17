

// Генерал Дукар любит командовать своей армией. Его любимый приказ «в начало строя». Он выстраивает свою армию в шеренгу и оглашает последовательность приказов. Каждый приказ имеет вид «Солдаты с li по ri — в начало строя!»
// Пронумеруем солдат в начальном положении с 1 до n, слева направо. Приказ «Солдаты с li по ri — в начало строя!» означает, что солдаты, стоящие с li по ri включительно, перемещаются в начало строя, сохраняя относительный порядок.
// Например, если в некоторый момент солдаты стоят в порядке 2,3,6,1,5,4, после приказа: «Солдаты с 2 по 4 — в начало строя!» порядок будет 3,6,1,2,5,4.
// По данной последовательности приказов найти конечный порядок солдат в строю.

// Формат ввода
// В первой строке два целых числа n and m (2≤n≤100000, 1≤m≤100000) — количество солдат и количество приказов. Следующие m строк содержат по два целых числа li и ri (1≤li≤ri≤n).
// Формат вывода
// Выведите n целых чисел — порядок солдат в конечном положении после выполнения всех приказов.


#include <iostream>
#include <memory>

struct BST {
    std::shared_ptr<BST> left = nullptr;
    std::shared_ptr<BST> right = nullptr;
    int value = 0;
    int cnt = 0;
    int priority = 0;
};

class BSTree {
public:
    void Insert(int position, std::shared_ptr<BST>& new_bst);
    void Front(int left, int right);
    std::shared_ptr<BST> GetRoot() const {
        return root_;
    }
    void BuildNewNode(std::shared_ptr<BST>& new_bst, int index) const;
    void Print(const std::shared_ptr<BST>& root) const;

private:
    std::shared_ptr<BST> root_;

    int Cnt(std::shared_ptr<BST>& node) const;
    void UpdateCnt(std::shared_ptr<BST>& node) const;
    std::shared_ptr<BST> Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right);
    void Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, int key);
};

int BSTree::Cnt(std::shared_ptr<BST>& node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->cnt;
}

void BSTree::UpdateCnt(std::shared_ptr<BST>& node) const {
    if (node != nullptr) {
        node->cnt = Cnt(node->left) + Cnt(node->right) + 1;
    }
}

std::shared_ptr<BST> BSTree::Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right) {
    if (left == nullptr) {
        return right;
    } else if (right == nullptr) {
        return left;
    }
    if (left->priority > right->priority) {
        left->right = Merge(left->right, right);
        UpdateCnt(left);
        return left;
    } else {
        right->left = Merge(left, right->left);
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
    UpdateCnt(root);
}

void BSTree::Insert(int position, std::shared_ptr<BST>& new_bst) {
    std::shared_ptr<BST> left, right;
    Split(root_, left, right, position);
    root_ =  Merge(Merge(left, new_bst), right);
}

void BSTree::Front(int left, int right) {
    std::shared_ptr<BST> less_right(nullptr), more_right(nullptr), less_left(nullptr), more_left_less_right(nullptr);
    Split(root_, less_right, more_right, right + 1);
    Split(less_right, less_left, more_left_less_right, left);
    root_ =  Merge(Merge(more_left_less_right, less_left), more_right);
}

void BSTree::BuildNewNode(std::shared_ptr<BST>& new_bst, int index) const {
    new_bst->left = nullptr;
    new_bst->right = nullptr;
    new_bst->priority = std::hash<int>{}(index + 1);
    new_bst->value = index + 1;
    new_bst->cnt = 1;
}

void BSTree::Print(const std::shared_ptr<BST>& root) const {
    if (root == nullptr) {
        return;
    }
    Print(root->left);
    std::cout << root->value << " ";
    Print(root->right);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    BSTree bst_treap;
    int n = 0;
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        std::shared_ptr<BST> new_bst(new BST);
        bst_treap.BuildNewNode(new_bst, i);
        bst_treap.Insert(i, new_bst);
    }
    int m = 0;
    std::cin >> m;
    for (size_t i = 0; i < m; ++i) {
        int left = 0;
        int right = 0;
        std::cin >> left >> right;
        bst_treap.Front(left - 1, right - 1);
    }
    bst_treap.Print(std::shared_ptr<BST>(bst_treap.GetRoot()));
    return 0;
}
