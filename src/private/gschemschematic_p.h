#ifndef GSCHEMSCHEMATICPRIVATE_H
#define GSCHEMSCHEMATICPRIVATE_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QColor>



#include "global.h"

class GSchemSchematic;

class GSchemSymbol;
class Line;
class Rectangle;
class Pin;
class Path;
class Component;
class Net;
class Bus;
class Text;
class Picture;
class Arc;
class Circle;
class Shape;

class GSchemSchematicPrivate {
    Q_DECLARE_PUBLIC(GSchemSchematic)
public:
    GSchemSchematicPrivate(GSchemSchematic *q);
    ~GSchemSchematicPrivate();

    GSchemSchematic *q_ptr;

    bool bCompoundComponent;
    quint8 mFileFormatVersion;

    QString mVersion;
    QList<Shape*> mShapes;

    bool read(QString filename);

    void addShape(Shape *shape);
    void removeShape(Shape *shape);


    Line *parseLine(QStringList parts);
    Text* parseText(QStringList parts, QTextStream *is);
    Rectangle* parseBox(QStringList parts);
    Path* parsePath(QStringList parts, QTextStream *is);
    Circle* parseCircle(QStringList parts);
    Arc* parseArc(QStringList parts);
    Picture* parsePicture(QStringList parts, QTextStream *is);
    Net* parseNet(QStringList parts);
    Bus* parseBus(QStringList parts);
    Pin* parsePin(QStringList parts);
    Component* parseComponent(QStringList parts);
    QPair<Qt::PenStyle, QVector<qreal> > toPenStyle(int style, int length, int gap);
    Qt::PenCapStyle toCapStyle(int style);

};

#endif // GSCHEMSCHEMATICPRIVATE_H
