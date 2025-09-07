#include "login.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.clytron.accountant_manager");
    LoginWindow window;
    return app->run(window);
}
