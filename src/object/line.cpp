#include "line.h"

#include "units.h"


Line::Line(QObject* parent, int x1, int y1, int x2, int y2, int width, QColor color) :
    MinMaxShape(parent, x1, y1, x2, y2),
    ColorShape(color),
    LineWidth(width) {

    setCapstyle(Qt::FlatCap);

}

Line::~Line() {}

void Line::paint(QPainter &painter, QPointF offset) {
    painter.save();

    int x1 = UnitConvert::internalToScreen(mX1);
    int y1 = UnitConvert::internalToScreen(mY1);
    int x2 = UnitConvert::internalToScreen(mX2);
    int y2 = UnitConvert::internalToScreen(mY2);

    QPen pen = painter.pen();
    pen.setColor(mColor);
    painter.setPen(pen);
    painter.drawLine(x1, y1, x2, y2);

    painter.restore();
}

bool Line::isIn(int, int) {
    // TODO
    return false;
}

