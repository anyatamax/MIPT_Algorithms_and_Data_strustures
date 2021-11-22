// Выведите второй по величине элемент в построенном дереве. Гарантируется, что такой найдется.

// Формат ввода
// Дана последовательность целых чисел, оканчивающаяся нулем. Сам ноль в последовательность не входит.
// Формат вывода
// Выведите ответ на задачу.


#include <iostream>
#include <string>
#include <vector>

struct Node {
    int key;
    Node* left;
    Node* right;
    Node* parent;
};

Node* Find(Node** root, int search) {
    Node* copy_root = *root;
    while (true) {
        if (copy_root->key < search) {
            if (copy_root->right == nullptr) {
                break;
            }
            copy_root = copy_root->right;
        } else if (copy_root->key > search) {
            if (copy_root->left == nullptr) {
                break;
            }
            copy_root = copy_root->left;
        } else {
            break;
        }
    }
    return copy_root;
}

void Insert(Node** root, int value) {
    if (*root == nullptr) {
        *root = new Node;
        (*root)->key = value;
        (*root)->parent = nullptr;
        (*root)->left = nullptr;
        (*root)->right = nullptr;
        return;
    }
    Node *find_node = Find(root, value);
    if (find_node->key != value) {
        if (value > find_node->key) {
            Node* new_node = new Node;
            new_node->key = value;
            new_node->parent = find_node;
            new_node->left = nullptr;
            new_node->right = nullptr;
            find_node->right = new_node;
        } else {
            Node* new_node = new Node;
            new_node->key = value;
            new_node->parent = find_node;
            new_node->left = nullptr;
            new_node->right = nullptr;
            find_node->left = new_node;
        }
    }
}

int HeightTree(Node** root) {
    int left, right;
    int height = 0;
    if (*root != nullptr) {
        left = HeightTree(&(*root)->left);
        right = HeightTree(&(*root)->right);
        height = std::max(left, right) + 1;
    }
    return height;
}

void PrintTree(Node** root) {
    if (*root == nullptr) {
        return;
    }
    PrintTree(&(*root)->left);
    std::cout << (*root)->key << "\n";
    PrintTree(&(*root)->right);
}

int Max(Node** root) {
    Node* copy_root = *root;
    while(copy_root->right != nullptr) {
        copy_root = copy_root->right;
    }
    return copy_root->key;
}

int SecondMax(Node** root) {
    Node* copy_root = *root;
    while (copy_root->right != nullptr) {
        copy_root = copy_root->right;
    }
    if (copy_root->left == nullptr) {
        return copy_root->parent->key;
    }
    return Max(&copy_root->left);
}

int main () {
    Node* tree(nullptr);
    int digit;
    std::cin >> digit;
    while (digit != 0) {
        Insert(&tree, digit);
        std::cin >> digit;
    }
    std::cout << SecondMax(&tree);
    return 0;
}
