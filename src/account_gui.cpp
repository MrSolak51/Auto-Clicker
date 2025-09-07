#include "account_gui.h"

AccountGui::AccountGui(Account * account):
account(account) {
    set_title("Account");
    set_default_size(300, 250);

    try {
        Glib::RefPtr<Gio::Resource> resource = Gio::Resource::create_from_file("resources.gresource");
        resource->register_global();
        Glib::RefPtr<Gdk::Pixbuf> icon = Gdk::Pixbuf::create_from_resource("/com/clytron/accountant_manager/resources/icon.png");
        set_icon(icon);
    } catch (const Glib::Error& e) {
        std::cerr << "Failed to load GResource: " << e.what() << std::endl;
    }

    // Etiket ve Giriş Kutularını Ekle
    add_labels_and_entries();

    // Güncelle Butonu
    button_update.set_label("Update Account");
    button_update.signal_clicked().connect(sigc::mem_fun(*this, &AccountGui::on_update_clicked));
    vbox.pack_start(button_update, Gtk::PACK_SHRINK);

    // Sil Butonu
    button_delete.set_label("DELETE ACCOUNT");
    button_delete.signal_clicked().connect(sigc::mem_fun(*this, &AccountGui::on_delete_clicked));
    vbox.pack_start(button_delete, Gtk::PACK_SHRINK);
    
    add(vbox);
    show_all_children();
}


void AccountGui::add_labels_and_entries() {
    add_entry("Username:", entry_username, account->get_username());
    add_entry("Name:", entry_name, account->get_name());
    add_entry("Surname:", entry_surname, account->get_surname());
    add_entry("E-Mail:", entry_email, account->get_email());
    add_entry("Password:", entry_password, "", true);
}


void AccountGui::add_entry(const Glib::ustring& label_text, Gtk::Entry& entry, const Glib::ustring& default_value, bool is_password) {
    auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 5);
    auto label = Gtk::make_managed<Gtk::Label>(label_text);
    
    entry.set_text(default_value);
    if (is_password) entry.set_visibility(false);
    
    hbox->pack_start(*label, Gtk::PACK_SHRINK);
    hbox->pack_start(entry);
    vbox.pack_start(*hbox, Gtk::PACK_SHRINK);
}


void AccountGui::on_update_clicked() {
    
    if (password_control())
    {
        if (entry_password.get_text().empty())
        {    
            account->update(entry_name.get_text(), entry_surname.get_text(), entry_username.get_text(), entry_email.get_text());
            account->update_password(entry_password.get_text());
        }
        else{
            account->update(entry_name.get_text(), entry_surname.get_text(), entry_username.get_text(), entry_email.get_text());
        }
        Gtk::MessageDialog dialog(*this, "Profile updated\nLogin again...", false, Gtk::MESSAGE_INFO);
        dialog.run();
        exit(0);
    }
    else
    {
        Gtk::MessageDialog dialog(*this, "Invalid password!", false, Gtk::MESSAGE_ERROR);
        dialog.run();
    }
}


void AccountGui::on_delete_clicked() {
    if (password_control())
    {
        account->delete_();
        Gtk::MessageDialog dialog(*this, "Profile deleted\nLogin again...", false, Gtk::MESSAGE_INFO);
        dialog.run();
        exit(1);
    }
    else
    {
        Gtk::MessageDialog dialog(*this, "Invalid password!", false, Gtk::MESSAGE_ERROR);
        dialog.run();
    }
    
}


bool AccountGui::password_control(){
    Gtk::Dialog dialog("Password Control", *this, true);
    Gtk::Box* content_area = dialog.get_content_area();

    Gtk::Entry password_entry;
    Gtk::Label password_label;

    password_label.set_text("Password");

    password_entry.set_visibility(false);
    password_entry.set_placeholder_text("exapmle1234...");
    content_area->pack_start(password_label);
    content_area->pack_start(password_entry);

    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("OK", Gtk::RESPONSE_OK);

    dialog.show_all_children();

    int result = dialog.run();

    if (result == Gtk::RESPONSE_OK)
        return account->db->is_user_valid(account->get_username(), password_entry.get_text());
    return false;
}

