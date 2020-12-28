#pragma once

#include "Lines.h"
#include "colors.h"
#include "Screen.h"

class BasePanel {
protected:
    explicit BasePanel(Rect linesRect);

    void registerKeys(Screen& screen);
    void drawOn(Screen& s);

    bool hasMapping() const;
    int scrollFromDelta(int delta) const;
    void updateLines();

    const Rect linesRect;
    wchar_t* start;

private:
    std::vector<Color> getScrollbar() const;

    Lines lines;
};
