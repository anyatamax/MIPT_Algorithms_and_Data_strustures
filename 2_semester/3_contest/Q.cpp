// Дана база данных о продажах некоторого интернет-магазина. Каждая строка входного файла представляет собой запись вида покупатель-товар-количество , где покупатель — имя покупателя (строка без пробелов), товар — название товараc (строка без пробелов), количество — количество приобретённых единиц товара.
// Создайте список всех покупателей и для каждого покупателя подсчитайте количество приобретенных им единиц каждого вида товаров.

// Формат ввода
// Во входном файле записано не более 10^5 строк в указанном формате.
// Имена покупателей и названия товаров представляют собой строки из заглавных и строчных латинских букв не длиннее 10 символов. Количество товара в каждой покупке — натуральное число, не превышающее 10^6.
// Формат вывода
// Выведите список всех покупателей в лексикографическом порядке, после имени каждого покупателя выведите двоеточие, затем выведите список названий всех приобретенных данным покупателем товаров в лексикографическом порядке, после названия каждого товара выведите количество единиц товара, приобретенных данным покупателем.
// Информация о каждом товаре выводится в отдельной строке.


#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <list>

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::map<std::string, std::map<std::string, long int>> buyers;
    std::string name, product;
    long int count;
    while (std::cin >> name >> product >> count) {
        auto buyer = buyers.find(name);
        if (buyer != buyers.end()) {
            auto prod = buyer->second.find(product);
            if (prod != buyer->second.end()) {
                prod->second += count;
            } else {
                buyer->second[product] = count;
            }
        } else {
            buyers[name] = std::map<std::string, long int>{{product, count}};
        }
    }
    std::list<std::pair<std::string, std::list<std::pair<std::string, long int>>>> sort_buyers;
    for (auto& it : buyers) {
        std::list<std::pair<std::string, long int>> sec;
        for (auto& it_2 : it.second) {
            sec.emplace_back(std::make_pair(it_2.first, it_2.second));
        }
        sec.sort();
        sort_buyers.emplace_back(std::make_pair(it.first, sec));
    }
    sort_buyers.sort();
    for (auto& it : sort_buyers) {
        std::cout << it.first << ":\n";
        for (auto& it_2 : it.second) {
            std::cout << it_2.first << " " << it_2.second << "\n";
        }
    }
    return 0;
}
