#include <gtk/gtk.h>
#include <stdio.h>
#include <windows.h>
#include <pthread.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 23

GtkWidget *window;
GtkWidget *scale;
GtkWidget *vbox, *vbox1, *vbox2;
GtkWidget *hbox, *hbox1, *keyboard_layout;
GtkWidget *imageLeft;
GtkWidget *imageRight;
GtkWidget *label, *label1;
GtkWidget *combobox;
GtkWidget *buttonLeft;
GtkWidget *buttonRight;

long cps = 1;
pthread_t click_thread;
volatile bool running = false, keyboard_clicked = false;
int selected_buttonI = -1;
int selected_buttonJ = -1;
const char *selected_key = "A";


typedef struct {
    int I;
    int J;
} ButtonData;

ButtonData *data;
ButtonData *data1;
ButtonData ***datas;

WORD vkCodes[6][23] = {
    {0x1B, 0x00, 0x70, /* F1 */ 0x71,  /* F2 */ 0x72,  /* F3 */ 0x73,  /* F4 */ 0x00, /*  */ 0x74,  /* F5 */ 0x75,  /* F6 */ 0x76,  /* F7 */ 0x77,  /* F8 */ 0x00, /*  */ 0x78,  /* F9 */ 0x79,  /* F10 */ 0x7A,  /* F11 */ 0x7B,  /* F12 */ 0x00, /*  */ 0x00, /*  */ 0x13,  /* Pause */ 0x00, /*  */ 0x00, /*  */ 0x00, /*  */ 0x00 /*  */},
    {0x00, /*  */0x31,  /* 1 */0x32,  /* 2 */0x33,  /* 3 */0x34,  /* 4 */0x35,  /* 5 */0x36,  /* 6 */0x37,  /* 7 */0x38,  /* 8 */0x39,  /* 9 */0x30,  /* 0 */0x00, /*  */0x00, /*  */0x08,  /* Backspace */0x00, /*  */0x00, /*  */0x2D,  /* Insert */0x24,  /* Home */0x21,  /* Page Up */0x00, /*  */0x6F,  /* Numpad / */0x6A,  /* Numpad * */0x6D  /* Numpad - */},
    {0x09,  /* Tab */0x51,  /* Q */0x57,  /* W */0x45,  /* E */0x52,  /* R */0x54,  /* T */0x59,  /* Y */0x55,  /* U */0x49,  /* I */0x4F,  /* O */0x50,  /* P */0x00, /*  */0x00, /*  */0x00, /*  */0x00, /*  */0x00, /*  */0x2E,  /* Delete */0x23,  /* End */0x22,  /* Page Down */0x67,  /* Numpad 7 */0x68,  /* Numpad 8 */0x69,  /* Numpad 9 */0x6B  /* Numpad + */},
    {0x14,  /* Caps Lock */0x41,  /* A */0x53,  /* S */0x44,  /* D */0x46,  /* F */0x47,  /* G */0x48,  /* H */0x4A,  /* J */0x4B,  /* K */0x4C,  /* L */0x00, /*  */0x00, /*  */0x0D,  /* Enter */0x00, /*  */0x00, /*  */0x00, /*  */0x00, /*  */0x00, /*  */0x64,  /* Numpad 4 */0x65,  /* Numpad 5 */0x66,  /* Numpad 6 */0x00 /*  */},
    {0xA0,  /* Sol Shift */0x00, /*  */0x5A,  /* Z */0x58, /* X */0x43,  /* C */0x56,  /* V */0x42,  /* B */0x4E,  /* N */0x4D,  /* M */0x00, /*  */0x00, /*  */0x00, /*  */0x00, /*  */0xA1,  /* Sağ Shift */0x00, /*  */0x00, /*  */0x00, /*  */0x26,  /* Yukarı ok tuşu */0x00, /*  */0x61,  /* Numpad 1 */0x62,  /* Numpad 2 */0x63,  /* Numpad 3 */0x6C  /* Numpad Enter */},
    {0xA2,  /* Sol Ctrl */0x5B,  /* Sol Windows Tuşu */0xA4, /* Sol Alt */0x00, /*  */0x00, /*  */0x00, /*  */0x20,  /* Space */0x00, /*  */0x00, /*  */0x00, /*  */0xA5,  /* Sağ Alt */0x5C,  /* Sağ Windows Tuşu */0x00, /*  */0xA3,  /* Sağ Ctrl */0x00, /*  */0x00, /*  */0x25,  /* Sol ok tuşu */0x28,  /* Aşağı ok tuşu */0x27,  /* Sağ ok tuşu */0x60,  /* Numpad 0 */0x00, /*  */0x6E,  /* Numpad . */0x00 /*  */},
};
char *labels[6][23] = {
    {"Esc", " ", "F1","F2","F3","F4"," ","F5","F6","F7","F8"," ","F9","F10","F11","F12"," ", " ","Pause"," "," "," "," "},
    {" ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", " ", " ", "Backspace"," "," ", "Ins", "Home", "PgUp", "/", "*", "-"},
    {"Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", " ", " ", " "," "," ", "Del", "End", "PgDn", "7", "8", "9", "+"},
    {"Caps Lock", "A", "S", "D", "F", "G", "H", "J", "K", "L", " ", " ", " ", "Enter"," "," ", " ", " ", " ", "4", "5", "6", " "},
    {"Shift", " ", "Z", "X", "C", "V", "B", "N", "M", " ", " ", " ", " ", "Shift"," "," ", " ", "^", " ", "1", "2", "3", "Enter"},
    {"Ctrl", "Win", "Alt", " ",  " ", " ", "Space", " ", " ", " ", "Alt", "Win", " ", "Ctrl"," "," ", "<", "v", ">", "0", " ", ",", " "}
};

void simulate_keypress(WORD vkCode) {
    INPUT input = {0};

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;
    input.ki.dwFlags = 0;
    SendInput(1, &input, sizeof(INPUT));

    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;  
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void leftclick() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void rightclick() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &input, sizeof(INPUT));
}

static void on_scale_value_changed(GtkRange *range) {
    cps = (int) gtk_range_get_value(range);
}

void *simulate_clicks(gpointer user_data) {
    while (running) {
        printf("%d %d", selected_buttonJ, selected_buttonI);
        int interval = 1000 / cps;

        if (selected_buttonI > -3 && selected_buttonJ > -3) {
            if (selected_buttonI == -1 && selected_buttonJ == -1) {
                leftclick();
            } else if (selected_buttonI == -2 && selected_buttonJ == -2) {
                rightclick();
            }
            else {
                simulate_keypress(vkCodes[selected_buttonI][selected_buttonJ]);
            }
        }
        Sleep(interval);
    }
    return NULL;
}

void select_button(GtkToggleButton *button, gpointer user_data) {
    ButtonData *data = (ButtonData *)user_data;
    int I = data->I;
    int J = data->J;
    selected_buttonI = I;
    selected_buttonJ = J;
}
void on_combobox_changed(GtkComboBoxText *widget, gpointer user_data) {
    selected_key = gtk_combo_box_text_get_active_text(widget);
}

HHOOK keyboardHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
        if (pKeyboard->vkCode == selected_key[0]) {
            if (wParam == WM_KEYDOWN && !keyboard_clicked) { 
                if (!running) {
                    running = true;
                    pthread_create(&click_thread, NULL, simulate_clicks, NULL);
                } else {
                    running = false;
                    pthread_cancel(click_thread);
                    pthread_join(click_thread, NULL);
                }
                keyboard_clicked = true;
            } else if(wParam == WM_KEYUP) keyboard_clicked = false;
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void InstallHook() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
}

void UninstallHook() {
    UnhookWindowsHookEx(keyboardHook);
}

void destruct(){
    if (running) {
        g_free(data);
        g_free(data1);
        for (int i = 0; i < ROWS; i++) {
            g_free(datas[i]);
        }
        g_free(datas);
        running = false;
        pthread_cancel(click_thread);
        pthread_join(click_thread, NULL);
    }
    UninstallHook();
}

void add_keybinds(){
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "A");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "B");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "C");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "D");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "E");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "F");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "G");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "H");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "I");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "J");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "K");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "L");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "M");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "N");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "O");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "P");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "Q");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "R");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "S");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "Ş");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "T");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "U");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "V");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "W");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "X");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "Y");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, "Z");
}

void add_signals(){
    g_signal_connect(scale, "value-changed", G_CALLBACK(on_scale_value_changed), NULL);
    g_signal_connect(combobox, "changed", G_CALLBACK(on_combobox_changed), NULL);


    data->I = -1;data->J = -1;
    g_signal_connect(buttonLeft, "toggled", G_CALLBACK(select_button), (gpointer)data);
    data1->I = -2;data1->J = -2;
    g_signal_connect(buttonRight, "toggled", G_CALLBACK(select_button), (gpointer)data1);
}

void add_keyboard_buttons(){
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 22);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
    int label_index = 0;
    for(size_t i = 0; i < 6; i++){
        for (size_t j = 0; j < 23; j++)
        {         
            GtkWidget * button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(buttonLeft), labels[i][j]);

            const gchar *label = gtk_button_get_label(GTK_BUTTON(button));

            
            datas[i][j] = g_malloc(sizeof(ButtonData));
            datas[i][j]->I = (int)i;datas[i][j]->J = (int)j;

            if (g_strcmp0(label, " ") == 0) {
                gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
            } else {
                g_signal_connect(button, "toggled", G_CALLBACK(select_button), (gpointer)datas[i][j]);
            }

            gtk_widget_set_name(button, "custom-radio-button");
            gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(button), j, i, 1, 1);
        }
    }
    gtk_box_pack_start(GTK_BOX(keyboard_layout), grid, FALSE, TRUE, 0);
}

void add_keyboard_widgets(){
    add_keyboard_buttons();
}

void add_widgets(){
    add_keyboard_widgets();

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_container_add(GTK_CONTAINER(vbox), keyboard_layout);
    gtk_container_add(GTK_CONTAINER(vbox), hbox1);
    
    gtk_box_pack_start(GTK_BOX(vbox1), label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox1), label1, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox2), scale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), combobox, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(hbox1), GTK_WIDGET(buttonLeft), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox1), GTK_WIDGET(buttonRight), TRUE, TRUE, 0);
}

void edit_widgets(){
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), 0);
}

void create_widgets(){
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    keyboard_layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    label = gtk_label_new("CPS");
    scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 100, 1);
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    label1 = gtk_label_new("BIND");
    combobox = gtk_combo_box_text_new();
    add_keybinds();

    imageLeft = gtk_image_new_from_file("mouse_left.png");
    buttonLeft =  gtk_radio_button_new_with_label(NULL, "Left");
    gtk_button_set_image(GTK_BUTTON(buttonLeft), imageLeft);
    gtk_widget_set_name(buttonLeft, "custom-radio-button");
    imageRight = gtk_image_new_from_file("mouse_right.png");
    buttonRight =  gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(buttonLeft), "Right");
    gtk_button_set_image(GTK_BUTTON(buttonRight), imageRight);
    gtk_widget_set_name(buttonRight, "custom-radio-button");
    
    add_widgets();
    edit_widgets();
    add_signals();
    gtk_widget_show_all(window);

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "#custom-radio-button {"
        "   border: 2px solid #007bff;"
        "   background-color: transparent;"
        "   color: #007bff;"
        "   padding: 10px;"
        "   border-radius: 5px;"
        "   background-image: url('checkbox_checked.png');"
        "}"
        "#custom-radio-button:disabled {"
        "   border: none;"
        "}"
        "#custom-radio-button:checked {"
        "   background-color: #007bff;"
        "   color: white;"
        "}",
        -1, NULL);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void create_window(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "DYClick");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    GdkPixbuf *icon_pixbuf = gdk_pixbuf_new_from_file("icon.ico", NULL);
    if (icon_pixbuf != NULL) {
        gtk_window_set_icon(GTK_WINDOW(window), icon_pixbuf);
    } else {
        g_print("Failed to load icon file!\n");
    }
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    data = g_malloc(sizeof(ButtonData));
    data1 = g_malloc(sizeof(ButtonData));    
    datas = malloc(ROWS * sizeof(ButtonData *));
    for (int i = 0; i < ROWS; i++) {
        datas[i] = malloc(COLS * sizeof(ButtonData));
    }

    create_widgets();
    InstallHook();
    gtk_main();
    destruct();
}

int main(int argc, char *argv[]) {
    create_window(argc, argv);
    return 0;
}
