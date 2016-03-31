#include "gschemsymbol.h"
#include "private/gschemsymbol_p.h"

#include "object/shape.h"
#include "object/line.h"
#include "object/text.h"
#include "object/rectangle.h"
#include "object/circle.h"
#include "object/arc.h"
#include "object/picture.h"
#include "object/pin.h"
#include "object/bus.h"
#include "object/component.h"
#include "object/net.h"
#include "object/path.h"

#include "global.h"

GSchemSymbol::GSchemSymbol(QObject *) :
    d_ptr(new GSchemSymbolPrivate(this)) {

}

//GSchemSymbol::GSchemSymbol(GSchemSymbolPrivate &d, QObject *parent=0) :
//    QObject(parent),
//    d_ptr(&d) {

//}

bool GSchemSymbol::read(QString filename) {
    return d_ptr->read(filename);
}

void GSchemSymbol::addShape(Shape *shape) {
    d_ptr->addShape(shape);
}

void GSchemSymbol::removeShape(Shape *shape) {
    d_ptr->removeShape(shape);
}

