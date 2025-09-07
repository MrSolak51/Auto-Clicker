#ifndef BOXES_H

#include "dialog.h"

class IBox :public Gtk::Box{
public:
    Gtk::Button add_button, delete_button, saved_button;
    Gtk::ListBox list_box;
    IBox(Gtk::Window* window, bool &is_save, List *list, List * saved_list, Database * db);
    virtual void add_saved() = 0;
    virtual void edit_saved() = 0;
    virtual void delete_saved() = 0;
    virtual void edit() = 0;
    virtual void delete_() = 0;
protected:
    Gtk::Window* window;
    Gtk::Box m_box;
    int item_count = 0;
    Gtk::ScrolledWindow scrolled_window;
    Gtk::Label box_title;
    
    List* list;
    List* saved_list;
    Database * db;

    virtual void refresh_list(int month, std::string year) = 0;
    virtual void on_add_button_clicked() = 0;
    virtual void on_delete_button_clicked() = 0;
    virtual void on_saved_button_clicked() = 0;
    virtual void on_list_box_row_selected(Gtk::ListBoxRow* row) = 0;
    virtual bool on_listbox_button_press(GdkEventButton* event) = 0;
    void clear_listbox();

    bool &is_save;
};      
class EarningsBox :public IBox {
public:
    EarningsBox(Gtk::Window* window, bool &is_save, List *list, List *saved_list, Database* db);
    void refresh_list(int month, std::string year) override;
    void add_saved() override;
    void edit_saved() override;
    void delete_saved() override;
    void edit() override;
    void delete_() override;

    ~EarningsBox();
protected:
    void on_add_button_clicked() override;
    void on_delete_button_clicked() override;
    void on_saved_button_clicked() override;
    void on_list_box_row_selected(Gtk::ListBoxRow* row) override;
    bool on_listbox_button_press(GdkEventButton* event) override;
private:
};
class ExpensesBox :public IBox {
public:
    ExpensesBox(Gtk::Window* window, bool &is_save, List *list, List *saved_list, Database* db);
    void refresh_list(int month, std::string year) override;
    void add_saved() override;
    void edit_saved() override;
    void delete_saved() override;
    void edit() override;
    void delete_() override;

    ~ExpensesBox();
protected:
    void on_add_button_clicked() override;
    void on_delete_button_clicked() override;
    void on_saved_button_clicked() override;
    void on_list_box_row_selected(Gtk::ListBoxRow* row) override;
    bool on_listbox_button_press(GdkEventButton* event) override;
private:
};
#endif //BOXES_H