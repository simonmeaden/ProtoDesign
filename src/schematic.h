#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include <QObject>
#include <QList>

class Grid;
class Shape;

class Schematic {

public:

//    virtual QString version()=0;

    Grid* grid();

    virtual bool read(QString)=0;
    void addShape(Shape*);
    void removeShape(Shape*);

protected:
    QString mVersion;
    QList<Shape*> mShapes;
    Grid *pGrid;

};

#endif // SCHEMATIC_H
