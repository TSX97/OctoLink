#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include <string>

namespace crypto {
    std::string hash_password(const std::string& password);
    bool verify_password(const std::string& password, const std::string& hash);

}


#endif