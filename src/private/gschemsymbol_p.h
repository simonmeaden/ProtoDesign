#ifndef GSCHEMSYMBOLPRIVATE_H
#define GSCHEMSYMBOLPRIVATE_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QColor>



#include "global.h"


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

struct GSchemSymbolPrivate {

    Q_DECLARE_PUBLIC(GSchemSymbol)

    GSchemSymbolPrivate(GSchemSymbol *q);
    ~GSchemSymbolPrivate();

    GSchemSymbol *q_ptr;

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

#endif // GSCHEMSYMBOLPRIVATE_H
