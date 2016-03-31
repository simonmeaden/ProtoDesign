#include "picture.h"

#include "units.h"

bool Picture::isIn(int, int) {
    // TODO
    return false;
}

void Picture::paint(QPainter &painter, QPointF offset) {
    painter.save();

    // TODO
//    int x = UnitConvert::internalToScreen(mX);
//    int y = UnitConvert::internalToScreen(mY);
//    int radius = UnitConvert::internalToScreen(mRadius);

//    QPen pen = painter.pen();
//    pen.setColor(QColor("black"));
//    painter.setPen(pen);
//    painter.drawArc(x, y, 2 * radius, 2 * radius, mStart, mSweep);

    painter.restore();
}

Picture::Picture(QObject *parent, int x, int y, int width, int height, int angle, bool mirrored) :
    MinMaxShape(parent, x, y, x + width, y + height),
    AngledShape(angle),
    MirrorShape(mirrored) {

}

Picture::~Picture() {}

void Picture::setFilename(QString filename) {
    mFilename = filename;
}

QString Picture::filename() {
    return mFilename;
}

void Picture::setEmbedded(bool embedded) {
    bEmbedded = embedded;
}

bool Picture::embedded() {
    return bEmbedded;
}
