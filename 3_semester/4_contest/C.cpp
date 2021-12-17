// Русскаярулетка
// Имя входного файла: стандартный ввод
// Имя выходного файла: стандартный вывод
// Ограничение по времени: 1 секунда
// Ограничение по памяти: 256 мегабайт
// Третий день не встает Иван с печи. Все ему надоело: и балалайка, и водка. Так больше нель-
// зя! сказал себе Иван, взял револьвер, надел валенки и ушел, хлопнув дверью.
// Здравствуй, Ваня! сказал ручной медведь Миша, увидев перед собой хмурого Ивана, сто-
// ящего на пороге его берлоги  С чем пожаловал?. Я хочу сыграть с тобой в одну игру¿ 
// сказал Иван, достав револьвер из-за пазухи. Миша молча сел за стол. Иван приблизился к столу 
// Вращай!¿  сказал он и дал пистолет в лапы медведю.
// Медведь увидел перед собой револьвер неизвестной модели с вращающимся барабаном. В неко-
// торые пазы были вставлены пули. Все пазы видны, кроме одного основного, из которого в ствол
// подается пуля. Медведь крутанул барабан.
// Что дальше? спросил Миша.
// Стреляй. Либо в себя, либо в меня.
// Медведь был очень предан своему хозяину, поэтому не мог его убить. Медведь снова глянул
// на барабан и захотел вычислить, будет ли сейчас произведен выстрел. Ведь от этого зависит его
// дальнейшая судьба. . .

// Формат входных данных
// Первая строка содержит целое число n (2 <= n <= 2·10^6)  количество пазов в барабане револьвера.
// Во второй строке задана строка a, описывающая состояние барабана револьвера до вращения.
// Строка состоит из n −1 символов, где ai = 0, если в i-м пазе по часовой стрелке, начиная от дула,
// нет пули, и ai = 1, если есть.
// В третьей строке задана строка b, описывающая состояние барабана револьвера после вращения
// в таком же формате.
// Гарантируется, что входные данные непротиворечивы.
// Формат выходных данных
// Выведите Yes, если выстрел точно будет произведен, No, если выстрела точно не будет или
// Random, если нельзя точно установить, будет ли выстрел.


#include <iostream>
#include <vector>
#include <string>

std::vector<int> PrefixFunc(std::string& str) {
    std::vector<int> p(str.size(), 0);
    for (size_t i = 1; i < str.size(); ++i) {
        int k = p[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = p[k - 1];
        }
        if (str[i] == str[k]) {
            ++k;
        }
        p[i] = k;
    }
    return p;
}

int main() {
    int n = 0;
    std::string before;
    std::string after;
    std::cin >> n >> before >> after;
    std::string cause_without_shot = before + '0' + before;
    std::string cause_with_shot = before + '1' + before;
    std::string find_first = after + '!' + cause_without_shot;
    std::string find_second = after + '!' + cause_with_shot;
    std::vector<int> cause_first = PrefixFunc(find_first);
    std::vector<int> cause_second = PrefixFunc(find_second);
    bool no_shot = false;
    bool has_shot = false;
    if (before == after) {
        no_shot = true;
        has_shot = true;
    } else {
        for (size_t i = 0; i < cause_without_shot.size(); ++i) {
            if (cause_first[i + after.size() + 1] == static_cast<int>(after.size())) {
                if (find_first[i + after.size() + 2] == '0') {
                    no_shot = true;
                } else {
                    has_shot = true;
                }
            } else if (cause_second[i + after.size() + 1] == static_cast<int>(after.size())) {
                if (find_second[i + after.size() + 2] == '0') {
                    no_shot = true;
                } else {
                    has_shot = true;
                }
            }
        }
    }
    if (has_shot && no_shot) {
        std::cout << "Random";
    } else if (has_shot) {
        std::cout << "Yes";
    } else if (no_shot) {
        std::cout << "No";
    }
    return 0;
}
