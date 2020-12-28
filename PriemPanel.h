#pragma once

#include "BasePanel.h"

#include <windows.h>

class Screen;

class PriemPanel : BasePanel {
public:
    PriemPanel();
    ~PriemPanel();

    void registerKeys(Screen& screen);

    void drawOn(Screen& s);

private:
    void openMapping();
    void closeMapping();

    HANDLE mapping;
};
