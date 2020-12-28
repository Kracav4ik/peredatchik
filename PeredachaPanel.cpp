#include "PeredachaPanel.h"

#include "Screen.h"
#include "MessagePopup.h"
#include "utils.h"
#include "priemoperedacha.h"

static const DWORD FILE_SIZE = 256;

PeredachaPanel::PeredachaPanel(Screen& screen, std::wstring filePath)
    : BasePanel(Rect{4, 9, 72, 13})
    , editable(screen.getEditable())
    , filePath(std::move(filePath))
    , file(INVALID_HANDLE_VALUE)
    , mapping(INVALID_HANDLE_VALUE)
    , pos(nullptr)
{
    editable.setEnabledGetter([this]() {
        return hasMapping() && !MessagePopup::isVisible();
    });
    editable.setTextSizeMax(256);
    clearText();
}

PeredachaPanel::~PeredachaPanel() {
    closeMapping();
}

void PeredachaPanel::registerKeys(Screen& screen) {
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
        std::wstring text = editable.getText();
        text += L'\n';
        memcpy(pos, text.c_str(), (text.size() + 1) * sizeof(wchar_t));
        pos += text.size();
        clearText();
        updateLines();
    });
}

void PeredachaPanel::drawOn(Screen& s) {
    Rect inner = Rect{0, 0, s.w(), s.h()}.withPadding(4, 1);
    s.boundedLine(inner.getLeftTop(), inner.w, L"Запись данных в проецируемый файл", true);
    s.textOut(inner.moved(0, 1).getLeftTop(), std::wstring(inner.w, L'═'));

    s.textOut(inner.moved(0, 3).getLeftTop(), L"Файл: " + filePath);

    s.paintRect(linesRect, FG::GREY | BG::DARK_GREY);

    if (hasMapping()) {
        s.textOut(inner.moved(0, 5).getLeftTop(), L"<Enter> дописать строчку в файл:");

        Rect editRect = inner.moved(0, 6).withH(1);
        s.paintRect(editRect, FG::BLACK | BG::DARK_CYAN);
        editable.drawOn(s, editRect.getLeftTop(), hasMapping() && !MessagePopup::isVisible());

        BasePanel::drawOn(s);
    }

    s.labelsFill(linesRect.moved(0, linesRect.h + 1).withH(1), {
        L"F5 Создать проецируемый файл", L"F8 Закрыть проецируемый файл", L"F10 Выход",
    }, FG::BLACK | BG::DARK_CYAN);
}

void PeredachaPanel::openMapping() {
    file = CreateFileW(
            filePath.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    );
    if (file == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        std::wstring errorText = getLastErrorText(error);
        if (error == ERROR_SHARING_VIOLATION) {  // too long error message
            size_t comma = errorText.find(L',');
            MessagePopup::show({errorText.substr(0, comma + 1), errorText.substr(comma + 2), L"Код ошибки " + std::to_wstring(error)});
        } else {
            MessagePopup::show({errorText, L"Код ошибки " + std::to_wstring(error)});
        }
        return;
    }
    mapping = CreateFileMappingW(
            file,
            nullptr,
            PAGE_READWRITE,
            0,
            FILE_SIZE,
            MAPPING_NAME.c_str()
    );
    if (mapping == nullptr) {
        mapping = INVALID_HANDLE_VALUE;
        DWORD error = GetLastError();
        MessagePopup::show({getLastErrorText(error), L"Код ошибки " + std::to_wstring(error)});
        closeMapping();
        return;
    }

    start = (wchar_t*)MapViewOfFile(mapping, FILE_MAP_WRITE, 0, 0, 0);
    if (start == nullptr) {
        DWORD error = GetLastError();
        MessagePopup::show({getLastErrorText(error), L"Код ошибки " + std::to_wstring(error)});
        closeMapping();
        return;
    }
    pos = start;
}

void PeredachaPanel::closeMapping() {
    if (start) {
        UnmapViewOfFile(start);
        start = nullptr;
        pos = nullptr;
    }
    if (mapping != INVALID_HANDLE_VALUE) {
        CloseHandle(mapping);
        mapping = INVALID_HANDLE_VALUE;
    }
    if (file != INVALID_HANDLE_VALUE) {
        CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
    }
}

void PeredachaPanel::clearText() {
    editable.setText(L"", 72);
}
