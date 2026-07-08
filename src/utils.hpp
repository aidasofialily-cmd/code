#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

inline std::string formatWithCommas(long long value) {
    std::string s = std::to_string(value);
    int n = static_cast<int>(s.length());
    int limit = (value < 0) ? 1 : 0;
    int insertPosition = n - 3;
    while (insertPosition > limit) {
        s.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return s;
}

#endif
