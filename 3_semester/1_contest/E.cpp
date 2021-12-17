

// На дворе начало нового учебного года, а значит, многие студенты вынуждены переселяться из привычного коридорного общежития в апартаменты премиум-класса. К сожалению, недобросовестные студенты постоянно затягивают свой переезд, поэтому новичкам ПФИМ приходится въезжать в комнаты, заполненные вещами дедов. В некоторых общежитиях практикуется выставление вещей предыдущих жильцов за дверь (иногда даже самими предыдущими жильцами), позднее кто-то (доподлинно неизвестно, кто) эти вещи забирает. Однако выставленные в проход вещи противоречат технике безопасности при пожаре, а проверка регулярно посещает общежития.

// Итак, сама задача: в общежитии имеется длинный основной коридор, который никак не должен быть захламлен по ТБ, но на мелкие нарушения проверка готова закрыть глаза. В этот коридор постоянно кто-то выставляет вещи, а кто-то постоянно их оттуда забирает. Когда приходит проверка, она считает, сколько предметов выставлено в коридор между двумя комнатами с заданными номерами (эти номера выбираются комиссией, проводящей проверку). Смоделируйте родное общежитие на языке C++.

// Формат ввода
// В первой строке входного файла содержатся два числа: N — количество комнат (1 ≤ N ≤ 10 000) и M — количество производимых в коридоре действий (1 ≤ M ≤ 50 000). Каждое действие имеет вид "1 L R S", что означает, что на участок коридора от L-ой комнаты до R-ую комнаты между всеми соседними комнатами выставили S предметов (S может быть и отрицательным, тогда это означает, что предметы из коридора забрали), или "2 L R", что значит, что проверка интересуется, сколько предметов выставлено на участке коридора от L-ой комнаты до R-й. Комнаты нумеруются от 0 до N. Гарантируется, что для запросов вида "1 L R S" при S < 0 на каждом участке между соседними комнатами от L до R число предметов составляет не менее S.
// Формат вывода
// На каждую команду 2 (запрос) вы должны выводить число K — суммарное число предметов, находящихся между L-й и R-й комнатами. Каждое число должно выводиться на новой строке. Известно, что в процессе работы суммарное количество предметов на любом интервале не превышает 231. 


#include <iostream>
#include <vector>

class SegmentTree {
public:
    void BuildSegmentTree(std::vector<long int>& arr);
    void Update(int root, int t_left, int t_right, int left, int right, long int delta);
    long int GetSum(int root, int t_left, int t_right, int left, int right);
    void Build(int v, int l, int r);
    int Size() const {
        return size_;
    }

private:
    std::vector<long int> st_;
    std::vector<long int> st_promise_;
    std::vector<std::pair<int, int>> segment_;
    int size_ = 0;

    void Push(int root);
};

void SegmentTree::BuildSegmentTree(std::vector<long int>& arr) {
    int deg = 0;
    while ((1 << deg) < arr.size()) {
        ++deg;
    }
    size_ = (1 << deg);
    st_.resize(2 * size_, 0);
    st_promise_.resize(2 * size_, 0);
    segment_.resize(2 * size_, std::pair<int, int>(0, 0));
}

void SegmentTree::Build(int v, int l, int r) {
    segment_[v] = std::pair<int, int>(l, r);
    if (l == r) {
        return;
    }
    Build(2 * v, l, (l + r) / 2);
    Build(2 * v + 1, (l + r) / 2 + 1, r);
}

long int SegmentTree::GetSum(int root, int t_left, int t_right, int left, int right) {
    Push(root);
    if (t_right < left || right < t_left || t_left > t_right) {
        return 0;
    }
    if (t_left >= left && t_right <= right) {
        return st_[root];
    }
    long int left_res = GetSum(root * 2, t_left, (t_left + t_right) / 2, left, right);
    long int right_res = GetSum(root * 2 + 1, (t_left + t_right) / 2 + 1, t_right, left, right);
    return left_res + right_res;
}

void SegmentTree::Update(int root, int t_left, int t_right, int left, int right, long int delta) {
    Push(root);
    if (t_right < left || right < t_left || t_left > t_right) {
        return;
    }
    if (t_left >= left && t_right <= right) {
        st_promise_[root] += delta;
        Push(root);
        return;
    }
    Update(root * 2, t_left, (t_left + t_right) / 2, left, right, delta);
    Update(root * 2 + 1, (t_left + t_right) / 2 + 1, t_right, left, right, delta);
    st_[root] = st_[root * 2] + st_[root * 2 + 1];
}

void SegmentTree::Push(int root) {
    st_[root] += (segment_[root].second - segment_[root].first + 1) * st_promise_[root];
    if (segment_[root].first < segment_[root].second) {
        st_promise_[root * 2] += st_promise_[root];
        st_promise_[root * 2 + 1] += st_promise_[root];
    }
    st_promise_[root] = 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n = 0;
    int task = 0;
    int count_tasks = 0;
    int index_l = 0;
    int index_r = 0;
    long int things = 0;
    std::cin >> n;
    std::vector<long int> rooms(n, 0);
    SegmentTree seg_t;
    seg_t.BuildSegmentTree(rooms);
    seg_t.Build(1, 1, seg_t.Size());
    std::cin >> count_tasks;
    for (int i = 0; i < count_tasks; ++i) {
        std::cin >> task;
        if (task == 1) {
            std::cin >> index_l >> index_r >> things;
            seg_t.Update(1, 1, seg_t.Size(), index_l + 1, index_r, things);
        } else {
            std::cin >> index_l >> index_r;
            std::cout << seg_t.GetSum(1, 1, seg_t.Size(), index_l + 1, index_r) << "\n";
        }
    }
    return 0;
}
