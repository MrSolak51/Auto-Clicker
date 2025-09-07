#ifndef DATABASE_H
#define DATABASE_H

#include <mariadb/mysql.h>
#include <iostream>
#include "list.h"
#include <vector>

class Database {
private: 
    MYSQL *conn;
    MYSQL_RES *res; 
    MYSQL_ROW row;
    List *earnings_list, *expenses_list;
    List *saved_earnings_list, *saved_expenses_list;
    int user_id;
    void get_user_id(const std::string& username, const std::string& password);
    void consumeResults();
public:
    Database();

    void set_earnings_list(List* earnings_list);
    void set_expenses_list(List* expenses_list);
    void set_saved_earnings_list(List* saved_earnings_list);
    void set_saved_expenses_list(List* saved_expenses_list);

    ~Database();
    void select_earnings(int month, std::string year);
    void select_expenses(int month, std::string year);
    void add_earning(std::string header, std::string description, float amount, bool paid, std::string date);
    void add_expense(std::string header, std::string description, float amount, bool paid, std::string date);
    void update_earning(int earning_id, std::string header, std::string description, float amount, bool paid,std::string date);
    void update_expense(int expense_id, std::string header, std::string description, float amount, bool paid,std::string date);
    void delete_earning(int earning_id);
    void delete_expense(int expense_id);
    float get_total(int month, std::string year);
    float get_total_earnings(int month, std::string year);
    float get_total_expenses(int month, std::string year);
    void select_saved_earnings();
    void select_saved_expenses();
    void add_saved_earning(std::string header, std::string description, float amount);
    void add_saved_expense(std::string header, std::string description, float amount);


    bool is_user_valid(const std::string& username, const std::string& password);
    void clear_table(std::string table_name, int month, std::string year);
    void clear_saved_table();
    void add_user(const std::string& name,const std::string& surname,const std::string& username,const std::string& email, const std::string& password);
    void get_user(std::string& name, std::string& surname, std::string& username, std::string& email);
    void update_user(std::string name, std::string surname, std::string username, std::string email);
    void update_password(std::string new_password);
    void delete_user();

    void get_monthly_statistics(int month, std::string year, std::vector<double> &total_earnings,std::vector<double> &total_expenses,std::vector<double> &total);
    void get_yearly_statistics(std::string year, std::vector<double> &total_earnings,std::vector<double> &total_expenses,std::vector<double> &total);

};

#endif //DATABASE_H
