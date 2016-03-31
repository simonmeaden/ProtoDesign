#include "pin.h"

Pin::Pin(QObject *parent, int x1, int y1, int x2, int y2, QColor color, int pinType, int whichEnd) :
    MinMaxShape(parent, x1, y1, x2, y2),
    ColorShape(color) {

    bPinType = pinType == 0 ? true : false; // 0 == NORMAL, 1 == BUSPIN (unused)
    bWhichEnd = whichEnd == 0 ? true : false; // 0 == x1-y1, 1 == x2-y2

}

Pin::~Pin() {}

bool Pin::isIn(int, int) {
    // TODO
    return false;
}

void Pin::paint(QPainter &painter, QPointF offset) {
    // TODO
}

/*! Set which end a net can connect to.
 *
 * If true then the first end (x1-y1) is the connectable end
 * therwise the second end (x2-y2) is connectable.
 */
void Pin::setWhichEnd(bool whichEnd) {
    bWhichEnd = whichEnd;
}

/*! Get which end a net can connect to.
 *
 * If true then the first end (x1-y1) is the connectable end
 * therwise the second end (x2-y2) is connectable.
 */
bool Pin::whichEnd() {
    return bWhichEnd;
}

/*! Set the pin type.
 *
 * If true the pin type is NORMAL, otherwise the pin type is
 * BUSPIN. At this point BUSPIN is unused by GSChem.
 */
void Pin::setPinType(bool pinType) {
    bPinType = pinType;
}

/*! Get the pin type.
 *
 * If true the pin type is NORMAL, otherwise the pin type is
 * BUSPIN. At this point BUSPIN is unused by GSChem.
 */
bool Pin::pinType() {
    return bPinType;
}
