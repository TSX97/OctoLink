#include "database.hpp"
#include <string>
#include <iostream>
#include <libpq-fe.h>

using namespace std;

class Database {
private:
    PGconn* conn;
public:
    Database(const string& connection_setting) {
        conn = PQconnectdb(connection_setting.c_str());
        if (PQstatus(conn) != CONNECTION_OK) {
            cerr << "DB error: " << PQerrorMessage(conn) << endl;
            conn = nullptr;
        }
    }
    ~Database() {
        if (conn) {
            PQfinish(conn);
        }
    }

    bool create_user(const string& name, const string& password) {
        if (!conn) return false;
        string hashed = hash_password(password); //TODO: do hashing
        string sql = "INSERT INTO user (name, password_hash) VALUES('" + name + "', " + "'" + password + ")'";
        PGresult* res = PQexec(conn, sql.c_str());
        bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return ok;
    }

    int authenticate_user(const string& name, const string& password) {
        if (!conn) return -1;
        string sql = "SELECT id, password_hash FROM users WHERE name = '" + name + "'";
        PGresult* res;
        if (PGresultStatus(res) != PGRES_TUPLE_OK || PQntuples(res) == 0) {
            PQclear(res);
            return -1;
        }
        string stored_hash = PQgetvalue(res, 0, 1);
        int id = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        return verify_password(password, stored_hash) ? id : -1;
    }
};

