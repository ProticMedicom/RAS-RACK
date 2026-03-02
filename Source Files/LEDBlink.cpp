#include <wiringPi.h>
#include <gtkmm.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

// Forward declaration
void LedBlink(class MyWindow* window);

// Atomic flag to control blinking
std::atomic<bool> keepBlinking(false);

class MyWindow : public Gtk::Window {
public:
    MyWindow() {
        set_title("Simple GUI on Raspberry Pi");
        //set_default_size(300, 250);

        fullscreen();  // <-- makes the window fullscreen
        set_default_size(300, 250); // optional fallback size

        vbox.set_orientation(Gtk::ORIENTATION_VERTICAL);
        vbox.set_spacing(10);
        vbox.set_border_width(10);  // GTKmm 3 uses set_border_width

        button.set_label("Start Blinking");

        // <<< Set button size here >>>
        button.set_size_request(300, 100);   // minimum width 300px, height 100px
        button.set_margin_top(20);           // optional extra spacing
        button.set_margin_bottom(20);


        button.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_button_clicked));

        progress.set_fraction(0.0);
        progress.set_text("0%");
        progress.set_show_text(true);

        // GTKmm 3 uses pack_start
        vbox.pack_start(button, Gtk::PACK_SHRINK);
        vbox.pack_start(progress, Gtk::PACK_SHRINK);

        add(vbox);
        show_all_children(); // GTKmm 3 uses show_all_children()
    }

    void update_progress(double fraction) {
        progress.set_fraction(fraction);
        int percent = static_cast<int>(fraction * 100);
        progress.set_text(std::to_string(percent) + "%");
    }

protected:
    void on_button_clicked() {
        if (!keepBlinking) {
            keepBlinking = true;
            ledThread = std::thread(LedBlink, this);
            ledThread.detach();
            button.set_label("Stop Blinking");
        }
        else {
            keepBlinking = false;
            button.set_label("Start Blinking");
        }
    }

private:
    Gtk::Button button;
    Gtk::ProgressBar progress;
    Gtk::Box vbox;
    std::thread ledThread;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.example.simplegui");
    MyWindow window;
    return app->run(window);
}

void LedBlink(MyWindow* window) {
    vector<int> ledPins = { 29, 28, 25, 27, 24, 23, 26, 22, 21, 31, 30, 11, 10, 14, 6, 13, 12, 5, 4, 3, 2, 1, 0, 16, 15, 7, 9, 8 };
    constexpr int blinktime = 100;
    size_t total = ledPins.size();

    if (wiringPiSetup() == -1) {
        cerr << "Failed to init WiringPi" << endl;
        return;
    }

    for (int pin : ledPins)
        pinMode(pin, OUTPUT);

    while (keepBlinking) {
        // Turn LEDs on (progress 0% → 100%)
        for (size_t i = 0; i < total && keepBlinking; ++i) {
            digitalWrite(ledPins[i], HIGH);
            sleep_for(milliseconds(blinktime));

            double fraction = static_cast<double>(i + 1) / total;
            Glib::signal_idle().connect_once([window, fraction]() {
                window->update_progress(fraction);
                });
        }

        // Turn LEDs off (progress 100% → 0%)
        for (size_t i = 0; i < total && keepBlinking; ++i) {
            digitalWrite(ledPins[i], LOW);
            sleep_for(milliseconds(blinktime));

            double fraction = 1.0 - static_cast<double>(i + 1) / total;
            Glib::signal_idle().connect_once([window, fraction]() {
                window->update_progress(fraction);
                });
        }
    }

    for (int pin : ledPins)
        digitalWrite(pin, LOW);

    Glib::signal_idle().connect_once([window]() {
        window->update_progress(0.0);
        });
}