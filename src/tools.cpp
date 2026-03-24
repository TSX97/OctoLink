#include <string>
#include <vector>
using namespace std;

namespace tools {
    vector<string> split(const string& s, char sep = ' ') {
        vector<string> result;
        string temp;
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == sep) {
                result.push_back(temp);
                temp.clear();
                continue;
            }
            temp += s[i];
        }
        return result;
    }

    string join(const vector<string>& v, const string& sep) {
        string result;
        for (size_t i = 0; i < v.size(); i++) {
            result += v[i] + sep;
        }
        if (!result.empty()) {
            result.pop_back();
        }
        return result;
    }
}