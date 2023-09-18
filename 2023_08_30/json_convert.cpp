#include <iostream>
#include <sstream>
#include <string>

std::string convertFormat(const std::string& input) {
    std::string result;
    std::istringstream stream(input);
    std::string segment;

    while (std::getline(stream, segment, ']')) {
        std::size_t start = segment.find('[');
        if (start != std::string::npos) {
            segment = segment.substr(start + 1);
            while (segment.back() == ' ' || segment.back() == ',') {
                segment.pop_back();
            }
            result += segment + ";";
        }
    }
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

int main() {
    std::string s = "[100,200], [10,20]";
    std::cout << "input = " << s << "\n";
    std::cout << convertFormat(s) << std::endl; // 输出: 100,200;10,20
    return 0;
}
