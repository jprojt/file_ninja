#include "utils/string_util.h"


std::string string_util::EscapeRegexString(const std::string& source_string) {
    std::string escaped;
    for (auto& c : source_string) {
        switch (c) {
            case '\\':
            case '^':
            case '.':
            case '$':
            case '|':
            case '(':
            case ')':
            case '[':
            case ']':
            case '*':
            case '+':
            case '?':
            case '{':
            case '}':
                escaped += '\\';
            default:
                escaped += c;
        }
    }
    return escaped;
}