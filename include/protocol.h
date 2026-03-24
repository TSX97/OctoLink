#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <nlohmann/json.hpp>

namespace TENTA {

using json = nlohmann::json;

// Клиентские команды
json msg(const std::string& to, const std::string& text);
json bnd(const std::string& name);
json lst();
json ext();

}

#endif