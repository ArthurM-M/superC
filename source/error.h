#pragma once
#include <string>

namespace Error {
    void report(int line, int col, const std::string& message);
}