#include <gtest/gtest.h>
#include "manager.hpp"
#include "client.hpp"
#include <boost/asio.hpp>

using namespace std;

class ClientManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        boost::asio::io_context io;
        boost::asio::ip::tcp::socket dummy_socket(io);

        alice = make_shared<Client>(move(dummy_socket), 1);

        boost::asio::io_context io2;
        boost::asio::ip::tcp::socket dummy_socket2(io2);
        bob = make_shared<Client>(move(dummy_socket2), 2);

        boost::asio::io_context io3;
        boost::asio::ip::tcp::socket dummy_socket3(io3);
        charlie = make_shared<Client>(move(dummy_socket3), 3);

        alice->name = "Alice";
        bob->name = "Bob";
        charlie->name = "Charlie";

        manager.add(alice);
        manager.add(bob);
        manager.add(charlie);
    }

    ClientManager manager;
    shared_ptr<Client> alice;
    shared_ptr<Client> bob;
    shared_ptr<Client> charlie;
};

// ========== ADD ==========

TEST_F(ClientManagerTest, Add_IncreasesSize) {
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket dummy_socket(io);
    auto david = make_shared<Client>(move(dummy_socket), 4);
    david->name = "David";

    size_t old_size = manager.get_all().size();
    manager.add(david);

    EXPECT_EQ(manager.get_all().size(), old_size + 1);
    EXPECT_NE(manager.find_by_name("David"), nullptr);
}

// ========== FIND BY ID ==========

TEST_F(ClientManagerTest, FindById_ReturnsCorrectClient) {
    auto found = manager.find_by_id(2);

    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->name, "Bob");
}

TEST_F(ClientManagerTest, FindById_NotFound_ReturnsNull) {
    auto found = manager.find_by_id(999);

    EXPECT_EQ(found, nullptr);
}

// ========== FIND BY NAME ==========

TEST_F(ClientManagerTest, FindByName_ReturnsCorrectClient) {
    auto found = manager.find_by_name("Charlie");

    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->id, 3);
}

TEST_F(ClientManagerTest, FindByName_NotFound_ReturnsNull) {
    auto found = manager.find_by_name("Nobody");

    EXPECT_EQ(found, nullptr);
}

// ========== REMOVE ==========

TEST_F(ClientManagerTest, RemoveById_RemovesCorrectClient) {
    manager.remove_by_id(2);

    EXPECT_EQ(manager.find_by_name("Bob"), nullptr);
    EXPECT_EQ(manager.find_by_name("Alice")->name, "Alice");
    EXPECT_EQ(manager.find_by_name("Charlie")->name, "Charlie");
    EXPECT_EQ(manager.get_all().size(), 2);
}

TEST_F(ClientManagerTest, RemoveByName_RemovesCorrectClient) {
    manager.remove_by_name("Alice");

    EXPECT_EQ(manager.find_by_name("Alice"), nullptr);
    EXPECT_EQ(manager.find_by_name("Bob")->name, "Bob");
    EXPECT_EQ(manager.find_by_name("Charlie")->name, "Charlie");
    EXPECT_EQ(manager.get_all().size(), 2);
}

TEST_F(ClientManagerTest, RemoveNonExistent_DoesNothing) {
    size_t old_size = manager.get_all().size();

    manager.remove_by_id(999);
    manager.remove_by_name("Nobody");

    EXPECT_EQ(manager.get_all().size(), old_size);
}

// ========== GET ALL ==========

TEST_F(ClientManagerTest, GetAll_ReturnsAllClients) {
    auto all = manager.get_all();

    EXPECT_EQ(all.size(), 3);

    vector<string> names;
    for (const auto& c : all) {
        names.push_back(c->name);
    }
    sort(names.begin(), names.end());

    EXPECT_EQ(names[0], "Alice");
    EXPECT_EQ(names[1], "Bob");
    EXPECT_EQ(names[2], "Charlie");
}