#ifndef MINMAXSHAPE_H
#define MINMAXSHAPE_H

#include <QObject>

#include "object/shape.h"

class MinMaxShape : public Shape {
    Q_OBJECT
public:
    MinMaxShape(QObject *);
    MinMaxShape(QObject*, int x1, int y1, int x2, int y2);
    ~MinMaxShape();

    virtual void paint(QPainter &, QPointF offset)=0;
    virtual bool isIn(int, int)=0;

    int x1();
    int x2();
    int y1();
    int y2();

protected:
    int mX1, mX2, mY1, mY2;

    void regularise(int x1, int y1, int x2, int y2);

};

#endif // MINMAXSHAPE_H
