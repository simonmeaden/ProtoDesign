#include "bus.h"

Bus::Bus(QObject *parent, int x1, int y1, int x2, int y2, QColor color, int ripperdir) :
    MinMaxShape(parent, x1, y1, x2, y2),
    ColorShape(color),
    mRipperDir(ripperdir) {

}

Bus::~Bus() {}

bool Bus::isIn(int, int) {
    // TODO
    return false;
}

void Bus::paint(QPainter &painter, QPointF offset) {
    // TODO
}

void Bus::setRipperDir(int ripperdir) {
    mRipperDir = ripperdir;
}

int Bus::ripperDir() {
    return mRipperDir;
}

