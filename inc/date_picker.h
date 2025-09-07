#ifndef DATEPICKER_H

#include <gtkmm.h>
#include <iostream>

class DatePicker : public Gtk::Dialog {
public:
    DatePicker(Gtk::Window& parent);
    void set_date(std::string date);
    std::string get_date() const;
protected:
    Gtk::Calendar date_picker;
private:
};

#endif //DATEPICKER_H