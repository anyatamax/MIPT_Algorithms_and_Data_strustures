

// Одно разбросанное на островах Океании государство решило создать сеть автомобильных дорог (вернее, мостов). По каждому мосту можно перемещаться в обе стороны. Был разработан план очередности строительства мостов и известно, что после постройки всех мостов можно будет проехать по ним с каждого острова на каждый (возможно, через некоторые промежуточные острова).
// Однако, этот момент может наступить до того, как будут построены все мосты. Ваша задача состоит в определении такого минимального количества мостов, после постройки которого (в порядке строительства по плану) можно будет попасть с любого острова на любой другой.

// Формат ввода
// Первая строка содержит два числа: N — число островов (1 ≤ N ≤ 100000) и M — количество мостов в плане (1 ≤ M ≤ 200000). В каждой следующей строке содержится описание моста — два числа x и y (0 ≤ x, y < N) — номера соединяемых островов.
// Формат вывода
// Выведите в выходной файл одно число — минимальное количество построенных мостов, по которым можно попасть с любого острова на любой.



#include <iostream>
#include <vector>

class DSU {
public:
    explicit DSU(int size);
    void MakeSet(int ver);
    int FindSet(int ver);
    int UnionSets(int ver_first, int ver_second);

private:
    std::vector<int> parent_;
    std::vector<int> size_;
};

int DSU::UnionSets(int ver_first, int ver_second) {
    ver_first = FindSet(ver_first);
    ver_second = FindSet(ver_second);
    if (ver_second != ver_first) {
        if (size_[ver_first] < size_[ver_second]) {
            std::swap(ver_first, ver_second);
        }
        parent_[ver_second] = ver_first;
        size_[ver_first] += size_[ver_second];
    }
    return size_[ver_first];
}

DSU::DSU(int size) : parent_(size), size_(size, 1) {
    for (int i = 0; i < size; ++i) {
        parent_[i] = i;
    }
}

void DSU::MakeSet(int ver) {
    parent_[ver] = ver;
    size_[ver] = 1;
}

int DSU::FindSet(int ver) {
    if (ver == parent_[ver]) {
        return ver;
    }
    return parent_[ver] = FindSet(parent_[ver]);
}

int main() {
    int n = 0;
    int m = 0;
    int index_edge = 0;
    bool flag_exist = false;
    std::cin >> n >> m;
    DSU sets(n);
    for (int i = 0; i < m; ++i) {
        int ver_first = 0;
        int ver_second = 0;
        std::cin >> ver_first >> ver_second;
        ++index_edge;
        int size_set = sets.UnionSets(ver_first, ver_second);
        if (size_set == n) {
            flag_exist = true;
            std::cout << index_edge;
            break;
        }
    }
    if (!flag_exist) {
        std::cout << -1;
    }
    return 0;
}
