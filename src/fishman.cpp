#include "fishman.hpp"
#include "tools.hpp"

void Fishman::handle(const std::string& raw, std::shared_ptr<Client> client, ClientManager& cm) {
    client->send("Echo: " + raw);
}