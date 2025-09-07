#include "boxes.h"

IBox::IBox(Gtk::Window* window, bool &is_save, List *list, List *saved_list, Database * db)
:window(window), is_save(is_save), list(list), saved_list(saved_list), db(db){
    set_orientation(Gtk::ORIENTATION_VERTICAL);
    box_title.set_alignment(Gtk::POS_LEFT);
    list_box.set_selection_mode(Gtk::SELECTION_SINGLE);
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrolled_window.add(list_box); 


    add_button.set_label("Add");
    delete_button.set_label("DELETE");
    saved_button.set_label("Saved");


    saved_button.set_sensitive(((saved_list->size) > 0) ? true : false);
    
    delete_button.set_sensitive(false);
    
    add_button.signal_clicked().connect(sigc::mem_fun(*this, &IBox::on_add_button_clicked));
    delete_button.signal_clicked().connect(sigc::mem_fun(*this, &IBox::on_delete_button_clicked));
    saved_button.signal_clicked().connect(sigc::mem_fun(*this, &IBox::on_saved_button_clicked));
    list_box.signal_row_selected().connect(sigc::mem_fun(*this, &IBox::on_list_box_row_selected));
    list_box.signal_button_press_event().connect(sigc::mem_fun(*this, &IBox::on_listbox_button_press), false);
    
    pack_start(box_title, Gtk::PACK_SHRINK, 0);
    pack_start(scrolled_window);
    pack_start(add_button, Gtk::PACK_SHRINK);
    pack_start(delete_button, Gtk::PACK_SHRINK);
    pack_start(saved_button, Gtk::PACK_SHRINK);

    show_all_children();
}


void IBox::refresh_list(int month, std::string year){}


void IBox::on_add_button_clicked() {}


void IBox::on_delete_button_clicked() {}


void IBox::on_saved_button_clicked() {}


void IBox::on_list_box_row_selected(Gtk::ListBoxRow* row) {}


bool IBox::on_listbox_button_press(GdkEventButton* event) { return -1;} 


void IBox::clear_listbox() {
    // ListBox'ın tüm çocuklarını al
    auto children = list_box.get_children();

    // Her bir çocuğu ListBox'tan kaldır
    for (auto widget : children) {
        list_box.remove(*widget);
    }
}


EarningsBox::EarningsBox(Gtk::Window* window, bool &is_save, List *list, List *saved_list, Database* db)
: IBox(window, is_save, list, saved_list, db){
    box_title.set_text("Earnings");
}


EarningsBox::~EarningsBox(){}


void EarningsBox::refresh_list(int month, std::string year){
    clear_listbox();
    Node * temp = list->get();
    while (temp != NULL)
    {
        auto row = Gtk::manage(new Gtk::ListBoxRow());
        auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 5));
        auto label = Gtk::manage(new Gtk::Label(temp->header));
        label->set_size_request(100, 30);
        label->set_margin_top(5);
        label->set_margin_bottom(5);
        label->set_margin_start(10);
        
        box->pack_start(*label);
        row->add(*box);
        list_box.append(*row);
        temp = temp->next;
    }
    box_title.set_text("Total Earnings:" + std::to_string(db->get_total_earnings(month, year)));
    show_all_children();
}


void EarningsBox::add_saved(){ 
    std::string header;
    std::string description;
    float amount;
    AddSavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Saved Earning", header, description, amount);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        
        saved_list->AddNode(header, description, amount, false, "");
        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void EarningsBox::delete_saved(){
    DeleteSavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), saved_list);
    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        int index = dialog.get_index();

        saved_list->DeleteNode(index);

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void EarningsBox::edit_saved(){ 
    std::string header;
    std::string description;
    float amount;
    EditSavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Edit Saved", header, description, amount, saved_list);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();

        int index = dialog.get_index();

        saved_list->EditNode(index, header, description, amount, false, "");

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void EarningsBox::edit(){ 
    std::string header;
    std::string description;
    float amount;
    std::string date;
    bool paid;
    EditDialog_ dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Edit Earnings", header, description, amount, paid, date, list);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        date = dialog.get_date();
        paid = dialog.is_paid();

        int index = dialog.get_index();

        list->EditNode(index, header, description, amount, paid, date);

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void EarningsBox::delete_(){
    DeleteDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), list);
    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        int index = dialog.get_index();
        list->DeleteNode(index);

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void EarningsBox::on_add_button_clicked() {
    // Dialog'u başlat
    std::string header;
    std::string description;
    float amount;
    bool paid;
    std::string date;
    AddDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Earnings", header, description, amount, paid, date);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        paid = dialog.is_paid();
        date = dialog.get_date();
        
        list->AddNode(header, description, amount, paid, date);

        auto row = Gtk::manage(new Gtk::ListBoxRow());
        auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 5));
        auto label = Gtk::manage(new Gtk::Label(header));
        label->set_size_request(100, 30);
        label->set_margin_top(5);
        label->set_margin_bottom(5);
        label->set_margin_start(10);
        
        box->pack_start(*label);
        row->add(*box);
        list_box.append(*row);
        is_save = false;window->set_title("Preminiliary Accountant*");
        delete_button.set_sensitive(false);
        item_count++;
        show_all_children();
    }
}


void EarningsBox::on_delete_button_clicked() {        
    Gtk::ListBoxRow* row = list_box.get_selected_row();
    if (row) {
        Gtk::MessageDialog dialog(*this->window, "Are you sure?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
        dialog.set_secondary_text("Are you sure you want to delete earning?");

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK) { 
            list->DeleteNode(row->get_index());
            list_box.remove(*row);
            delete_button.set_sensitive(false);
            is_save = false;window->set_title("Preminiliary Accountant*");
        
        }
    }
}


void EarningsBox::on_saved_button_clicked() {
    // Dialog'u başlat
    std::string header = "";
    std::string description = "";
    float amount;
    bool paid;
    std::string date;
    SavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Earnings", header, description, amount, paid, saved_list, date);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        paid = dialog.is_paid();
        date = dialog.get_date();
        

        auto row = Gtk::manage(new Gtk::ListBoxRow());
        auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 5));
        auto label = Gtk::manage(new Gtk::Label(header));
        label->set_size_request(100, 30);
        label->set_margin_top(5);
        label->set_margin_bottom(5);
        label->set_margin_start(10);
        
        box->pack_start(*label);
        row->add(*box);
        list_box.append(*row);
        is_save = false;window->set_title("Preminiliary Accountant*");
        delete_button.set_sensitive(false);
        item_count++;
        show_all_children();
    }
}


void EarningsBox::on_list_box_row_selected(Gtk::ListBoxRow* row) {
    delete_button.set_sensitive(true);
}


bool EarningsBox::on_listbox_button_press(GdkEventButton* event){
    if (event->type == GDK_2BUTTON_PRESS) {  // Çift tıklama olup olmadığını kontrol et
        Gtk::ListBoxRow* row = list_box.get_row_at_y(event->y);
        if (row) {
            int index = row->get_index();
                if (row) {
                    Gtk::Box* box = dynamic_cast<Gtk::Box*>(row->get_child()); // Box'ı al
                    if (box) {
                        // Box içindeki tüm child widget'ları al
                        std::vector<Gtk::Widget*> children = box->get_children();
                        
                        // Eğer içinde bir label varsa güncelle
                        for (Gtk::Widget* child : children) {
                            Gtk::Label* label = dynamic_cast<Gtk::Label*>(child);
                            if (label) {
                                std::string header = list->get(index)->header;
                                std::string description = list->get(index)->description;
                                float amount = list->get(index)->amount;
                                bool paid = list->get(index)->paid;
                                std::string date = list->get(index)->date;
                                EditDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Earnings", header, description, amount, paid, date);

                                // Dialog'u göster ve cevabını al
                                int result = dialog.run();

                                // Cevaba göre işlem yap
                                if (result == Gtk::RESPONSE_OK) {
                                    header = dialog.get_header_text();
                                    description = dialog.get_description_text();
                                    amount = dialog.get_amount();
                                    paid = dialog.is_paid();
                                    date = dialog.get_date();
                                    

                                    list->EditNode(index, header, description, amount, paid, date);

                                    is_save = false;window->set_title("Preminiliary Accountant*");

                                    show_all_children();
                                }
                                label->set_text(header);
                                break;  // İlk bulduğumuz label'ı güncelledik, çıkabiliriz.
                            }
                        }
                    }
                }
            delete_button.set_sensitive(false);
        }
    }
    return false;  // Diğer event'ler işlenmeye devam etsin
}


ExpensesBox::ExpensesBox(Gtk::Window* window, bool& is_save, List *list, List *saved_list, Database* db)
: IBox(window, is_save, list, saved_list, db){
    box_title.set_text("Expenses");
}


void ExpensesBox::refresh_list(int month, std::string year){
    clear_listbox();
    Node * temp = list->get();
    while (temp != NULL)
    {
        auto row = Gtk::manage(new Gtk::ListBoxRow());
        auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 5));
        auto label = Gtk::manage(new Gtk::Label(temp->header));
        label->set_size_request(100, 30);
        label->set_margin_top(5);
        label->set_margin_bottom(5);
        label->set_margin_start(10);
        
        box->pack_start(*label);
        row->add(*box);
        list_box.append(*row);
        temp = temp->next;
    }
    box_title.set_text("Total Expenses:" + std::to_string(db->get_total_expenses(month, year)));
    show_all_children();
}


void ExpensesBox::add_saved(){
    std::string header;
    std::string description;
    float amount;
    AddSavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Saved Expense", header, description, amount);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        
        saved_list->AddNode(header, description, amount, false, "");
        is_save = false;window->set_title("Preminiliary Accountant*");

    }
}


void ExpensesBox::delete_saved(){
    DeleteSavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), saved_list);
    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        int index = dialog.get_index();
        saved_list->DeleteNode(index);

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void ExpensesBox::edit_saved(){ 
    std::string header;
    std::string description;
    float amount;
    EditSavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Edit", header, description, amount, saved_list);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();

        int index = dialog.get_index();
        
        saved_list->EditNode(index, header, description, amount, false, "");

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void ExpensesBox::edit(){ 
    std::string header;
    std::string description;
    float amount;
    std::string date;
    bool paid;
    EditDialog_ dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Edit Expenses", header, description, amount, paid, date, list);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        date = dialog.get_date();
        paid = dialog.is_paid();

        int index = dialog.get_index();

        list->EditNode(index, header, description, amount, paid, date);

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


void ExpensesBox::delete_(){
    DeleteDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), list);
    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        int index = dialog.get_index();
        list->DeleteNode(index);

        is_save = false;window->set_title("Preminiliary Accountant*");
    }
}


ExpensesBox::~ExpensesBox(){}


void ExpensesBox::on_add_button_clicked() {
    // Dialog'u başlat
    std::string header;
    std::string description;
    float amount;
    bool paid;
    std::string date;
    AddDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Expenses", header, description, amount, paid, date);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        paid = dialog.is_paid();
        date = dialog.get_date();
        

    
        list->AddNode(header, description, amount, paid, date);

        auto row = Gtk::manage(new Gtk::ListBoxRow());
        auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 5));
        auto label = Gtk::manage(new Gtk::Label(header));
        label->set_size_request(100, 30);
        label->set_margin_top(5);
        label->set_margin_bottom(5);
        label->set_margin_start(10);
        
        box->pack_start(*label);
        row->add(*box);
        list_box.append(*row);
        is_save = false;window->set_title("Preminiliary Accountant*");
        delete_button.set_sensitive(false);
        item_count++;
        show_all_children();
    }
}


void ExpensesBox::on_delete_button_clicked() {        
    Gtk::ListBoxRow* row = list_box.get_selected_row();
    if (row) {
        Gtk::MessageDialog dialog(*this->window, "Are you sure?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
        dialog.set_secondary_text("Are you sure you want to delete the expense?");

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK) { 
            list->DeleteNode(row->get_index());
            list_box.remove(*row);
            delete_button.set_sensitive(false);
            is_save = false;window->set_title("Preminiliary Accountant*");
        
        }
    }
}


void ExpensesBox::on_saved_button_clicked() {
    std::string header = "";
    std::string description = "";
    float amount;
    bool paid;
    std::string date;
    SavedDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Earnings", header, description, amount, paid, saved_list, date);

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        header = dialog.get_header_text();
        description = dialog.get_description_text();
        amount = dialog.get_amount();
        paid = dialog.is_paid();
        date = dialog.get_date();
        

        auto row = Gtk::manage(new Gtk::ListBoxRow());
        auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 5));
        auto label = Gtk::manage(new Gtk::Label(header));
        label->set_size_request(100, 30);
        label->set_margin_top(5);
        label->set_margin_bottom(5);
        label->set_margin_start(10);
        
        box->pack_start(*label);
        row->add(*box);
        list_box.append(*row);
        show_all_children(); 
        is_save = false;window->set_title("Preminiliary Accountant*");
        delete_button.set_sensitive(false);
        item_count++;
    }
}


void ExpensesBox::on_list_box_row_selected(Gtk::ListBoxRow* row) {
    delete_button.set_sensitive(true);
}   


bool ExpensesBox::on_listbox_button_press(GdkEventButton* event) {
    if (event->type == GDK_2BUTTON_PRESS) {  // Çift tıklama olup olmadığını kontrol et
        Gtk::ListBoxRow* row = list_box.get_row_at_y(event->y);
        if (row) {
            int index = row->get_index();
                if (row) {
                    Gtk::Box* box = dynamic_cast<Gtk::Box*>(row->get_child()); // Box'ı al
                    if (box) {
                        // Box içindeki tüm child widget'ları al
                        std::vector<Gtk::Widget*> children = box->get_children();
                        
                        // Eğer içinde bir label varsa güncelle
                        for (Gtk::Widget* child : children) {
                            Gtk::Label* label = dynamic_cast<Gtk::Label*>(child);
                            if (label) {
                                std::string header = list->get(index)->header;
                                std::string description = list->get(index)->description;
                                float amount = list->get(index)->amount;
                                bool paid = list->get(index)->paid;
                                std::string date = list->get(index)->date;

                                EditDialog dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Earnings", header, description, amount, paid, date);

                                // Dialog'u göster ve cevabını al
                                int result = dialog.run();

                                // Cevaba göre işlem yap
                                if (result == Gtk::RESPONSE_OK) {
                                    header = dialog.get_header_text();
                                    description = dialog.get_description_text();
                                    amount = dialog.get_amount();
                                    paid = dialog.is_paid();
                                    date = dialog.get_date();
                                     
                                    list->EditNode(index, header, description, amount, paid, date);

                                    is_save = false;window->set_title("Preminiliary Accountant*");
                                    show_all_children();
                                }
                                label->set_text(header);
                                break;
                            }
                        }
                    }
                }
            delete_button.set_sensitive(false);
        }
    }
    return false;
}

