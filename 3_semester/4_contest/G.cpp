// Z-function


#include <iostream>
#include <vector>
#include <string>

std::vector<int> ZFunc(std::string& str) {
    std::vector<int> z(str.size(), 0);
    int left = 0;
    int right = 0;
    int len = str.size();
    z[0] = len;
    for (int i = 1; i < len; ++i) {
        z[i] = std::max(0, std::min(right - i, z[i - left]));
        while (i + z[i] < len && str[z[i]] == str[i + z[i]]) {
            ++z[i];
        }
        if (right < i + z[i]) {
            left = i;
            right = i + z[i];
        }
    }
    return z;
}

int main() {
    std::string str;
    std::cin >> str;
    std::vector<int> prefix_mas = ZFunc(str);
    for (auto& i : prefix_mas) {
        std::cout << i << " ";
    }
    return 0;
}
