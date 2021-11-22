// Treap
// Реализуйте структуру данных, которая поддерживает множество S целых чисел, с котором разрешается производить следующие операции:
//     add(i) — добавить в множество S число i (если он там уже есть, то множество не меняется);
//     next(i) — вывести минимальный элемент множества, не меньший i. Если искомый элемент в структуре отсутствует, необходимо вывести -1.

// Формат ввода
// Исходно множество S пусто. Первая строка входного файла содержит n — количество операций (1≤ n≤ 300000). Следующие n строк содержат операции. Каждая операция имеет вид либо «+ i», либо «? i». Операция «? i» задает запрос next(i).
// Если операция «+ i» идет во входном файле в начале или после другой операции «+», то она задает операцию add(i). Если же она идет после запроса «?», и результат этого запроса был y, то выполняется операция add(i + y) mod 10^9.
// Во всех запросах и операциях добавления параметры лежат в интервале от 0 до 10^9.
// Формат вывода
// Для каждого запроса выведите одно число — ответ на запрос.


#include <algorithm>
#include <iostream>

struct Treap {
    int key, priority;
    Treap *left, *right;
    Treap() = default;
    Treap(int k, int p) : key(k), priority(p), left(nullptr), right(nullptr) {
    }
};

void Split(Treap* root, int key, Treap*& l, Treap*& r) {
    if (root == nullptr) {
        l = nullptr;
        r = nullptr;
    } else if (key < root->key) {
        Split(root->left, key, l, root->left);
        r = root;
    }
    else {
        Split(root->right, key, root->right, r);
        l = root;
    }
}

void Insert(Treap*& root, Treap* key) {
    if (root == nullptr) {
        root = key;
    }
    else if (key->priority > root->priority) {
        Split(root, key->key, key->left, key->right);
        root = key;
    }
    else {
        if (key->key < root->key) {
            Insert(root->left, key);
        } else {
            Insert(root->right, key);
        }
    }
}

void Merge(Treap*& root, Treap* l, Treap* r) {
    if (l == nullptr) {
        root = l;
    } else if (r == nullptr) {
        root = r;
    } else if (l->priority > r->priority) {
        Merge(l->right, l->right, r);
        root = l;
    }
    else {
        Merge(r->left, l, r->left);
        root = r;
    }
}

int FindNext(Treap* root, int search) {
    Treap* copy_root = root;
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

int Next(Treap* root, int key) {
    int next_min = FindNext(root, key);
    if (next_min == 1000000001) {
        return -1;
    }
    return next_min;
}

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    Treap* tree(nullptr);
    char prev_task = ' ';
    int prev_next_return;
    int num;
    int priority = 0;
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
                Treap* new_node = new Treap((digit + prev_next_return) % 1000000000, priority++);
                Insert(tree, new_node);
            } else {
                Treap* new_node = new Treap(digit, priority++);
                Insert(tree, new_node);
            }
            prev_task = task;
        }
    }
    return 0;
}
