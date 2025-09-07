#ifndef ACCOUNTGUI_H

#include <gtkmm.h>
#include "account.h"

class AccountGui : public Gtk::Window {
public:
    AccountGui(Account * account);

private:
    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL, 5};
    Gtk::Entry entry_username, entry_name, entry_surname, entry_email, entry_password;
    Gtk::Button button_update, button_delete;

    void add_labels_and_entries();
    void add_entry(const Glib::ustring& label_text, Gtk::Entry& entry, const Glib::ustring& default_value, bool is_password = false);
    void on_update_clicked();
    void on_delete_clicked();
    bool password_control();

    Account * account;
};

#endif //ACCOUNTGUI_H