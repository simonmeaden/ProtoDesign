#include "schematic.h"
#include "object/grid.h"
#include "object/shape.h"


//Schematic::Schematic() {

//    pGrid->setGridUnit(Unit::MIL); // standard Geda MIL and 100 MIL grid.
//    pGrid->setGridSpacing(100.0);

//}

//Schematic::~Schematic() {}

//QString Schematic::version() {
//    return mVersion;
//}

void Schematic::addShape(Shape *shape) {
    mShapes.append(shape);
}


void Schematic::removeShape(Shape *shape) {
    mShapes.removeOne(shape);
}

Grid* Schematic::grid() {
    return pGrid;
}
