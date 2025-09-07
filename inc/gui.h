#ifndef GUI_H

#include "boxes.h"
#include "account.h"
#include "account_gui.h"
#include "graph.h"

class MyWindow : public Gtk::Window {
public:
    MyWindow(Database* db, Account * account);
    ~MyWindow();
protected:
    void on_date_changed();
    void on_add_earning_clicked();
    void on_add_expense_clicked();
    void on_edit_earning_clicked();
    void on_edit_expense_clicked();
    void on_delete_earning_clicked();
    void on_delete_expense_clicked();

    
    void on_add_saved_earning_clicked();
    void on_add_saved_expense_clicked();
    void on_edit_saved_earning_clicked();
    void on_edit_saved_expense_clicked();
    void on_delete_saved_earning_clicked();
    void on_delete_saved_expense_clicked();

    void on_account_clicked();
    void on_monthly_statistics_clicked();
    void on_yearly_statistics_clicked();
    
    bool on_exit(GdkEventAny* event);
    void on_exit_clicked();
    void on_save_clicked();
private:
    Gtk::Box m_box,m_box2,m_box3;
    Gtk::ComboBoxText month_combobox;
    Gtk::SpinButton year_spinbox;
    Gtk::Label total_label;
    Glib::RefPtr<Gtk::Adjustment> adjustment;

    List *earnings_list, *expenses_list;
    List *saved_earnings_list, *saved_expenses_list;
    
    Database *db;
    Account *account;

    EarningsBox *e;
    ExpensesBox *ex;
    
    bool is_save;
    int month;
    std::string year;
};

#endif //GUI_H