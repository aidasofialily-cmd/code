#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <algorithm>

inline std::string formatWithCommas(long long value) {
    bool isNegative = value < 0;
    std::string s = std::to_string(isNegative ? -value : value);
    int insertPosition = static_cast<int>(s.length()) - 3;
    while (insertPosition > 0) {
        s.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    if (isNegative) {
        s.insert(0, "-");
    }
    return s;
}

#endif // UTILS_HPP
