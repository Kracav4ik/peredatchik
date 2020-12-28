#include "BasePanel.h"

#include "utils.h"

BasePanel::BasePanel(Rect linesRect)
    : linesRect(linesRect)
    , start(nullptr)
{
}

void BasePanel::registerKeys(Screen& screen) {
    screen.handleKey(VK_UP, 0, [this]() {
        lines.setScrollOffset(scrollFromDelta(-1));
    });
    screen.handleKey(VK_DOWN, 0, [this]() {
        lines.setScrollOffset(scrollFromDelta(1));
    });
    screen.handleKey(VK_PRIOR, 0, [this]() {
        lines.setScrollOffset(scrollFromDelta(-linesRect.h/2));
    });
    screen.handleKey(VK_NEXT, 0, [this]() {
        lines.setScrollOffset(scrollFromDelta(linesRect.h/2));
    });
    screen.handleKey(VK_HOME, ANY_CTRL_PRESSED, [this]() {
        lines.setScrollOffset(0);
    });
    screen.handleKey(VK_END, ANY_CTRL_PRESSED, [this]() {
        lines.setScrollOffset(scrollFromDelta(lines.getLinesCount()));
    });
}

void BasePanel::drawOn(Screen& s) {
    s.paintRect(linesRect, FG::GREY | BG::DARK_GREY);

    if (hasMapping()) {
        lines.drawOn(s, linesRect.withW(linesRect.w - 1));
        s.pixelMap(linesRect.moved(linesRect.w - 1, 0).withW(1), getScrollbar(), Color::DarkGrey);
    }
}

bool BasePanel::hasMapping() const {
    return start != nullptr;
}

int BasePanel::scrollFromDelta(int delta) const {
    return clamp(0, delta + lines.getScrollOffset(), lines.getLinesCount() - linesRect.h + 1);
}

void BasePanel::updateLines() {
    if (!hasMapping()) {
        lines.setLines({});
        return;
    }
    std::vector<std::wstring> rows;
    auto lineStart = start;
    auto cur = start;
    for (; *cur != L'\0'; ++cur) {
        if (*cur == L'\n') {
            rows.emplace_back(lineStart, cur);
            lineStart = cur + 1;
        }
    }
    lines.setLines(styledText(std::move(rows), FG::GREY | BG::BLACK));
    lines.setScrollOffset(scrollFromDelta(lines.getLinesCount()));
}

std::vector<Color> BasePanel::getScrollbar() const {
    int pixelsCount = 2*linesRect.h;
    std::vector<Color> result(pixelsCount, Color::DarkGrey);
    double totalArea = lines.getLinesCount() + 1;
    int scrollBegin = roundI(pixelsCount * lines.getScrollOffset() / totalArea);
    int scrollSize = clamp(1, roundI(pixelsCount * linesRect.h / totalArea), pixelsCount);
    scrollBegin = std::min(scrollBegin, pixelsCount - scrollSize);
    for (int i = scrollBegin; i < scrollBegin + scrollSize; ++i) {
        result[i] = Color::White;
    }
    return result;
}
