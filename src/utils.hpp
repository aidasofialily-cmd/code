#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

/**
 * Formats a long long value with thousands separators (commas).
 * Example: 1234567 -> "1,234,567"
 */
inline std::string formatWithCommas(long long value) {
    std::string s = std::to_string(value);
    int insertPosition = static_cast<int>(s.length()) - 3;
    while (insertPosition > (value < 0 ? 1 : 0)) {
        s.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return s;
}

#endif
