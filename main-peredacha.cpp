#include "PeredachaPanel.h"
#include "MessagePopup.h"
#include "Screen.h"

std::wstring getFullPath(const std::wstring& path) {
    DWORD size = GetFullPathNameW(path.c_str(), 0, nullptr, nullptr);
    std::wstring result(size - 1, L' ');
    GetFullPathNameW(path.c_str(), size, result.data(), nullptr);
    return result;
}

int main() {
    Screen s(80, 25);
    s.setTitle(L"PEREDATCHIK =======>");

    bool running = true;

    std::wstring fname = getFullPath(L"test.txt");
    PeredachaPanel panel(s, fname);

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
