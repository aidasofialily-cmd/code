#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

/**
 * Formats a long long value with commas as thousands separators.
 * Example: 1000 -> "1,000", 1000000 -> "1,000,000"
 */
inline std::string formatWithCommas(long long value) {
    std::string s = std::to_string(value);
    int insertPosition = static_cast<int>(s.length()) - 3;
    int limit = (value < 0) ? 1 : 0;
    while (insertPosition > limit) {
        s.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return s;
}

#endif
