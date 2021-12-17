

// Реализуйте структуру данных, которая поддерживает множество S целых чисел, с которым разрешается производить следующие операции:

//     add(i) — добавить в множество S число i (если оно там уже есть, то множество не меняется);
//     sum(l, r) — вывести сумму всех элементов x из S, которые удовлетворяют неравенствуl ≤ x ≤ r.

// Формат ввода

// Исходно множество S пусто. Первая строка входного файла содержит n — количество операций (1 ≤ n ≤ 3 ⋅ 10^5). Следующие n строк содержат операции. Каждая операция имеет вид либо «+ i», либо «? l r». Операция «? l r» задает запрос sum(l, r).

// Если операция «+ i» идет во входном файле в начале или после другой операции «+», то она задает операцию add(i). Если же она идет после запроса «?», и результат этого запроса был y, то выполняется операция add((i + y) % 10^9).

// Во всех запросах и операциях добавления параметры лежат в интервале от 0 до 10^9.



#include <cstdio>
#include <ctime>
#include <iostream>
#include <memory>

const int MOD_VALUE = 1000000000;

struct BST {
    std::shared_ptr<BST> left = nullptr;
    std::shared_ptr<BST> right = nullptr;
    long long int key = 0;
    long long int result = 0;
    long long int priority = 0;
};

class BSTree {
public:
    long long int Query(long long int left, long long int right);
    void Insert(std::shared_ptr<BST> key);
    long long int Find(long long int search);
    void BuildNewNode(std::shared_ptr<BST>& new_bst, long long int key) const;

private:
    std::shared_ptr<BST> root_;
    const long long int MAX_VALUE = 10000000000000000;

    long long int GetNodeResult(std::shared_ptr<BST>& node) const;
    void UpdateNode(std::shared_ptr<BST>& node);
    std::shared_ptr<BST> Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right);
    void Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, long long int key);
};

long long int BSTree::GetNodeResult(std::shared_ptr<BST>& node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->result;
}

void BSTree::UpdateNode(std::shared_ptr<BST>& node) {
    if (node == nullptr) {
        return;
    }
    node->result = GetNodeResult(node->left) + node->key + GetNodeResult(node->right);
}

std::shared_ptr<BST> BSTree::Merge(std::shared_ptr<BST> left, std::shared_ptr<BST> right) {
    if (left == nullptr) {
        UpdateNode(right);
        return right;
    } else if (right == nullptr) {
        UpdateNode(left);
        return left;
    }
    if (left->priority > right->priority) {
        left->right = Merge(left->right, right);
        UpdateNode(left);
        return left;
    } else {
        right->left = Merge(left, right->left);
        UpdateNode(right);
        return right;
    }
}

void BSTree::Split(std::shared_ptr<BST> root, std::shared_ptr<BST>& left, std::shared_ptr<BST>& right, long long int key) {
    if (root == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    if (root->key < key) {
        Split(root->right, root->right, right, key);
        left = root;
    } else {
        Split(root->left, left, root->left, key);
        right = root;
    }
    UpdateNode(left);
    UpdateNode(right);
}


long long int BSTree::Query(long long int left, long long int right) {
    std::shared_ptr<BST> node_less_left(nullptr);
    std::shared_ptr<BST> node_more_left(nullptr);
    Split(root_, node_less_left, node_more_left, left);
    std::shared_ptr<BST> node_more_right(nullptr);
    std::shared_ptr<BST> node_more_left_less_right(nullptr);
    Split(node_more_left, node_more_left_less_right, node_more_right, right);
    long long int result = GetNodeResult(node_more_left_less_right);
    node_more_left = Merge(node_more_left_less_right, node_more_right);
    root_ = Merge(node_less_left, node_more_left);
    return result;
}

void BSTree::Insert (std::shared_ptr<BST> key) {
    std::shared_ptr<BST> left(nullptr), right(nullptr);
    Split(root_, left, right, key->key);
    root_ = Merge(Merge(left, key), right);
}


long long int BSTree::Find(long long int search) {
    std::shared_ptr<BST> copy_root = root_;
    long long int min_greater_search = MAX_VALUE;
    while (copy_root != nullptr) {
        if (copy_root->key < search) {
            copy_root = copy_root->right;
        } else if (copy_root->key > search) {
            min_greater_search = std::min(copy_root->key, min_greater_search);
            copy_root = copy_root->left;
        } else {
            min_greater_search = copy_root->key;
            break;
        }
    }
    return min_greater_search;
}

void BSTree::BuildNewNode(std::shared_ptr<BST>& new_bst, long long int key) const {
    new_bst->left = nullptr;
    new_bst->right = nullptr;
    new_bst->priority = rand() << 10 + rand();
    new_bst->key = key;
    new_bst->result = 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    BSTree bst_tree;
    srand(time(nullptr));
    int n = 0;
    char task = ' ';
    char prev_task = ' ';
    long long int prev_result = 0;
    scanf("%d", &n);
    for (size_t i = 0; i < n; ++i) {
        scanf("%c", &task);
        if (task == '\n') {
            scanf("%c", &task);
        }
        if (task == '?') {
            long long int l, r;
            scanf("%lld%lld", &l, &r);
            prev_result = bst_tree.Query(l, r + 1);
            printf("%lld\n", prev_result);
            prev_task = task;
        } else {
            long long int key;
            scanf("%lld", &key);
            if (prev_task == '?') {
                key = (key % MOD_VALUE + prev_result % MOD_VALUE) % MOD_VALUE;
                if (bst_tree.Find(key) == key) {
                    prev_task = task;
                    continue;
                }
                std::shared_ptr<BST> new_bst(new BST);
                bst_tree.BuildNewNode(new_bst, key);
                bst_tree.Insert(new_bst);
            } else {
                if (bst_tree.Find(key) == key) {
                    prev_task = task;
                    continue;
                }
                std::shared_ptr<BST> new_bst(new BST);
                bst_tree.BuildNewNode(new_bst, key);
                bst_tree.Insert(new_bst);
            }
            prev_task = task;
        }
    }
    return 0;
}
