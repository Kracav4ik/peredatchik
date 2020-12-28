#include "PriemPanel.h"

#include "MessagePopup.h"
#include "utils.h"
#include "priemoperedacha.h"

PriemPanel::PriemPanel()
    : BasePanel(Rect{4, 6, 72, 16})
    , mapping(INVALID_HANDLE_VALUE)
{
}

PriemPanel::~PriemPanel() {
    closeMapping();
}

void PriemPanel::registerKeys(Screen& screen) {
    BasePanel::registerKeys(screen);
    screen.handleKey(VK_F5, 0, [this]() {
        if (hasMapping()) {
            MessagePopup::show({L"Проецируемый файл уже открыт."});
            return;
        }
        openMapping();
        updateLines();
    });
    screen.handleKey(VK_F8, 0, [this]() {
        if (!hasMapping()) {
            MessagePopup::show({L"Проецируемый файл уже закрыт."});
            return;
        }
        closeMapping();
        updateLines();
    });
    screen.handleKey(VK_RETURN, 0, [this]() {
        if (!hasMapping()) {
            return;
        }
        updateLines();
    });
}

void PriemPanel::drawOn(Screen& s) {
    Rect inner = Rect{0, 0, s.w(), s.h()}.withPadding(4, 1);
    s.boundedLine(inner.getLeftTop(), inner.w, L"Чтение данных из проецируемого файла", true);
    s.textOut(inner.moved(0, 1).getLeftTop(), std::wstring(inner.w, L'═'));

    s.paintRect(linesRect, FG::GREY | BG::DARK_GREY);

    if (hasMapping()) {
        s.textOut(inner.moved(0, 3).getLeftTop(), L"<Enter> заново перечитать проецируемый файл");

        BasePanel::drawOn(s);
    }

    s.labelsFill(linesRect.moved(0, linesRect.h + 1).withH(1), {
            L"F5 Открыть проецируемый файл", L"F8 Закрыть проецируемый файл", L"F10 Выход",
    }, FG::BLACK | BG::DARK_CYAN);
}

void PriemPanel::openMapping() {
    mapping = OpenFileMappingW(
            FILE_MAP_ALL_ACCESS,
            false,
            MAPPING_NAME.c_str()
    );
    if (mapping == nullptr) {
        mapping = INVALID_HANDLE_VALUE;
        DWORD error = GetLastError();
        MessagePopup::show({getLastErrorText(error), L"Код ошибки " + std::to_wstring(error)});
        closeMapping();
        return;
    }

    start = (wchar_t*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
    if (start == nullptr) {
        DWORD error = GetLastError();
        MessagePopup::show({getLastErrorText(error), L"Код ошибки " + std::to_wstring(error)});
        closeMapping();
    }
}

void PriemPanel::closeMapping() {
    if (start) {
        UnmapViewOfFile(start);
        start = nullptr;
    }
    if (mapping != INVALID_HANDLE_VALUE) {
        CloseHandle(mapping);
        mapping = INVALID_HANDLE_VALUE;
    }
}
