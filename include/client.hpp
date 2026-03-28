#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <boost/asio.hpp>

struct Client {
    std::string name;
    int id;
    std::string target;
    boost::asio::ip::tcp::socket socket;

    Client(boost::asio::ip::tcp::socket socket, int id);
    void send(const std::string& message);

};

#endif