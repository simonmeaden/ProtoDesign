#ifndef GSCHEMSYMBOL_H
#define GSCHEMSYMBOL_H

#include <QObject>
#include <QString>

class Line;
class Text;
class Rectangle;
class Path;
class Circle;
class Arc;
class Picture;
class Net;
class Bus;
class Component;
class Pin;
class Shape;

class GSchemSymbolPrivate;

class GSchemSymbol : public QObject {
    Q_OBJECT
public:
    GSchemSymbol(QObject *);

    bool read(QString);

public slots:
    void addShape(Shape*);
    void removeShape(Shape*);

protected:
    GSchemSymbolPrivate * d_ptr;

private:
    Q_DECLARE_PRIVATE(GSchemSymbol)

};

#endif // GSCHEMSYMBOL_H
