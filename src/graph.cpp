#include "graph.h"

LineChartArea::LineChartArea(std::vector<double> data)
: data(data) {}


bool LineChartArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    const int width = get_allocated_width();
    const int height = get_allocated_height();

    // Arka planı beyaz yap
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();

    // **Maksimum ve Minimum değerleri bul**
    double min_value = *std::min_element(data.begin(), data.end());
    double max_value = *std::max_element(data.begin(), data.end());

    // **Orta çizgi (0 ekseni)**
    double zero_line = height / 2;

    // **Y ekseni ölçekleme** (Hem negatif hem pozitif değerleri kapsayacak şekilde)
    double scale = (height / 2) / std::max(std::abs(min_value), std::abs(max_value));

    // **X ekseni boyunca noktalar arası mesafe**
    double x_step = static_cast<double>(width) / (data.size() - 1);

    // **Çizgiyi Çiz**
    cr->set_source_rgb(0.0, 0.0, 0.0); // Siyah
    cr->set_line_width(2.0);

    cr->move_to(0, zero_line - data[0] * scale); // İlk noktayı başlat

    for (size_t i = 1; i < data.size(); ++i) {
        double x = i * x_step;
        double y = zero_line - data[i] * scale;
        cr->line_to(x, y);
    }

    cr->stroke(); // **Tüm noktaları bağlayan çizgiyi çiz**

    // **Noktaları Çiz**
    for (size_t i = 0; i < data.size(); ++i) {
        double x = i * x_step;
        double y = zero_line - data[i] * scale;

        // 📌 **RENK AYARI**: Pozitifler mavi, negatifler kırmızı
        if (data[i] > 0)
            cr->set_source_rgb(0.2, 0.6, 1.0); // Mavi
        else if (data[i] < 0)
        {
            cr->set_source_rgb(1.0, 0.2, 0.2); // Kırmızı
        }
        else
        {
            cr->set_source_rgb(0.5, 0.5, 0.5); // Gri
        }

        cr->arc(x, y, 3, 0, 2 * M_PI);
        cr->fill();
    }

    // **Orta çizgiyi çiz (0 ekseni)**
    cr->set_source_rgb(0.0, 0.0, 0.0); // Siyah
    cr->set_line_width(2.0);
    cr->move_to(0, zero_line);
    cr->line_to(width, zero_line);
    cr->stroke();

    return true;
}


LineChartWindow::LineChartWindow(std::vector<double> total_earnings,std::vector<double> total_expenses,std::vector<double> total_){
    set_title("Grafik");
    set_default_size(500, 300);

    try {
        Glib::RefPtr<Gio::Resource> resource = Gio::Resource::create_from_file("resources.gresource");
        resource->register_global();
        Glib::RefPtr<Gdk::Pixbuf> icon = Gdk::Pixbuf::create_from_resource("/com/clytron/accountant_manager/resources/icon.png");
        set_icon(icon);
    } catch (const Glib::Error& e) {
        std::cerr << "Failed to load GResource: " << e.what() << std::endl;
    }
    
    earnings_label.set_text("Earnings");
    expenses_label.set_text("Expenses");
    total_label.set_text("Total");


    earnings = new LineChartArea(total_earnings);
    expenses = new LineChartArea(total_expenses);
    total = new LineChartArea(total_);


    m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box1.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box2.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box3.set_orientation(Gtk::ORIENTATION_VERTICAL);
    
    m_box1.pack_start(earnings_label);
    m_box1.pack_start(*earnings);
    m_box2.pack_start(expenses_label);
    m_box2.pack_start(*expenses);
    m_box3.pack_start(total_label);
    m_box3.pack_start(*total);
    m_box.pack_start(m_box1);
    m_box.pack_start(m_box2);
    m_box.pack_start(m_box3);
    add(m_box);
    
    show_all_children();
}

