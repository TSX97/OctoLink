#include <gtest/gtest.h>
#include "protocol.hpp"
#include "manager.hpp"
#include <boost/asio.hpp>

using namespace std;

class TentaTest : public ::testing::Test {
protected:
    void SetUp() override {
        boost::asio::io_context io;
        boost::asio::ip::tcp::socket dummy_socket(io);

        sender = make_shared<Client>(move(dummy_socket), 1);
        sender->name = "Alice";
        sender->target = "Bob";

        boost::asio::io_context io2;
        boost::asio::ip::tcp::socket dummy_socket2(io2);
        receiver = make_shared<Client>(move(dummy_socket2), 2);
        receiver->name = "Bob";

        manager.add(sender);
        manager.add(receiver);

        sender->clear_log();
        receiver->clear_log();
    }

    ClientManager manager;
    shared_ptr<Client> sender;
    shared_ptr<Client> receiver;
};

// ========== REG ==========

TEST_F(TentaTest, Reg_AddsNewClient) {
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket dummy_socket(io);
    auto new_client = make_shared<Client>(move(dummy_socket), 3);

    TENTA::reg("David", new_client, manager);

    EXPECT_EQ(new_client->name, "David");
    EXPECT_NE(manager.find_by_name("David"), nullptr);
}

TEST_F(TentaTest, Reg_WithExistingName_SendsError) {
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket dummy_socket(io);
    auto new_client = make_shared<Client>(move(dummy_socket), 3);

    TENTA::reg("Bob", new_client, manager);

    auto log = new_client->get_log();
    ASSERT_FALSE(log.empty());
    EXPECT_NE(log[0].find("error"), string::npos);
    EXPECT_NE(log[0].find("already exists"), string::npos);

    EXPECT_EQ(manager.find_by_name("Bob"), receiver);
}

TEST_F(TentaTest, Reg_WithEmptyName_SendsError) {
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket dummy_socket(io);
    auto new_client = make_shared<Client>(move(dummy_socket), 3);

    TENTA::reg("", new_client, manager);

    auto log = new_client->get_log();
    ASSERT_FALSE(log.empty());
    EXPECT_NE(log[0].find("cannot be empty"), string::npos);
}

// ========== BND ==========

TEST_F(TentaTest, Bnd_SetsTarget) {
    TENTA::bnd("Alice", receiver, manager);

    EXPECT_EQ(receiver->target, "Alice");

    auto log = receiver->get_log();
    ASSERT_FALSE(log.empty());
    EXPECT_NE(log[0].find("Will send to Alice"), string::npos);
}

TEST_F(TentaTest, Bnd_ToNonexistentTarget_SendsError) {
    size_t old_log_size = receiver->get_log().size();

    TENTA::bnd("Nobody", receiver, manager);

    EXPECT_EQ(receiver->target, "");

    auto log = receiver->get_log();
    ASSERT_GT(log.size(), old_log_size);
    EXPECT_NE(log.back().find("user not found"), string::npos);
}

TEST_F(TentaTest, Bnd_WithEmptyTarget_SendsError) {
    size_t old_log_size = receiver->get_log().size();

    TENTA::bnd("", receiver, manager);

    auto log = receiver->get_log();
    ASSERT_GT(log.size(), old_log_size);
    EXPECT_NE(log.back().find("cannot be empty"), string::npos);
}

// ========== MSG ==========

TEST_F(TentaTest, Msg_SendsToTarget) {
    TENTA::msg("Hello, Bob!", sender, manager);

    auto receiver_log = receiver->get_log();
    ASSERT_EQ(receiver_log.size(), 1);
    EXPECT_EQ(receiver_log[0], "[PM from Alice]: Hello, Bob!");

    auto sender_log = sender->get_log();
    ASSERT_EQ(sender_log.size(), 1);
    EXPECT_NE(sender_log[0].find("Sent to Bob"), string::npos);
}

TEST_F(TentaTest, Msg_WithEmptyText_SendsError) {
    size_t old_log_size = sender->get_log().size();

    TENTA::msg("", sender, manager);

    auto log = sender->get_log();
    ASSERT_GT(log.size(), old_log_size);
    EXPECT_NE(log.back().find("cannot be empty"), string::npos);

    EXPECT_TRUE(receiver->get_log().empty());
}

TEST_F(TentaTest, Msg_ToNonexistentTarget_SendsError) {
    sender->target = "Nobody";
    sender->clear_log();

    TENTA::msg("Hello!", sender, manager);

    auto log = sender->get_log();
    ASSERT_FALSE(log.empty());
    EXPECT_NE(log[0].find("user not found"), string::npos);

    EXPECT_TRUE(receiver->get_log().empty());
}

TEST_F(TentaTest, Msg_WhenTargetNotSet_SendsError) {
    sender->target = "";
    sender->clear_log();

    TENTA::msg("Hello!", sender, manager);

    auto log = sender->get_log();
    ASSERT_FALSE(log.empty());
    EXPECT_NE(log[0].find("user not found"), string::npos);
}

// ========== LST ==========

TEST_F(TentaTest, Lst_ReturnsListOfUsers) {
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket dummy_socket(io);
    auto david = make_shared<Client>(move(dummy_socket), 4);
    david->name = "David";
    manager.add(david);
    auto charlie = make_shared<Client>(move(dummy_socket), 4);
    charlie->name = "Charlie";
    manager.add(charlie);

    sender->clear_log();

    TENTA::lst(sender, manager);

    auto log = sender->get_log();
    ASSERT_FALSE(log.empty());

    string response = log[0];
    EXPECT_NE(response.find("Bob"), string::npos);
    EXPECT_NE(response.find("Charlie"), string::npos);
    EXPECT_NE(response.find("David"), string::npos);
    EXPECT_EQ(response.find("Alice"), string::npos);
}

TEST_F(TentaTest, Lst_WithTarget_HighlightsTarget) {

    boost::asio::io_context io;
    boost::asio::ip::tcp::socket dummy_socket(io);
    auto charlie = make_shared<Client>(move(dummy_socket), 4);
    charlie->name = "Charlie";
    manager.add(charlie);

    sender->target = "Bob";
    sender->clear_log();


    TENTA::lst(sender, manager);

    auto log = sender->get_log();
    ASSERT_FALSE(log.empty());

    string response = log[0];
    EXPECT_NE(response.find("*Bob"), string::npos);
    EXPECT_NE(response.find("Charlie"), string::npos);
}

// ========== EXT ==========

TEST_F(TentaTest, Ext_SendsGoodbye) {
    TENTA::ext(sender);

    auto log = sender->get_log();
    ASSERT_FALSE(log.empty());
    EXPECT_NE(log[0].find("Goodbye"), string::npos);
}