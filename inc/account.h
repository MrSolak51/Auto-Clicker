#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include "database.h"

class Account {
    private:
        std::string username;
        std::string name;
        std::string surname;
        std::string email;
    public:
        Account(Database *db, std::string name, std::string surname, std::string username, std::string email);
        ~Account();
        std::string get_username();
        std::string get_name();
        std::string get_surname();
        std::string get_email();
        void update(std::string name, std::string surname, std::string username, std::string email);
        void update_password(std::string new_password);
        void delete_();
        Database *db;
};

#endif //ACCOUNT_H