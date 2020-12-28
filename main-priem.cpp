#include "PriemPanel.h"
#include "MessagePopup.h"
#include "Screen.h"

int main() {
    Screen s(80, 25);
    s.setTitle(L"=======> PRIEMNIK");

    bool running = true;

    PriemPanel panel;

    // Drawing
    auto repaint = [&]() {
        s.clear(FG::BLACK | BG::GREY);

        panel.drawOn(s);
        MessagePopup::drawOn(s);

        s.flip();
    };

    // Global exit
    s.handlePriorityKey(VK_F10, 0, [&]() {
        running = false;
    });

    // Global message popup
    MessagePopup::registerKeys(s);

    panel.registerKeys(s);

    // Initial state
    repaint();

    // Main loop
    while (running) {
        s.processEvent();
        repaint();
    }
}
