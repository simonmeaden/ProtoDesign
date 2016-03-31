#ifndef XYSHAPE_H
#define XYSHAPE_H

#include <QObject>

#include "object/shape.h"

class XYShape : public Shape {
    Q_OBJECT
public:
    explicit XYShape(QObject*, int x, int y);
    ~XYShape();

    virtual void paint(QPainter &, QPointF offset)=0;
    virtual bool isIn(int, int)=0;

    int x();
    int y();

protected:
    int mX, mY;
};

#endif // XYSHAPE_H
