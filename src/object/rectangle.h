#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QObject>

#include "object/minmaxshape.h"
#include "object/shape.h"

class Rectangle : public MinMaxShape, public FilledShape, public ColorShape, public LineWidth {
    Q_OBJECT
public:
    explicit Rectangle(QObject*, int, int, int, int, int, QColor);
    ~Rectangle();

    virtual void paint(QPainter &painter, QPointF offset);
    virtual bool isIn(int, int);

};

#endif // RECTANGLE_H
