#include "tools.hpp"
#include <sstream>
#include <algorithm>

namespace tools {

    // ========== СТРОКОВЫЕ УТИЛИТЫ ==========

    std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string token;
        while (std::getline(ss, token, delim)) {
            if (!token.empty()) {
                result.push_back(token);
            }
        }
        return result;
    }

    std::string join(const std::vector<std::string>& parts, const std::string& sep) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i != 0) result += sep;
            result += parts[i];
        }
        return result;
    }

    // ========== JSON УТИЛИТЫ ==========

    nlohmann::json parse_json(const std::string& raw) {
        try {
            return nlohmann::json::parse(raw);
        } catch (...) {
            return nlohmann::json::object();
        }
    }

    std::string get_cmd(const nlohmann::json& j) {
        return j.value("cmd", "");
    }

    std::string get_arg(const nlohmann::json& j, const std::string& key) {
        return j.value(key, "");
    }

    // ========== ФОРМИРОВАНИЕ ОТВЕТОВ ==========

    nlohmann::json ok(const std::string& data) {
        nlohmann::json j = {{"status", "ok"}};
        if (!data.empty()) {
            j["data"] = data;
        }
        return j;
    }

    nlohmann::json error(const std::string& err) {
        return {{"status", "error"}, {"error", err}};
    }

    bool is_error(const nlohmann::json& j) {
        return j.value("status", "") == "error";
    }

    std::string get_error(const nlohmann::json& j) {
        return j.value("error", "Unknown error");
    }

    std::string get_data(const nlohmann::json& j) {
        return j.value("data", "");
    }

}
