#include "gui.h"

MyWindow::MyWindow(Database*db, Account * account) 
: db(db), account(account) {
    is_save = true;
    set_title("Preminiliary Accountant - " + account->get_username());
    set_default_size(400, 300);
    
    try {
        Glib::RefPtr<Gio::Resource> resource = Gio::Resource::create_from_file("resources.gresource");
        resource->register_global();
        Glib::RefPtr<Gdk::Pixbuf> icon = Gdk::Pixbuf::create_from_resource("/com/clytron/accountant_manager/resources/icon.png");
        set_icon(icon);
    } catch (const Glib::Error& e) {
        std::cerr << "Failed to load GResource: " << e.what() << std::endl;
    }
    
    earnings_list = new List();
    expenses_list = new List();
    saved_earnings_list = new List();
    saved_expenses_list = new List();


    db->set_earnings_list(earnings_list);
    db->set_expenses_list(expenses_list);
    db->set_saved_earnings_list(saved_earnings_list);
    db->set_saved_expenses_list(saved_expenses_list);

    db->select_saved_earnings();
    db->select_saved_expenses();

    e = new EarningsBox(this, is_save, earnings_list, saved_earnings_list, db);
    ex = new ExpensesBox(this, is_save, expenses_list, saved_expenses_list, db);
    
    auto accel_group = Gtk::AccelGroup::create();
    add_accel_group(accel_group);

    // Menü çubuğunu ve menü öğelerini oluştur
    auto menubar = Gtk::make_managed<Gtk::MenuBar>();

    // **File Menüsü**
    auto file_menu = Gtk::make_managed<Gtk::Menu>();
    auto file_item = Gtk::make_managed<Gtk::MenuItem>("File");
    file_item->set_submenu(*file_menu);

    auto save_item = Gtk::make_managed<Gtk::MenuItem>("Save");
    auto exit_item = Gtk::make_managed<Gtk::MenuItem>("Exit");



    save_item->add_accelerator("activate", accel_group, GDK_KEY_s, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    exit_item->add_accelerator("activate", accel_group, GDK_KEY_q, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);

    save_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_save_clicked));
    exit_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_exit_clicked));


    file_menu->append(*save_item);
    file_menu->append(*exit_item);

    // **Process Menüsü**
    auto process_menu = Gtk::make_managed<Gtk::Menu>();
    auto process_item = Gtk::make_managed<Gtk::MenuItem>("Process");
    process_item->set_submenu(*process_menu);

    auto add_menu = Gtk::make_managed<Gtk::Menu>();
    auto add_item = Gtk::make_managed<Gtk::MenuItem>("Add");
    add_item->set_submenu(*add_menu);

    auto add_earning_item = Gtk::make_managed<Gtk::MenuItem>("Earnings");
    add_earning_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_add_earning_clicked));
    auto add_expense_item = Gtk::make_managed<Gtk::MenuItem>("Expense");
    add_expense_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_add_expense_clicked));

    add_menu->append(*add_earning_item);
    add_menu->append(*add_expense_item);

    
    auto edit_menu = Gtk::make_managed<Gtk::Menu>();
    auto edit_item = Gtk::make_managed<Gtk::MenuItem>("Edit");
    edit_item->set_submenu(*edit_menu);

    auto edit_earning_item = Gtk::make_managed<Gtk::MenuItem>("Earnings");
    edit_earning_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_edit_earning_clicked));
    auto edit_expense_item = Gtk::make_managed<Gtk::MenuItem>("Expense");
    edit_expense_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_edit_expense_clicked));

    edit_menu->append(*edit_earning_item);
    edit_menu->append(*edit_expense_item);

    
    auto delete_menu = Gtk::make_managed<Gtk::Menu>();
    auto delete_item = Gtk::make_managed<Gtk::MenuItem>("Delete");
    delete_item->set_submenu(*delete_menu);

    auto delete_earning_item = Gtk::make_managed<Gtk::MenuItem>("Earnings");
    delete_earning_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_delete_earning_clicked));
    auto delete_expense_item = Gtk::make_managed<Gtk::MenuItem>("Expense");
    delete_expense_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_delete_expense_clicked));

    delete_menu->append(*delete_earning_item);
    delete_menu->append(*delete_expense_item);


    process_menu->append(*add_item);
    process_menu->append(*edit_item);
    process_menu->append(*delete_item);

    // **Saved Menüsü**
    auto saved_menu = Gtk::make_managed<Gtk::Menu>();
    auto saved_item = Gtk::make_managed<Gtk::MenuItem>("Saved");
    saved_item->set_submenu(*saved_menu);

    auto add_saved_menu = Gtk::make_managed<Gtk::Menu>();
    auto add_saved_item = Gtk::make_managed<Gtk::MenuItem>("Add");
    add_saved_item->set_submenu(*add_saved_menu);

    auto add_saved_earning_item = Gtk::make_managed<Gtk::MenuItem>("Earnings");
    add_saved_earning_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_add_saved_earning_clicked));
    auto add_saved_expense_item = Gtk::make_managed<Gtk::MenuItem>("Expense");
    add_saved_expense_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_add_saved_expense_clicked));

    add_saved_menu->append(*add_saved_earning_item);
    add_saved_menu->append(*add_saved_expense_item);

    
    auto edit_saved_menu = Gtk::make_managed<Gtk::Menu>();
    auto edit_saved_item = Gtk::make_managed<Gtk::MenuItem>("Edit");
    edit_saved_item->set_submenu(*edit_saved_menu);

    auto edit_saved_earning_item = Gtk::make_managed<Gtk::MenuItem>("Earnings");
    edit_saved_earning_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_edit_saved_earning_clicked));
    auto edit_saved_expense_item = Gtk::make_managed<Gtk::MenuItem>("Expense");
    edit_saved_expense_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_edit_saved_expense_clicked));

    edit_saved_menu->append(*edit_saved_earning_item);
    edit_saved_menu->append(*edit_saved_expense_item);

    
    auto delete_saved_menu = Gtk::make_managed<Gtk::Menu>();
    auto delete_saved_item = Gtk::make_managed<Gtk::MenuItem>("Delete");
    delete_saved_item->set_submenu(*delete_saved_menu);

    auto delete_saved_earning_item = Gtk::make_managed<Gtk::MenuItem>("Earnings");
    delete_saved_earning_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_delete_saved_earning_clicked));
    auto delete_saved_expense_item = Gtk::make_managed<Gtk::MenuItem>("Expense");
    delete_saved_expense_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_delete_saved_expense_clicked));

    delete_saved_menu->append(*delete_saved_earning_item);
    delete_saved_menu->append(*delete_saved_expense_item);


    saved_menu->append(*add_saved_item);
    saved_menu->append(*edit_saved_item);
    saved_menu->append(*delete_saved_item);


    auto account_menu = Gtk::make_managed<Gtk::Menu>();
    auto account_item = Gtk::make_managed<Gtk::MenuItem>(this->account->get_username());
    account_item->set_submenu(*account_menu);


    auto account_info_item = Gtk::make_managed<Gtk::MenuItem>("Account");
    account_info_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_account_clicked));



    auto stats_menu = Gtk::make_managed<Gtk::Menu>();

    auto stats_item = Gtk::make_managed<Gtk::MenuItem>("Statistics");

    
    stats_item->set_submenu(*stats_menu);

    auto monthly_item = Gtk::make_managed<Gtk::MenuItem>("Monthly");    
    monthly_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_monthly_statistics_clicked));

    auto yearly_item = Gtk::make_managed<Gtk::MenuItem>("Yearly");    
    yearly_item->signal_activate().connect(sigc::mem_fun(*this, &MyWindow::on_yearly_statistics_clicked));
    
    stats_menu->append(*monthly_item);
    stats_menu->append(*yearly_item);





    account_menu->append(*account_info_item);
    account_menu->append(*stats_item);


    // Menü çubuğuna öğeleri ekle
    menubar->append(*file_item);
    menubar->append(*process_item);
    menubar->append(*saved_item);
    menubar->append(*account_item);


    m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box2.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    m_box3.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    month_combobox.append("January");
    month_combobox.append("February");
    month_combobox.append("March");
    month_combobox.append("April");
    month_combobox.append("May");
    month_combobox.append("June");
    month_combobox.append("July");
    month_combobox.append("August");
    month_combobox.append("September");
    month_combobox.append("October");
    month_combobox.append("November");
    month_combobox.append("December");



    std::time_t now = std::time(nullptr);

    // Yerel zamana dönüştür
    std::tm* local_time = std::localtime(&now);

    // Ay ve yılı al
    int year_ = local_time->tm_year + 1900; // tm_year 1900'den itibaren yılı verir
    month = local_time->tm_mon;    // tm_mon 0'dan başlar (Ocak = 0)

    year = std::to_string(year_);

    
    adjustment = Gtk::Adjustment::create(1000, 1000, 9999, 1, 10, 0);
    year_spinbox.set_adjustment(adjustment);
    year_spinbox.set_numeric(true);  // Sadece sayı girişine izin ver
    year_spinbox.set_wrap(false);    // Döngüye girmesin
    year_spinbox.set_digits(0);      // Ondalık basamak kullanma
    year_spinbox.set_value(year_);

    year_spinbox.signal_value_changed().connect(sigc::mem_fun(*this, &MyWindow::on_date_changed));
    
    month_combobox.signal_changed().connect(sigc::mem_fun(*this, &MyWindow::on_date_changed));
    month_combobox.set_active(month);

    m_box2.pack_start(month_combobox);
    m_box2.pack_start(year_spinbox, Gtk::PACK_SHRINK);

    m_box.pack_start(*menubar, Gtk::PACK_SHRINK);
    m_box.pack_start(m_box2, Gtk::PACK_SHRINK);
    m_box.pack_start(total_label, Gtk::PACK_SHRINK);
    m_box.pack_start(m_box3);

    m_box3.pack_start(*e);
    m_box3.pack_start(*ex);

    add(m_box);

    signal_delete_event().connect(sigc::mem_fun(*this, &MyWindow::on_exit));

    show_all_children();
}


MyWindow::~MyWindow(){
    delete e;
    delete ex;
}


void MyWindow::on_date_changed() {
    // Seçili öğeyi al
    if (is_save) {       
        int index = month_combobox.get_active_row_number();
        this->month = index;

        db->select_earnings(index, std::to_string((int)(year_spinbox.get_value())));
        db->select_expenses(index, std::to_string((int)(year_spinbox.get_value())));
    
    
        total_label.set_text("Total:" + std::to_string(db->get_total(index, std::to_string((int)(year_spinbox.get_value())))));
        
        e-> refresh_list(index, std::to_string((int)(year_spinbox.get_value())));
        ex->refresh_list(index, std::to_string((int)(year_spinbox.get_value())));
    }
    else {
        Gtk::MessageDialog dialog(*this, "There are unsaved datas", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL);
        dialog.set_secondary_text("If closed without saving, unsaved data will be lost.\nAre you sure you want to close?");

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK) {       
            int index = month_combobox.get_active_row_number();
            this->month = index;
            
            db->select_earnings(index, std::to_string((int)(year_spinbox.get_value())));
            db->select_expenses(index, std::to_string((int)(year_spinbox.get_value())));
        
        
            total_label.set_text("Total:" + std::to_string(db->get_total(index, std::to_string((int)(year_spinbox.get_value())))));
            
            e-> refresh_list(index, std::to_string((int)(year_spinbox.get_value())));
            ex->refresh_list(index, std::to_string((int)(year_spinbox.get_value())));
        }
    }
}


void MyWindow::on_add_earning_clicked(){e->add_button.clicked();}


void MyWindow::on_add_expense_clicked(){ex->add_button.clicked();}


void MyWindow::on_edit_earning_clicked(){e->edit(); e->refresh_list(month, year);}


void MyWindow::on_edit_expense_clicked(){ex->edit(); ex->refresh_list(month, year);}


void MyWindow::on_delete_earning_clicked(){e->delete_(); e->refresh_list(month, year);}


void MyWindow::on_delete_expense_clicked(){ex->delete_(); ex->refresh_list(month, year);}


void MyWindow::on_add_saved_earning_clicked(){e->add_saved();}


void MyWindow::on_add_saved_expense_clicked(){ex->add_saved();}


void MyWindow::on_edit_saved_earning_clicked(){e->edit_saved();}


void MyWindow::on_edit_saved_expense_clicked(){ex->edit_saved();}


void MyWindow::on_delete_saved_earning_clicked(){e->delete_saved();}


void MyWindow::on_delete_saved_expense_clicked(){ex->delete_saved();}


void MyWindow::on_account_clicked(){
    auto account_gui = new AccountGui(account);
    account_gui->set_modal(true);  // Ana  pencereyle etkileşimi engeller
    account_gui->signal_hide().connect([account_gui]() { delete account_gui; });
    account_gui->show();
}


void MyWindow::on_monthly_statistics_clicked(){

    std::vector<double> total_earnings;
    std::vector<double> total_expenses;
    std::vector<double> total_;
    db->get_monthly_statistics(month, year, total_earnings,total_expenses,total_);
    auto monthly_statistic_gui = new LineChartWindow(total_earnings,total_expenses,total_);
    monthly_statistic_gui->set_modal(true);  // Ana pencereyle etkileşimi engeller
    monthly_statistic_gui->signal_hide().connect([monthly_statistic_gui]() { delete monthly_statistic_gui; });
    monthly_statistic_gui->show();
}


void MyWindow::on_yearly_statistics_clicked(){

    std::vector<double> total_earnings;
    std::vector<double> total_expenses;
    std::vector<double> total_;
    db->get_yearly_statistics(year, total_earnings,total_expenses,total_);
    auto yearly_statistic_gui = new LineChartWindow(total_earnings,total_expenses,total_);
    yearly_statistic_gui->set_modal(true);  // Ana  pencereyle etkileşimi engeller
    yearly_statistic_gui->signal_hide().connect([yearly_statistic_gui]() { delete yearly_statistic_gui; });
    yearly_statistic_gui->show();
}


bool MyWindow::on_exit(GdkEventAny* event) {
    on_exit_clicked();
    return true;
}


void MyWindow::on_exit_clicked(){
    if (is_save) {    
        exit(1);
    }
    else {
        Gtk::MessageDialog dialog(*this, "Kaydedilmeyen veriler var", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL);
        dialog.set_secondary_text("Kaydetmeden kapatılırsa kaydedilmeyen veriler kaybolacaktır. Kapatmak istediğinizden emin misiniz?");

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK) { 
            exit(1);
        }
    }
}


void MyWindow::on_save_clicked(){

    db->clear_saved_table();
    for (int i = 0; i < saved_earnings_list->size; i++)
    {
        db->add_saved_earning(saved_earnings_list->get(i)->header, saved_earnings_list->get(i)->description, saved_earnings_list->get(i)->amount);
    }
    for (int i = 0; i < saved_expenses_list->size; i++)
    {
        db->add_saved_expense(saved_expenses_list->get(i)->header, saved_expenses_list->get(i)->description, saved_expenses_list->get(i)->amount);
    }

    db->clear_table("earnings", month, year);
    for (int i = 0; i < earnings_list->size; i++)
    {
        db->add_earning(earnings_list->get(i)->header, earnings_list->get(i)->description, earnings_list->get(i)->amount, earnings_list->get(i)->paid, earnings_list->get(i)->date);
    }

    db->clear_table("expenses", month, year);
    for (int i = 0; i < expenses_list->size; i++)
    {
        db->add_expense(expenses_list->get(i)->header, expenses_list->get(i)->description, expenses_list->get(i)->amount, expenses_list->get(i)->paid, expenses_list->get(i)->date);
    }

    is_save = true;
    set_title("Preminiliary Accountant"); 
}

