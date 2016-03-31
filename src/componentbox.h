#ifndef COMPONENTBOX_H
#define COMPONENTBOX_H

#include <QFrame>

#include "schematic.h"

class ComponentBox : public QFrame {
public:
    ComponentBox(QWidget*);

    virtual void paint(QPainter*);

protected:
    Schematic *pSchematic;
};

#endif // COMPONENTBOX_H
