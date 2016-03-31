#ifndef PIN_H
#define PIN_H

#include <QObject>

#include "object/minmaxshape.h"
#include "object/shape.h"

class Pin : public MinMaxShape, public ColorShape {
    Q_OBJECT
public:
    explicit Pin(QObject*, int, int, int, int, QColor, int, int);
    ~Pin();

    bool whichEnd();
    bool pinType();
    virtual void paint(QPainter &painter, QPointF offset);
    bool isIn(int, int);

public slots:
    void setWhichEnd(bool);
    void setPinType(bool);

protected:
    bool bWhichEnd, bPinType;
};

#endif // PIN_H
