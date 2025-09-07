#include "login.h"
#include <filesystem>

RegisterWindow::RegisterWindow(Gtk::Window& parent, Database* db) 
: Gtk::Dialog("Create Account", parent), db(db) {
    set_default_size(300, 300);
    // Box ve Butonlar

    
    password_entry.set_visibility(false);

    vbox.pack_start(first_name_label);
    vbox.pack_start(first_name_entry);
    vbox.pack_start(last_name_label);
    vbox.pack_start(last_name_entry);
    vbox.pack_start(username_label);
    vbox.pack_start(username_entry);
    vbox.pack_start(email_label);
    vbox.pack_start(email_entry);
    vbox.pack_start(password_label);
    vbox.pack_start(password_entry);
    vbox.pack_start(register_button);
    get_content_area()->pack_start(vbox);
    // Kayıt butonunun sinyali
    register_button.signal_clicked().connect(sigc::mem_fun(*this, &RegisterWindow::on_register_clicked));
    show_all_children();
}


void RegisterWindow::on_register_clicked() {
    std::string first_name = first_name_entry.get_text();
    std::string last_name = last_name_entry.get_text();
    std::string username = username_entry.get_text();
    std::string email = email_entry.get_text();
    std::string password = password_entry.get_text();
    if (first_name.empty() || last_name.empty() || username.empty() || email.empty() || password.empty()) {
        Gtk::MessageDialog dialog(*this, "Please fill in all fields!", false, Gtk::MESSAGE_ERROR);
        dialog.run();
    } else {
        Gtk::MessageDialog dialog(*this, "Registration successful! Your account has been created.", false, Gtk::MESSAGE_INFO);
        dialog.run();
        db->add_user(first_name, last_name, username, email, password);
        response(Gtk::RESPONSE_ACCEPT);
    }
}


LoginWindow::LoginWindow() {
    set_title("Login");
    set_default_size(300, 200);

    try {
        Glib::RefPtr<Gio::Resource> resource = Gio::Resource::create_from_file("resources.gresource");
        resource->register_global();
        Glib::RefPtr<Gdk::Pixbuf> icon = Gdk::Pixbuf::create_from_resource("/com/clytron/accountant_manager/resources/icon.png");
        set_icon(icon);
    } catch (const Glib::Error& e) {
        std::cerr << "Failed to load GResource: " << e.what() << std::endl;
    }

    db = new Database();
    password_entry.set_visibility(false);
    // Box ve Butonlar
    vbox.pack_start(username_label);
    vbox.pack_start(username_entry);
    vbox.pack_start(password_label);
    vbox.pack_start(password_entry);
    vbox.pack_start(login_button);
    vbox.pack_start(create_account_button);
    add(vbox);
    // Butonların sinyalleri
    login_button.signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::on_login_clicked));
    create_account_button.signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::on_create_account_clicked));
    show_all_children();
}


void LoginWindow::on_login_clicked() {
    std::string username = username_entry.get_text();
    std::string password = password_entry.get_text();
    username_entry.set_text("");
    password_entry.set_text("");
    if (db->is_user_valid(username, password)) {
        
        std::string name = "";
        std::string surname = "";
        std::string email = "";

        db->get_user(name, surname, username, email);
        account = new Account(db, name, surname, username, email);

        auto new_win = new MyWindow(db, account);
        Glib::RefPtr<Gtk::Application> app = this->get_application();

        app->add_window(*new_win);
        app->remove_window(*this);
        this->hide();
        auto delete_self = [this]() { 
            delete this; 
        };
        new_win->show();
        
    } else {
        Gtk::MessageDialog dialog(*this, "Invalid username and password!", false, Gtk::MESSAGE_ERROR);
        dialog.run();
    }
}


void LoginWindow::on_create_account_clicked() {
    RegisterWindow register_window(*this, db);  // LoginWindow'u parent olarak geçir
    register_window.run();  // Kayıt penceresini başlat
}

