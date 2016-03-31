#include "rectangle.h"

#include "units.h"

Rectangle::Rectangle(QObject* parent, int x1, int y1, int x2, int y2, int lineWidth, QColor color) :
    MinMaxShape(parent, x1, y1, x2, y2),
    ColorShape(color),
    LineWidth(lineWidth) {

    regularise(x1, y1, x2, y2);

}

Rectangle::~Rectangle() {}


void Rectangle::paint(QPainter &painter, QPointF offset) {
    painter.save();

    int x1 = UnitConvert::internalToScreen(mX1);
    int y1 = UnitConvert::internalToScreen(mY1);
    int x2 = UnitConvert::internalToScreen(mX2);
    int y2 = UnitConvert::internalToScreen(mY2);

    QPen pen = painter.pen();
    pen.setColor(QColor("black"));
    painter.setPen(pen);
    QRect rect(QPoint(x1, y1), QPoint(x2, y2));
    painter.drawRect(rect);

    painter.restore();
}

bool Rectangle::isIn(int, int) {
    // TODO
    return false;
}
