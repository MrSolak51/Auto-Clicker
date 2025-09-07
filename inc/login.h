#ifndef LOGIN_H

#include "gui.h"

class RegisterWindow : public Gtk::Dialog {
public:
    RegisterWindow(Gtk::Window& parent, Database* db);

protected:
    // Kayıt işlemi
    void on_register_clicked();

private:
    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL, 5};
    Gtk::Label first_name_label{"Name:"};
    Gtk::Entry first_name_entry;
    Gtk::Label last_name_label{"Surname:"};
    Gtk::Entry last_name_entry;
    Gtk::Label username_label{"Username:"};
    Gtk::Entry username_entry;
    Gtk::Label email_label{"Email:"};
    Gtk::Entry email_entry;
    Gtk::Label password_label{"Password:"};
    Gtk::Entry password_entry;
    Gtk::Button register_button{"Create Account"};
    Database *db;
};

class LoginWindow : public Gtk::Window {
public:
    LoginWindow();
protected:
    // Giriş işlemi
    void on_login_clicked();

    // Hesap oluşturma
    void on_create_account_clicked();

private:
    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL, 5};
    Gtk::Label username_label{"Username:"};
    Gtk::Entry username_entry;
    Gtk::Label password_label{"Password:"};
    Gtk::Entry password_entry;
    Gtk::Button login_button{"Login"};
    Gtk::Button create_account_button{"Create Account"};
    Database *db;
    Account * account;
};    

#endif //LOGIN_H