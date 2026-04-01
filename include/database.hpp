#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>

class Database {
	public: 
		Database(const std::string& connection_setting);
		~Database();
		bool create_user(const std::string& name, const std::string& password);
		int authenticate(const std::string& name, const std::string& password);
		void save_message(int from, int to, const std::string& text);
	private:

}
#endif
