#pragma once

#include "BasePanel.h"

#include <windows.h>
#include <string>

class Screen;
class EditableText;

class PeredachaPanel : BasePanel {
public:
    PeredachaPanel(Screen& screen, std::wstring filePath);
    ~PeredachaPanel();

    void registerKeys(Screen& screen);

    void drawOn(Screen& s);

private:
    void openMapping();
    void closeMapping();
    void clearText();

    EditableText& editable;
    std::wstring filePath;
    HANDLE file;
    HANDLE mapping;
    wchar_t* pos;
};
