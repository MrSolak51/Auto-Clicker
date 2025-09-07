#include "date_picker.h"

DatePicker::DatePicker(Gtk::Window& parent)
    : Gtk::Dialog("", parent) {
    // Dialog başlığını ve butonları ayarla
    add_button("OK", Gtk::RESPONSE_OK);
    add_button("Cancel", Gtk::RESPONSE_CANCEL);    
    
    get_content_area()->pack_start(date_picker);
    show_all_children();
}


void DatePicker::set_date(std::string date){
    int year, month, day;
    char dash;
    
    std::istringstream ss(date);
    if (ss >> year >> dash >> month >> dash >> day) {
        // Gtk::Calendar'da ay 0-11 aralığında olduğu için düzeltme yapıyoruz.
        date_picker.select_month(month - 1, year);
        date_picker.select_day(day);
    }
}


std::string DatePicker::get_date() const{
    guint year, month, day;
    
    // Takvimden tarihi al
    date_picker.get_date(year, month, day);

    // Ay 0'dan başladığı için +1 ekliyoruz
    std::ostringstream date_stream;
    date_stream << year << "-" << (month + 1) << "-" << day;
    
    return date_stream.str(); 
}

