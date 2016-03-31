#include "net.h"

Net::Net(QObject *parent, int x1, int y1, int x2, int y2, QColor color) :
    MinMaxShape(parent, x1, y1, x2, y2),
    ColorShape(color) {

}

Net::~Net() {}

bool Net::isIn(int, int) {
    // TODO
    return false;
}

void Net::paint(QPainter &painter, QPointF offset) {
    // TODO
}

