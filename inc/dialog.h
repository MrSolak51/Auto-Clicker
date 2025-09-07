#ifndef DIALOG_H


#include "date_picker.h"
#include <ctime>
#include "database.h"

class IAdderDialog : public Gtk::Dialog {
public:
    IAdderDialog(Gtk::Window& parent, std::string header, std::string description, float amount);
    std::string get_header_text() const;
    std::string get_description_text() const;
    float get_amount() const;
    bool is_paid() const;
    std::string get_date() const;
protected:
    std::string title;
    Gtk::Box m_box, m_box2, m_box3, m_box4;
    Gtk::Entry header_entry, description_entry;
    Gtk::Button date_button;
    
    Glib::RefPtr<Gtk::Adjustment> adjustment;
    Gtk::SpinButton amount_spinbutton;     
    Gtk::CheckButton paid_checkbutton;
    std::string date;
private:
};

class SavedDialog : public IAdderDialog {
public:
    SavedDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, List * saved_listP, std::string date);
    int get_index();
protected:
    void on_saved_combobox_changed();
    void on_date_button_clicked();
private:
    Gtk::ComboBoxText saved_combobox;
    List * saved_list;
};

class AddDialog : public IAdderDialog {
public:
    AddDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, std::string date);
protected:
    void on_date_button_clicked();
};

class EditDialog : public IAdderDialog {
public:
    EditDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, std::string date);  
protected:
    void on_date_button_clicked();
};   

class AddSavedDialog : public IAdderDialog {
public:
    AddSavedDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount);
};
class EditSavedDialog : public IAdderDialog {
    public:
        EditSavedDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, List * saved_listP);
        int get_index();
    protected:
        void on_saved_combobox_changed();
    private:
        Gtk::ComboBoxText saved_combobox;
        List * saved_list;
    };
class DeleteSavedDialog : public Gtk::Dialog {
public:
    DeleteSavedDialog(Gtk::Window& parent, List * saved_listP);
    int get_index();
protected:
    std::string title;
    Gtk::Box m_box;
private:
    Gtk::ComboBoxText saved_combobox;
    List * saved_list;
};
class EditDialog_ : public IAdderDialog {
public:
    EditDialog_(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, std::string date, List * listP);
    int get_index();
protected:
    void on_saved_combobox_changed();
    void on_date_button_clicked();
private:
    Gtk::ComboBoxText saved_combobox;
    List * list;
};
class DeleteDialog : public Gtk::Dialog {
public:
DeleteDialog(Gtk::Window& parent, List * listP);
    int get_index();
protected:
    std::string title;
    Gtk::Box m_box;
private:
    Gtk::ComboBoxText saved_combobox;
    List * list;
};
#endif //DIALOG_H