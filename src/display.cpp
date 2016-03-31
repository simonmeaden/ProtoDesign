#include "display.h"

Display::Display(QWidget *parent) : QLabel(parent) {
    setFrameStyle(QFrame::StyledPanel);
}

QSize Display::minimumSizeHint() const {
    return QSize(100, 100);
}

QSize Display::sizeHint() const {
    return QSize(400, 200);
}

void Display::paintEvent(QPaintEvent * /* event */) {

}
