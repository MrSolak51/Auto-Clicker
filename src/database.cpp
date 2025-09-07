#include "database.h"

void Database::set_earnings_list(List* earnings_list){this->earnings_list = earnings_list;}


void Database::set_expenses_list(List* expenses_list){this->expenses_list = expenses_list;}


void Database::set_saved_earnings_list(List* saved_earnings_list){this->saved_earnings_list = saved_earnings_list;}


void Database::set_saved_expenses_list(List* saved_expenses_list){this->saved_expenses_list = saved_expenses_list;}


Database::Database() {
    conn = mysql_init(NULL);
    if (!conn) {
        std::cerr << "MySQL can't start!" << std::endl;
    }
    // MariaDB / MySQL bağlantısı
    conn = mysql_real_connect(conn, "localhost", "root", "root", "accountant_manager", 3306, NULL, 0);
    if (conn) {
    } else {
        std::cerr << "Connection failed: " << mysql_error(conn) << std::endl;
        std::system("pause");
        exit(-1);
    }
}


Database::~Database(){}


void Database::consumeResults() {
    MYSQL_RES* result;
    do {
        result = mysql_store_result(conn);
        if (result) {
            mysql_free_result(result);
        }
    } while (mysql_next_result(conn) == 0);
}


float Database::get_total(int month, std::string year) {
    std::string select_query = "call get_total("  + std::to_string(user_id) + ", " + std::to_string(month + 1) + ",'" + year + "');";
    float total = 0.0f;

    if (mysql_query(conn, select_query.c_str())) {
        std::cerr << "get_total error: " << mysql_error(conn) << std::endl;
        return total;
    }

    // Local variable kullanıyoruz
    MYSQL_RES* local_res = mysql_store_result(conn);
    if (!local_res) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    } else {
        MYSQL_ROW local_row;
        while ((local_row = mysql_fetch_row(local_res))) {
            total = std::atof(local_row[0]);
        }
        mysql_free_result(local_res);
    }

    // Tüm sonuç kümelerini tüket
    while (mysql_next_result(conn) == 0) {
        MYSQL_RES* extra_res = mysql_store_result(conn);
        if (extra_res) {
            mysql_free_result(extra_res);
        }
    }
    consumeResults();
    return total;
}


float Database::get_total_earnings(int month, std::string year) {
    std::string select_query = "call get_total_earnings(" + std::to_string(user_id) + ", " + std::to_string(month + 1) + ",'" + year + "');";
    float total = 0.0f;

    if (mysql_query(conn, select_query.c_str())) {
        std::cerr << "get_total_earnings error: " << mysql_error(conn) << std::endl;
        return total;
    }

    // Local variable kullanıyoruz
    MYSQL_RES* local_res = mysql_store_result(conn);
    if (!local_res) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    } else {
        MYSQL_ROW local_row;
        while ((local_row = mysql_fetch_row(local_res))) {
            total = std::atof(local_row[0]);
        }
        mysql_free_result(local_res);
    }

    // Tüm sonuç kümelerini tüket
    while (mysql_next_result(conn) == 0) {
        MYSQL_RES* extra_res = mysql_store_result(conn);
        if (extra_res) {
            mysql_free_result(extra_res);
        }
    }
    consumeResults();
    return total;
}


float Database::get_total_expenses(int month, std::string year) {
    std::string select_query = "call get_total_expenses(" + std::to_string(user_id) + ", " + std::to_string(month + 1) + ",'" + year + "');";
    float total = 0.0f;

    if (mysql_query(conn, select_query.c_str())) {
        std::cerr << "get_total_expenses error: " << mysql_error(conn) << std::endl;
        return total;
    }

    // Local variable kullanıyoruz
    MYSQL_RES* local_res = mysql_store_result(conn);
    if (!local_res) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    } else {
        MYSQL_ROW local_row;
        while ((local_row = mysql_fetch_row(local_res))) {
            total = std::atof(local_row[0]);
        }
        mysql_free_result(local_res);
    }

    // Tüm sonuç kümelerini tüket
    while (mysql_next_result(conn) == 0) {
        MYSQL_RES* extra_res = mysql_store_result(conn);
        if (extra_res) {
            mysql_free_result(extra_res);
        }
    }

    consumeResults();
    return total;
}


void Database::select_earnings(int month, std::string year){
    // SELECT komutu (hesaplar tablosundaki tüm verileri çekiyoruz)
    std::string select_query = "SELECT* FROM earnings where MONTH(saved_date) =" + std::to_string(month + 1) + " and YEAR(saved_date) ='" + year + "' and user_id=" + std::to_string(user_id);

    // Sorguyu çalıştır
    if (mysql_query(conn, select_query.c_str())) {
        std::cerr << "SELECT* FROM earnings error: " << mysql_error(conn) << std::endl;
    }   


    // Sorgu sonucunu al
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    }   
    earnings_list->clear();
    // Sonuçları ekrana yazdır
    while ((row = mysql_fetch_row(res))) {
        earnings_list->AddNode(row[2], row[3], std::atof(row[4]), (row[5]) ? true : false, row[6]);
    }

    // Bellek temizleme
    mysql_free_result(res);

}


void Database::select_expenses(int month, std::string year){
    // SELECT komutu (hesaplar tablosundaki tüm verileri çekiyoruz)
    std::string select_query = "SELECT* FROM expenses where MONTH(saved_date) =" + std::to_string(month + 1) + " and YEAR(saved_date) ='" + year + "' and user_id=" + std::to_string(user_id);

    // Sorguyu çalıştır
    if (mysql_query(conn, select_query.c_str())) {
        std::cerr << "SELECT* FROM expenses error: " << mysql_error(conn) << std::endl;
    }   


    // Sorgu sonucunu al
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    }   
    
    expenses_list->clear();
    // Sonuçları ekrana yazdır
    while ((row = mysql_fetch_row(res))) {
        expenses_list->AddNode(row[2], row[3], std::atof(row[4]), (row[5]) ? true : false, row[6]);
    }

    // Bellek temizleme
    mysql_free_result(res);
}


void Database::add_earning(std::string header, std::string description, float amount, bool paid, std::string date){
    std::string add_earning_procedure = "CALL add_earning(" + std::to_string(user_id) + ", '" + header + "', '" + description + "', " + std::to_string(amount) + ", " + std::to_string(paid) + ", '" + date + "');";
    if (mysql_query(conn, add_earning_procedure.c_str())) {
        std::cerr << "add_earning error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::add_expense(std::string header, std::string description, float amount, bool paid, std::string date){
    std::string add_expense_procedure = "CALL add_expense(" + std::to_string(user_id) + ", '" + header + "', '" + description + "', " + std::to_string(amount) + ", " + std::to_string(paid) + ", '" + date + "');";
    if (mysql_query(conn, add_expense_procedure.c_str())) {
        std::cerr << "add_expense error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::select_saved_earnings(){
    // SELECT komutu (hesaplar tablosundaki tüm verileri çekiyoruz)
    std::string select_query = "SELECT* FROM saved WHERE type_=0 and user_id=" + std::to_string(user_id);

    // Sorguyu çalıştır
    if (mysql_query(conn, select_query.c_str())) {
        std::cerr << "SELECT* FROM saved error: " << mysql_error(conn) << std::endl;
    }   


    // Sorgu sonucunu al
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    }   
    saved_earnings_list->clear();
    // Sonuçları ekrana yazdır
    while ((row = mysql_fetch_row(res))) {
        saved_earnings_list->AddNode(row[2], row[3], std::atof(row[4]), false, "");
    }

    // Bellek temizleme
    mysql_free_result(res);
}


void Database::select_saved_expenses(){
    // SELECT komutu (hesaplar tablosundaki tüm verileri çekiyoruz)
    std::string select_query = "SELECT* FROM saved WHERE type_=1 and user_id=" + std::to_string(user_id);

    // Sorguyu çalıştır
    if (mysql_query(conn, select_query.c_str())) {
        std::cerr << "SELECT* FROM saved error: " << mysql_error(conn) << std::endl;
    }   


    // Sorgu sonucunu al
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    }   
    saved_expenses_list->clear();
    // Sonuçları ekrana yazdır
    while ((row = mysql_fetch_row(res))) {
        saved_expenses_list->AddNode(row[2], row[3], std::atof(row[4]), false, "");
    }

    // Bellek temizleme
    mysql_free_result(res);
}


void Database::add_saved_earning(std::string header, std::string description, float amount){
    std::string add_saved_earning_procedure = "CALL add_saved_earning(" + std::to_string(user_id) + ", '" + header + "', '" + description + "', " + std::to_string(amount) + ");";
    if (mysql_query(conn, add_saved_earning_procedure.c_str())) {
        std::cerr << "add_saved_earning error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::add_saved_expense(std::string header, std::string description, float amount){
    std::string add_saved_expense_procedure = "CALL add_saved_expense(" + std::to_string(user_id) + ", '" + header + "', '" + description + "', " + std::to_string(amount) + ");";
    if (mysql_query(conn, add_saved_expense_procedure.c_str())) {
        std::cerr << "add_saved_expense error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::clear_table(std::string table_name, int month, std::string year){
    std::string delete_procedure = "DELETE FROM " + table_name + " where MONTH(saved_date) =" + std::to_string(month + 1) + " and YEAR(saved_date) ='" + year + "' and user_id=" + std::to_string(user_id);
    if (mysql_query(conn, delete_procedure.c_str())) {
        std::cerr << "DELETE error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::clear_saved_table(){
    std::string delete_procedure = "DELETE FROM saved WHERE user_id=" + std::to_string(user_id);
    if (mysql_query(conn, delete_procedure.c_str())) {
        std::cerr << "DELETE FROM saved error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


bool Database::is_user_valid(const std::string& username, const std::string& password) {
    // Sorgu hazırlığı
    std::string user_control_procedure = "CALL is_user_valid('" + username + "', '" + password + "');";

    // Sorguyu çalıştır
    if (mysql_query(conn, user_control_procedure.c_str())) {
        std::cerr << "Query error: " << mysql_error(conn) << std::endl;
        return false;  // Hata durumunda false döndür
    }

    // Sonuçları al
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Error retrieving result: " << mysql_error(conn) << std::endl;
        return false;
    }

    // Sonuçları kontrol et
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != nullptr) {
        // 'IsValid' kolonunun değerini al (bool tipinde)
        int is_valid = std::stoi(row[0]); // Boolean değerini int olarak alabiliriz (0 = false, 1 = true)
        mysql_free_result(result);  // Sonuçları serbest bırak
        while (mysql_next_result(conn) == 0) {
            MYSQL_RES* extra_res = mysql_store_result(conn);
            if (extra_res) {
                mysql_free_result(extra_res);
            }
        }
        if(is_valid == 1){
            get_user_id(username, password);
        }
        return is_valid == 1;  // Eğer 1 ise true, 0 ise false döndür
    } else {
        std::cerr << "No results returned." << std::endl;
        mysql_free_result(result);  // Sonuçları serbest bırak   
        while (mysql_next_result(conn) == 0) {
            MYSQL_RES* extra_res = mysql_store_result(conn);
            if (extra_res) {
                mysql_free_result(extra_res);
            }
        }
        return false;
    }
}


void Database::get_user_id(const std::string& username, const std::string& password){
    // Sorgu hazırlığı
    std::string user_control_procedure = "SELECT* FROM users WHERE username='" + username + "' and pass='" + password + "'";

    // Sorguyu çalıştır
    if (mysql_query(conn, user_control_procedure.c_str())) {
        std::cerr << "Query error: " << mysql_error(conn) << std::endl;
    }

    // Sonuçları al
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Error retrieving result: " << mysql_error(conn) << std::endl;
    }

    // Sonuçları kontrol et
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != nullptr) {
        // 'IsValid' kolonunun değerini al (bool tipinde)
        int id = std::stoi(row[0]); // Boolean değerini int olarak alabiliriz (0 = false, 1 = true)
        mysql_free_result(result);  // Sonuçları serbest bırak
        while (mysql_next_result(conn) == 0) {
            MYSQL_RES* extra_res = mysql_store_result(conn);
            if (extra_res) {
                mysql_free_result(extra_res);
            }
        }
        this->user_id = id;
    } else {
        std::cerr << "No results returned." << std::endl;
        mysql_free_result(result);  // Sonuçları serbest bırak   
        while (mysql_next_result(conn) == 0) {
            MYSQL_RES* extra_res = mysql_store_result(conn);
            if (extra_res) {
                mysql_free_result(extra_res);
            }
        }
    }
}


void Database::add_user(const std::string& name, const std::string& surname, const std::string& username, const std::string& email, const std::string& password){
    std::string add_saved_earning_procedure = "CALL add_user('" + name + "', '" + surname + "', '" + username + "', '" + email + "', '" + password + "');";
    if (mysql_query(conn, add_saved_earning_procedure.c_str())) {
        std::cerr << "add_user error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::get_user(std::string& name, std::string& surname, std::string& username, std::string& email){
    std::string query = "call get_user(" + std::to_string(user_id) + ")";
    // Sorguyu çalıştır
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "get_user error: " << mysql_error(conn) << std::endl;
    }  

    // Sonuçları al
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Error retrieving result: " << mysql_error(conn) << std::endl;
    }

    // Sonuçları kontrol et
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != nullptr) {
        // 'IsValid' kolonunun değerini al (bool tipinde)        
        name = row[1];
        surname = row[2];
        username = row[3];
        email = row[4];
        mysql_free_result(result);  // Sonuçları serbest bırak
        while (mysql_next_result(conn) == 0) {
            MYSQL_RES* extra_res = mysql_store_result(conn);
            if (extra_res) {
                mysql_free_result(extra_res);
            }
        }
    } else {
        std::cerr << "No results returned." << std::endl;
        mysql_free_result(result);  // Sonuçları serbest bırak   
        while (mysql_next_result(conn) == 0) {
            MYSQL_RES* extra_res = mysql_store_result(conn);
            if (extra_res) {
                mysql_free_result(extra_res);
            }
        }
    }
}


void Database::update_user(std::string name, std::string surname, std::string username, std::string email){
    std::string query = "call update_user(" + std::to_string(user_id) + ", '" + name + "', '" + surname + "', '" + username + "', '" + email + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "update_user error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::update_password(std::string new_password){
    std::string query = "call update_password(" + std::to_string(user_id) + ", '" + new_password + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "update_password error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::delete_user(){
    std::string delete_user_procedure = "CALL delete_user(" + std::to_string(user_id) + ");";
    if (mysql_query(conn, delete_user_procedure.c_str())) {
        std::cerr << "DELETE error: " << mysql_error(conn) << std::endl;
    }
    else {   
        consumeResults();
    }
}


void Database::get_monthly_statistics(int month, std::string year, std::vector<double> &total_earnings,std::vector<double> &total_expenses,std::vector<double> &total){
    // SELECT komutu (hesaplar tablosundaki tüm verileri çekiyoruz)
    std::string query = "call get_monthly_statics(" + std::to_string(user_id) + ", "+ std::to_string(month + 1) + ", '" + year + "')";

    // Sonuçları ekrana yazdır
    total_earnings.push_back(0);
    total_expenses.push_back(0);
    total.push_back(0);
       
    // Sorguyu çalıştır
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "get_monthly_statics error: " << mysql_error(conn) << std::endl;
    }   


    // Sorgu sonucunu al
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    }   
    // Sonuçları ekrana yazdır
    while ((row = mysql_fetch_row(res))) {
        // Verileri al
        double total_earnings_ = (row[0] != NULL) ? std::stod(row[0]) : 0.0; // total_earnings sütununu al
        double total_expenses_ = (row[1] != NULL) ? std::stod(row[1]) : 0.0;  // total_expenses sütununu al
        double total_ = (row[2] != NULL) ? std::stod(row[2]) : 0.0;            // total sütununu al

        // Verileri statistics vektörüne ekle
        total_earnings.push_back(total_earnings_);
        total_expenses.push_back(total_expenses_);
        total.push_back(total_);
    }

    // Bellek temizleme
    mysql_free_result(res);
    do {
        res = mysql_store_result(conn);
        if (res) {
            mysql_free_result(res); // Belleği temizle
        }
    } while (mysql_next_result(conn) == 0);
}


void Database::get_yearly_statistics(std::string year, std::vector<double> &total_earnings,std::vector<double> &total_expenses,std::vector<double> &total){
    // SELECT komutu (hesaplar tablosundaki tüm verileri çekiyoruz)
    std::string query = "call get_yearly_statics(" + std::to_string(user_id) + ", '"+ year + "')";

    // Sonuçları ekrana yazdır
    total_earnings.push_back(0);
    total_expenses.push_back(0);
    total.push_back(0);
       
    // Sorguyu çalıştır
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "get_yearly_statics error: " << mysql_error(conn) << std::endl;
    }   


    // Sorgu sonucunu al
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "Result error" << mysql_error(conn) << std::endl;
    }   
    // Sonuçları ekrana yazdır
    while ((row = mysql_fetch_row(res))) {
        // Verileri al
        double total_earnings_ = (row[0] != NULL) ? std::stod(row[0]) : 0.0; // total_earnings sütununu al
        double total_expenses_ = (row[1] != NULL) ? std::stod(row[1]) : 0.0;  // total_expenses sütununu al
        double total_ = (row[2] != NULL) ? std::stod(row[2]) : 0.0;            // total sütununu al


        // Verileri statistics vektörüne ekle
        total_earnings.push_back(total_earnings_);
        total_expenses.push_back(total_expenses_);
        total.push_back(total_);
    }

    // Bellek temizleme
    mysql_free_result(res);
    do {
        res = mysql_store_result(conn);
        if (res) {
            mysql_free_result(res); // Belleği temizle
        }
    } while (mysql_next_result(conn) == 0);
}

