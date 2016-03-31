#include "xyshape.h"

#include "units.h"

XYShape::XYShape(QObject *parent, int x, int y) :
    Shape(parent),
    mX(UnitConvert::toInternal(MIL, x)),
    mY(UnitConvert::toInternal(MIL, y)) {

}

XYShape::~XYShape() {}

int XYShape::x() {return mX; }

int XYShape::y() { return mY; }
