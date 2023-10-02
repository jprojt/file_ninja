#include "utils/formatter.h"
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

std::string formatter::FormatBytes(long kb, int pad) {
    static const std::vector<std::string> units {
        "B", "KB", "MB", "GB", "TB", "PB", "EB"
    };
    double fmt_val = kb;
    std::stringstream sstream;
    for (size_t i = 0; i < units.size(); i++) {
        if (fmt_val < 1024) {
            int precision = fmt_val < 100 ? 2 : 0; 
            if (fmt_val - (long)fmt_val < 0.01) {
                precision = 0;
            }
            sstream << std::right << std::setw(pad) << std::fixed << std::setprecision(precision) << fmt_val << " " << units[i];
            return sstream.str();
        }
        fmt_val /= 1024.0;
    }

    return "";
}