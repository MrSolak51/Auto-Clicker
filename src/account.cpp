#include "account.h"

Account::Account(Database *db, std::string name, std::string surname, std::string username, std::string email)
: name(name), surname(surname), username(username), email(email),db(db) {}


Account::~Account(){}


std::string Account::get_name(){return name;}


std::string Account::get_surname(){return surname;}


std::string Account::get_username(){return username;}


std::string Account::get_email(){return email;}


void Account::update(std::string name, std::string surname, std::string username, std::string email){
    db->update_user(name, surname, username, email);
}


void Account::update_password(std::string new_password){
    db->update_password(new_password);
}


void Account::delete_(){
    db->delete_user();
}

