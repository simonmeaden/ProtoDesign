#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>

#include "object/xyshape.h"
#include "object/shape.h"

class Circle : public XYShape, public FilledShape, public ColorShape, public LineWidth {
    Q_OBJECT
public:
    explicit Circle(QObject*, int, int, int, int, QColor);
    ~Circle();

    virtual void paint(QPainter&, QPointF);
    bool isIn(int, int);

    int radius();

protected:
    int mRadius;
};
#endif // CIRCLE_H
