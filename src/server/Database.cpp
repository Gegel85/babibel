/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Database.cpp
*/

#include <vector>
#include <iostream>
#include "Database.hpp"

namespace Babel::Server
{
    Database::Database(const std::string &filename, bool debug):
    _debug(debug)
    {
        int rc = sqlite3_open(filename.c_str(), &this->_db);

        if (rc)
            throw Exceptions::CreationError(std::string("Can't open Database: ") + sqlite3_errmsg(_db));
        this->_init_tables();
    }

    Database::~Database()
    {
        sqlite3_close(this->_db);
    }

    std::string Database::get_last_error()
    {
        std::string str(this->_last_error);
        sqlite3_free(this->_last_error);
        return str;
    }

    static int av_callback(void *data, int argc, char **argv, char **col_name)
    {
        Database *self = reinterpret_cast<Database *>(data);
        std::vector<DSresponse> result = {};
        for (int i = 0; i < argc; i++) {
            self->_last_data.push_back({std::string(col_name[i]), std::string(argv[i])});
            if (self->_debug)
                printf("Added in Callback vector: {%s, %s}\n", col_name[i], argv[i]);
        }
        return 0;
    }


    std::vector<DSresponse> Database::raw_request(const std::string &request)
    {
        this->_last_data.clear();
        if (this->_debug)
            std::cout << "Database raw request: " << request << std::endl;
        int rc = sqlite3_exec(this->_db, request.c_str(), av_callback, this, &this->_last_error);
        if (rc != SQLITE_OK)
            throw Exceptions::SQLError(std::string("An SQL Error occured: ") + this->get_last_error());
        if (this->_debug) {
            printf("=== LAST DATA ===\n");
            for (size_t i = 0; i < this->_last_data.size(); i++)
                printf("{%s, %s}\n", this->_last_data[i].column_name.c_str(), this->_last_data[i].data.c_str());
            printf("=================\n");
        }
        return this->_last_data;
    }


    void Database::_init_tables()
    {
        const char *sql = (
        "CREATE TABLE IF NOT EXISTS Member (\n"
        "    ID INTEGER PRIMARY KEY AUTOINCREMENT,\n"
        "    Username CHAR(32) NOT NULL,\n"
        "    Passwd CHAR(32) NOT NULL\n"
        ");");

        this->raw_request(sql);
    }

    void Database::check_forbidden_input(const std::string &str)
    {
        std::size_t index = str.find(std::string("'"));

        if (index != std::string::npos)
            throw Exceptions::ForbiddenInput("A forbidden input has been given, please stop put quote in your input for trying SQL injection.");

    }

    int Database::register_member(const std::string &username, const std::string &passwd)
    {
        check_forbidden_input(username);
        check_forbidden_input(passwd);

        if (this->member_exist(username))
            throw Exceptions::MemberAlreadyExist("This username is already taken");
        const std::string sql = std::string("INSERT INTO Member (Username,Passwd)\n") +
                                "VALUES ('" + username + "', '" + passwd +"');";
        this->raw_request(sql);
        return this->check_creds(username, passwd);
    }

    int Database::check_creds(const std::string &username, const std::string &passwd)
    {
        check_forbidden_input(username);
        check_forbidden_input(passwd);

        const std::string sql = (
            std::string("SELECT (ID) FROM Member WHERE Username='") +
            username + "' AND Passwd='" + passwd +"'"
        );
        auto data = this->raw_request(sql);

        if (data.empty())
            throw Exceptions::NotFound("Bad credentials given");
        return std::stoi(data[0].data);
    }

    bool Database::member_exist(const std::string &username)
    {
        try {
            get_member_by_name(username);
            return true;
        }
        catch (Exceptions::NotFound &_) {
            return false;
        }
    }

    int Database::get_member_by_name(const std::string &username)
    {
        check_forbidden_input(username);

        const std::string sql = (
                std::string("SELECT (ID) FROM Member WHERE Username='") + username + "'");
        auto data = this->raw_request(sql);

        if (data.empty())
            throw Exceptions::NotFound("User with this name was not found");
        return std::stoi(this->_last_data[0].data);
    }

    std::string Database::get_user_name(int id)
    {
        const std::string sql = (
                std::string("SELECT (Username) FROM Member WHERE ID = ") + std::to_string(id));
        auto data = this->raw_request(sql);

        if (data.empty())
            throw Exceptions::NotFound("User with this ID was not found");
        return std::string(this->_last_data[0].data);
    }

    std::vector<DSuser> Database::get_all_user()
    {
        const std::string sql = "SELECT ID, Username FROM Member;";
        auto data = this->raw_request(sql);

        if (data.size() % 2)
            throw Exceptions::SQLError(std::string("get_all_user must return a even size, but it was ") + std::to_string(data.size()));
        std::vector<DSuser> result = {};
        for (size_t i = 0; i < data.size(); i += 2)
            result.push_back({std::stoi(data[i].data), data[i + 1].data});
        return result;
    }
}