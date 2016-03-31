#include "gschemschematic.h"
#include "private/gschemschematic_p.h"

GSchemSchematic::GSchemSchematic(QObject *parent) :
    QObject(parent),
    d_ptr(new GSchemSchematicPrivate(this)) {

}


GSchemSchematic::~GSchemSchematic() {}

bool GSchemSchematic::read(QString filename) {

    return d_ptr->read(filename);

}

//GSchemSchematic::GSchemSchematic(GSchemSchematicPrivate &d) :
//    GSchemSymbol(d) {}
