#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include "manager.hpp"
#include <memory>

class Server {
private:
    boost::asio::io_context io_;
    boost::asio::ip::tcp::acceptor acceptor_;
    ClientManager client_manager_;

    void handle_client(std::shared_ptr<Client> client);
    void accept();

public:
    Server(short port);
    ~Server();
    void run();
    void stop();
};


#endif