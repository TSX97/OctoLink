#ifndef FISHMAN_HPP
#define FISHMAN_HPP

#include <string>
#include <memory>
#include "client.hpp"
#include "manager.hpp"

namespace Fishman {
    void handle(const std::string& raw, std::shared_ptr<Client> client, ClientManager& cm);
}

#endif