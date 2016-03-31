#include "circle.h"

#include "units.h"


Circle::Circle(QObject *parent, int x, int y, int radius, int lineWidth, QColor color) :
    XYShape(parent, x, y),
    ColorShape(color),
    LineWidth(lineWidth),
    mRadius(radius) {
}

Circle::~Circle() {}

bool Circle::isIn(int, int) {
    // TODO
    return false;
}

void Circle::paint(QPainter &painter, QPointF offset) {
    painter.save();

    int x = UnitConvert::internalToScreen(mX);
    int y = UnitConvert::internalToScreen(mY);
    int radius = UnitConvert::internalToScreen(mRadius);

    QPen pen = painter.pen();
    pen.setColor(QColor("black"));
    painter.setPen(pen);
    QPointF centre(x, y);
    painter.drawEllipse(centre, radius, radius);

    painter.restore();
}

int Circle::radius() {
    return mRadius;
}
