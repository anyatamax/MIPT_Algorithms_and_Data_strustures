

// Физики проводят эксперимент для исследования частиц трёх типов: x, y и z. Они запускают в коллайдер пронумерованный ряд из n частиц. Во время эксперимента происходит воздействие на одну конкретную частицу, после чего частица исчезает с i-го места ряда и моментально появляется на месте j. После её исчезновения номера частиц, стоящих правее, уменьшаются на 1, а после появления, номера частиц, стоящих правее, увеличиваются на 1. После определенного числа воздействий физики интересуются какая частица стоит на месте k. Напишите программу, которая поможет физикам.

// Формат ввода
// В первой строке файла два целых числа: n — количество частиц и m — общее количество воздействий и вопросов (1 ≤ n ≤ 10^6, 1 ≤ m ≤ 15000). Во второй строке — последовательность из символов x, y и z длиной n. На каждой из следующих m строк (1 ≤ m ≤ 15000) описано воздействие или вопрос. Строка, в которой описано воздействие, начинается символом a и после пробела дается два целых числа из отрезка [1, n]. Первое из них — положение до взаимодействия, а второе — после. Строка, в которой описан вопрос, начинается символом q и после пробела дается одно целое число из отрезка [1, n]. Оно указывает позицию, которая интересует физиков.
// Формат вывода
// Выведите столько строк, сколько вопросов во входном файле. В строке номер i надо записать ответ на вопрос i — название соответствующей частицы x, y или z. 


#include <cstdio>
#include <ctime>
#include <iostream>
#include <memory>

struct BST {
    std::shared_ptr<BST> left = nullptr;
    std::shared_ptr<BST> right = nullptr;
    int value = 0;
    int cnt = 0;
    int priority = 0;
    BST() = default;
};

class BSTree {
public:
    BSTree() : root_(nullptr) {};
    BSTree(int prior, int val) : root_(new BST) {
        root_->priority = prior;
        root_->value = val;
        root_->cnt = 1;
    }
    int Cnt(std::shared_ptr<BST>& node) const;
    void UpdateCnt(std::shared_ptr<BST>& node) const;
    std::shared_ptr<BST> Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right);
    void Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, int key);
    void Insert(int position, std::shared_ptr<BST>& new_bst);
    std::shared_ptr<BST> Remove(int position);
    char Get(const std::shared_ptr<BST>& root, int pos);
    std::shared_ptr<BST> GetRoot() const {
        return root_;
    }

private:
    std::shared_ptr<BST> root_;
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

std::shared_ptr<BST> BSTree::Remove(int position) {
    std::shared_ptr<BST> less_pos(nullptr);
    std::shared_ptr<BST> more_pos(nullptr);
    Split(root_, less_pos, more_pos, position);
    std::shared_ptr<BST> pos(nullptr);
    std::shared_ptr<BST> without_pos(nullptr);
    Split(more_pos, pos, without_pos, 1);
    root_ =  Merge(less_pos, without_pos);
    return pos;
}

char BSTree::Get(const std::shared_ptr<BST>& root, int pos) {
    if (pos == Cnt(root->left)) {
        return root->value;
    } else if (pos < Cnt(root->left)) {
        return Get(root->left, pos);
    } else {
        return Get(root->right, pos - Cnt(root->left) - 1);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    BSTree bst_treap;
    srand(time(NULL));
    int n = 0;
    scanf("%d", &n);
    char symbol = ' ';
    char task = ' ';
    int m = 0;
    scanf("%d", &m);
    scanf("%c", &symbol);
    for (size_t i = 0; i < n; i++) {
        scanf("%c", &symbol);
        std::shared_ptr<BST> new_bst(new BST);
        new_bst->left = nullptr;
        new_bst->right = nullptr;
        new_bst->priority = rand();
        new_bst->value = symbol;
        new_bst->cnt = 1;
        bst_treap.Insert(i, new_bst);
    }
    for (size_t i = 0; i < m; ++i) {
        scanf("%c", &task);
        if (task == '\n') {
            scanf("%c", &task);
        }
        if (task == 'a') {
            int prev = 0;
            int next = 0;
            scanf("%d%d", &prev, &next);
            std::shared_ptr<BST> delete_pos(bst_treap.Remove(prev - 1));
            bst_treap.Insert(next - 1, delete_pos);
        } else {
            int position = 0;
            scanf("%d", &position);
            printf("%c\n", bst_treap.Get(bst_treap.GetRoot(), position - 1));
        }
    }
    return 0;
}
