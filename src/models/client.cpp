#include <string>
#include <boost/asio.hpp>
#include "models/client.hpp"

using namespace std;
using boost::asio::ip::tcp;

Client::Client(tcp::socket s, int i): name("User" + to_string(i)), id(i), target(""), socket(move(s)){}

void Client::send(const string& message) {
#ifndef INK_TEST_MODE
    boost::asio::write(socket, boost::asio::buffer(message + "\n"));
#else
    test_log.push_back(message);
#endif
}