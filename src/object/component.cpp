#include "component.h"

Component::Component(QObject *parent, int x, int y, int angle, bool mirrored) :
    XYShape(parent, x, y),
    AngledShape(angle),
    MirrorShape(mirrored) {

}

Component::~Component() {}

void Component::paint(QPainter &painter, QPointF offset) {
    // TODO
}


void Component::setSelectable(bool selectable) {
    bSelectable = selectable;
}

bool Component::selectable() {
    return bSelectable;
}

void Component::setBaseName(QString baseName) {
    mBaseName = baseName;
}

QString Component::baseName() {
    return mBaseName;
}

bool Component::isIn(int, int) {
    return false;
}

