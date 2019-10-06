/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Database.hpp
*/
#ifndef CPP_BABEL_2019_DATABASE_HPP
#define CPP_BABEL_2019_DATABASE_HPP

#include <sqlite3.h>
#include <string>

#define DEFAULT_DATABASE_FILENAME std::string("database.db")

namespace Babel::Server {

    struct DSresponse {
        std::string column_name;
        std::string data;
    };

    struct DSuser {
        int id;
        std::string username;
    };

    class Database {
    private:
        sqlite3 *_db = nullptr;
        char *_last_error = nullptr;

        void _init_tables();
        std::string get_last_error();
        void check_forbidden_input(const std::string &str);

    public:
        bool _debug;
        std::vector<DSresponse> _last_data;

        Database(const std::string &filename = DEFAULT_DATABASE_FILENAME, bool debug=false);
        ~Database();
        std::vector<DSresponse> raw_request(const std::string &request);
        bool member_exist(const std::string &username);
        int get_member_by_name(const std::string &username);
        int register_member(const std::string &username, const std::string &passwd);
        int check_creds(const std::string &username, const std::string &passwd);
        std::string get_user_name(int id);
        std::vector<DSuser> get_all_user();
    };

    namespace Exceptions {
        class SQLBaseException : public std::exception {
        private:
            std::string _msg;
        public:
            explicit SQLBaseException(const std::string &msg) : _msg(msg)
            {};

            const char *what() const noexcept override
            { return this->_msg.c_str(); };
        };

        class SQLError : public SQLBaseException {
        public:
            explicit SQLError(const std::string &msg) : SQLBaseException(msg)
            {};
        };

        class CreationError : public SQLBaseException {
        public:
            explicit CreationError(const std::string &msg) : SQLBaseException(msg)
            {};
        };

        class NotFound : public SQLBaseException {
        public:
            explicit NotFound(const std::string &msg) : SQLBaseException(msg)
            {};
        };

        class MemberAlreadyExist : public SQLBaseException {
        public:
            explicit MemberAlreadyExist(const std::string &msg) : SQLBaseException(msg)
            {};
        };

        class ForbiddenInput : public SQLBaseException {
        public:
            explicit ForbiddenInput(const std::string &msg) : SQLBaseException(msg)
            {};
        };
    }
}


#endif //CPP_BABEL_2019_DATABASE_HPP
