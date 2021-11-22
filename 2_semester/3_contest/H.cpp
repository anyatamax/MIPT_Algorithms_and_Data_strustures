//AVL


#include <iostream>

struct AVL {
    int key;
    int height;
    AVL* left;
    AVL* right;
    AVL(int k) {
        key = k;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

int Height(AVL* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int BalanceFactor(AVL* node) {
    return (Height(node->right) - Height(node->left));
}

void CorrectHeight(AVL* node) {
    int height_first = Height(node->left);
    int height_second = Height(node->right);
    node->height = std::max(height_first, height_second) + 1;
}

AVL* RightRotation(AVL* node) {
    AVL* new_node = node->left;
    node->left = new_node->right;
    new_node->right = node;
    CorrectHeight(node);
    CorrectHeight(new_node);
    return new_node;
}

AVL* LeftRotation(AVL* node)
{
    AVL* new_node = node->right;
    node->right = new_node->left;
    new_node->left = node;
    CorrectHeight(node);
    CorrectHeight(new_node);
    return new_node;
}

AVL* BalanceTree(AVL* node) {
    CorrectHeight(node);
    if (BalanceFactor(node) == 2) {
        if (BalanceFactor(node->right) < 0)
            node->right = RightRotation(node->right);
        return LeftRotation(node);
    }
    if (BalanceFactor(node) == -2) {
        if (BalanceFactor(node->left) > 0)
            node->left = LeftRotation(node->left);
        return RightRotation(node);
    }
    return node;
}

AVL* Insert(AVL* root, int value) {
    if (root == nullptr) {
        root = new AVL(value);
        return root;
    }
    if (root->key > value) {
        root->left = Insert(root->left, value);
    } else if (root->key < value) {
        root->right = Insert(root->right, value);
    } else {
        return root;
    }
    return BalanceTree(root);
}

int FindNext(AVL* root, int search) {
    AVL* copy_root = root;
    int min_greater_search = 1000000001;
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

int Next(AVL* root, int key) {
    int next_min = FindNext(root, key);
    if (next_min == 1000000001) {
        return -1;
    }
    return next_min;
}

int main () {
    AVL* tree(nullptr);
    char prev_task = ' ';
    int prev_next_return;
    int num;
    std::cin >> num;
    for (size_t i = 0; i < num; ++i) {
        int digit;
        char task;
        std::cin >> task >> digit;
        if (task == '?') {
            prev_next_return = Next(tree, digit);
            std::cout << prev_next_return << "\n";
            prev_task = task;
        } else {
            if (prev_task == '?') {
                tree = Insert(tree, (digit + prev_next_return) % 1000000000);
            } else {
                tree = Insert(tree, digit);
            }
            prev_task = task;
        }
    }
    return 0;
}
