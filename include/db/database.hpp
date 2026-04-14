#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <libpq-fe.h>
#include <vector>
#include <string>

class Database {
public:
	Database(const std::string& connection_setting);
	~Database();
	bool create_user(const std::string& name, const std::string& password);
	int find_user(const std::string& name);
	int authenticate(const std::string& name, const std::string& password);
	std::vector<std::pair<int, std::string>> get_all();
	int get_or_create_private_chat(int user1_id, int user2_id);
	void save_message(int chat_id, int from_user_id, const std::string& text);

private:
	PGconn* conn;

};
#endif
