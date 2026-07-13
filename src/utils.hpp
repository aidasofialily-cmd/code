#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

inline std::string formatWithCommas(long long n) {
    std::string s = std::to_string(n);
    int insertPosition = static_cast<int>(s.length()) - 3;
    int limit = (n < 0) ? 1 : 0;
    while (insertPosition > limit) {
        s.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return s;
}

#endif
