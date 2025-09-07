#ifndef GRAPH_H

#include <gtkmm.h>
#include <cairomm/context.h>
#include <vector>
#include <random>
#include <iostream>
#include "list.h"

class LineChartArea : public Gtk::DrawingArea {
private:
    std::vector<double> data;
public:
    LineChartArea(std::vector<double> data);
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

class LineChartWindow : public Gtk::Window {
public:
    LineChartWindow(std::vector<double> total_earnings,std::vector<double> total_expenses,std::vector<double> total);
private:
    LineChartArea *earnings;
    LineChartArea *expenses;
    LineChartArea *total;

    Gtk::Box m_box, m_box1, m_box2, m_box3;

    Gtk::Label earnings_label, expenses_label, total_label;
};

#endif //GRAPH_H