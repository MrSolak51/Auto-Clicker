#include "dialog.h"

//IAdderDialog Public start
IAdderDialog::IAdderDialog(Gtk::Window& parent, std::string header, std::string description, float amount)
    : Gtk::Dialog("", parent) {
    // Dialog başlığını ve butonları ayarla
    add_button("OK", Gtk::RESPONSE_OK);
    add_button("Cancel", Gtk::RESPONSE_CANCEL);

    
    
    m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box2.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box3.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    m_box4.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    // Float değerler için Adjustment oluştur
    adjustment = Gtk::Adjustment::create(0.0, 0.0, 1000000000.0, 0.01, 1.0, 0.0);
        
    // SpinButton'u Adjustment ile başlat
    amount_spinbutton.set_adjustment(adjustment);
    amount_spinbutton.set_digits(2);  // Virgülden sonra 2 basamak göster
    amount_spinbutton.set_wrap(true); // Max değeri geçince başa dönsün

    paid_checkbutton.set_active(false);
    m_box.pack_start(m_box2);
    m_box.pack_start(m_box3);
    m_box.pack_start(m_box4);

    m_box2.pack_start(header_entry);
    m_box2.pack_start(description_entry);
    m_box3.pack_start(amount_spinbutton);
    
    get_content_area()->pack_start(m_box);
}


std::string IAdderDialog::get_header_text() const { return header_entry.get_text(); }


std::string IAdderDialog::get_description_text() const { return description_entry.get_text(); }


float IAdderDialog::get_amount() const { return amount_spinbutton.get_value(); }


bool IAdderDialog::is_paid() const { return paid_checkbutton.get_active(); }


std::string IAdderDialog::get_date() const { return date; }


SavedDialog::SavedDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, List * saved_listP, std::string date)
: IAdderDialog(parent, header, description, amount) { 
    title = "Saved ";
    title += str;   

    set_title(title);   

    this->saved_list = saved_listP; 

    Node *temp = this->saved_list->get();
    while (temp != NULL)
    {
        saved_combobox.append(temp->header);
        temp = temp->next;
    }   


    saved_combobox.signal_changed().connect(sigc::mem_fun(*this, &SavedDialog::on_saved_combobox_changed));
    saved_combobox.set_active(0);   

    date_button.signal_clicked().connect(sigc::mem_fun(*this, &SavedDialog::on_date_button_clicked));


    m_box3.pack_start(paid_checkbutton);
    m_box.pack_start(date_button);
    m_box4.pack_start(saved_combobox);  

    show_all_children();
}


void SavedDialog::on_saved_combobox_changed() {
    // Seçili öğeyi al        
    int index = saved_combobox.get_active_row_number();

    header_entry.set_text(saved_list->get(index)->header);
    description_entry.set_text(saved_list->get(index)->description);
    amount_spinbutton.set_value(saved_list->get(index)->amount);
    paid_checkbutton.set_active(saved_list->get(index)->paid ? true : false);

    this->date = saved_list->get(index)->date;
    date_button.set_label(date);
}


void SavedDialog::on_date_button_clicked(){
    DatePicker dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()));


    dialog.set_date(date);
    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        date = dialog.get_date();
        date_button.set_label(date);
    }
}


int SavedDialog::get_index() {return saved_combobox.get_active_row_number();}


AddDialog::AddDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, std::string date)
: IAdderDialog(parent, header, description, amount) { 
    title = "Add ";
    title += str;
    set_title(title);

    date_button.signal_clicked().connect(sigc::mem_fun(*this, &AddDialog::on_date_button_clicked));

    m_box.pack_start(date_button);
    m_box3.pack_start(paid_checkbutton);

    date_button.set_label("Choose Date...");  

    show_all_children();
}
void AddDialog::on_date_button_clicked(){
    DatePicker dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()));

    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        date = dialog.get_date();
        date_button.set_label(date);
    }
}


EditDialog::EditDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, std::string date)
: IAdderDialog(parent, header, description, amount) { 
    title = "Edit ";
    title += str;
    set_title(title);
    header_entry.set_text(header);
    description_entry.set_text(description);
    amount_spinbutton.set_value(amount);
    paid_checkbutton.set_active(paid);

    this->date = date;
    date_button.set_label(date);

    date_button.signal_clicked().connect(sigc::mem_fun(*this, &EditDialog::on_date_button_clicked));
    m_box3.pack_start(paid_checkbutton);
    m_box.pack_start(date_button);  

    show_all_children();
}
void EditDialog::on_date_button_clicked(){
    DatePicker dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()));
    dialog.set_date(date);
    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        date = dialog.get_date();
        date_button.set_label(date);
    }
}


AddSavedDialog::AddSavedDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount)
: IAdderDialog(parent, header, description, amount) { 
    title = "Add Saved";
    title += str;
    set_title(title);
    date_button.set_label("Choose Date...");  

    show_all_children();
}


EditSavedDialog::EditSavedDialog(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, List * saved_listP)
: IAdderDialog(parent, header, description, amount) { 
    title = "Saved ";
    title += str;   

    set_title(title);   

    this->saved_list = saved_listP; 

    Node *temp = this->saved_list->get();
    while (temp != NULL)
    {
        saved_combobox.append(temp->header);
        temp = temp->next;
    }   


    saved_combobox.signal_changed().connect(sigc::mem_fun(*this, &EditSavedDialog::on_saved_combobox_changed));
    saved_combobox.set_active(0);   

    m_box4.pack_start(saved_combobox);  

    show_all_children();
}


void EditSavedDialog::on_saved_combobox_changed() {
    // Seçili öğeyi al        
    int index = saved_combobox.get_active_row_number();
    header_entry.set_text(saved_list->get(index)->header);
    description_entry.set_text(saved_list->get(index)->description);
    amount_spinbutton.set_value(saved_list->get(index)->amount);
}


int EditSavedDialog::get_index(){return saved_combobox.get_active_row_number();}


DeleteSavedDialog::DeleteSavedDialog(Gtk::Window& parent, List * saved_listP){
    add_button("OK", Gtk::RESPONSE_OK);
    add_button("Cancel", Gtk::RESPONSE_CANCEL);
    
    title = "Delete Saved";
    set_title(title);

    this->saved_list = saved_listP; 

    Node *temp = this->saved_list->get();
    while (temp != NULL)
    {
        saved_combobox.append(temp->header);
        temp = temp->next;
    }
    saved_combobox.set_active(0); 
    
    get_content_area()->pack_start(saved_combobox);
    show_all_children();
}


int DeleteSavedDialog::get_index(){return saved_combobox.get_active_row_number();}


EditDialog_::EditDialog_(Gtk::Window& parent, std::string str, std::string header, std::string description, float amount, bool paid, std::string date, List * listP)
: IAdderDialog(parent, header, description, amount) { 
    title = "Saved ";
    title += str;   

    set_title(title);   

    this->list = listP; 

    Node *temp = this->list->get();
    while (temp != NULL)
    {
        saved_combobox.append(temp->header);
        temp = temp->next;
    }   


    saved_combobox.signal_changed().connect(sigc::mem_fun(*this, &EditDialog_::on_saved_combobox_changed));
    saved_combobox.set_active(0);   

    m_box4.pack_start(saved_combobox);  

    show_all_children();
}


void EditDialog_::on_saved_combobox_changed() {
    // Seçili öğeyi al        
    int index = saved_combobox.get_active_row_number();
    header_entry.set_text(list->get(index)->header);
    description_entry.set_text(list->get(index)->description);
    amount_spinbutton.set_value(list->get(index)->amount);
    date_button.set_label(list->get(index)->date);
    paid_checkbutton.set_active(list->get(index)->paid == 1 ? true : false);

    date_button.signal_clicked().connect(sigc::mem_fun(*this, &EditDialog_::on_date_button_clicked));
    date = list->get(index)->date;

    m_box3.pack_start(paid_checkbutton);
    m_box.pack_start(date_button);

}


int EditDialog_::get_index(){return saved_combobox.get_active_row_number();}


void EditDialog_::on_date_button_clicked(){
    DatePicker dialog(*dynamic_cast<Gtk::Window*>(get_toplevel()));
    dialog.set_date(date);
    // Dialog'u göster ve cevabını al
    int result = dialog.run();

    // Cevaba göre işlem yap
    if (result == Gtk::RESPONSE_OK) {
        date = dialog.get_date();
        date_button.set_label(date);
    }
}


DeleteDialog::DeleteDialog(Gtk::Window& parent, List * listP){
    add_button("OK", Gtk::RESPONSE_OK);
    add_button("Cancel", Gtk::RESPONSE_CANCEL);
    
    title = "Delete Saved";
    set_title(title);

    this->list = listP; 

    Node *temp = this->list->get();
    while (temp != NULL)
    {
        saved_combobox.append(temp->header);
        temp = temp->next;
    }
    saved_combobox.set_active(0); 
    
    get_content_area()->pack_start(saved_combobox);
    show_all_children();
}


int DeleteDialog::get_index(){return saved_combobox.get_active_row_number();}

