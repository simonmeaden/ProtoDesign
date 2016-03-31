#include "arc.h"

#include "units.h"


Arc::Arc(QObject *parent, int x, int y, int radius, int width, int startAngle, int deltaAngle, QColor color) :
    XYShape(parent, x, y),
    ColorShape(color),
    LineWidth(width),
    mRadius(radius),
    mStart(startAngle * 16),
    mDelta(deltaAngle * 16) { // angles for arcs are specified in 1/16 degree

}

Arc::~Arc() {}

bool Arc::isIn(int, int) {
    // TODO
    return false;
}

void Arc::paint(QPainter &painter, QPointF offset) {
    painter.save();

    int x = UnitConvert::internalToScreen(mX);
    int y = UnitConvert::internalToScreen(mY);
    int radius = UnitConvert::internalToScreen(mRadius);

    QPen pen = painter.pen();
    pen.setColor(QColor("black"));
    painter.setPen(pen);
    painter.drawArc(x, y, 2 * radius, 2 * radius, mStart, mDelta);

    painter.restore();
}

int Arc::radius() {
    return mRadius;
}

int Arc::startAngle() {
    return mStart;
}

int Arc::deltaAngle() {
    return mDelta;
}
