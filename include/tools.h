#include <string>
#include <vector>

namespace tools {
    std::vector<std::string> split(const std::string& s, char sep);
    std::string join(const std::vector<std::string>& v, const std::string& sep);
} //TODO ПАРСЕР