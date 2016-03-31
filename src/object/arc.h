#ifndef ARC_H
#define ARC_H

#include <QObject>

#include "object/xyshape.h"
#include "object/shape.h"

class Arc : public XYShape, public ColorShape, public LineWidth {
    Q_OBJECT
public:
    explicit Arc(QObject*, int, int, int, int, int, int, QColor);
    ~Arc();

    virtual void paint(QPainter&, QPointF);
    virtual bool isIn(int, int);

    int radius();
    int startAngle();
    int deltaAngle();

protected:
    int mRadius, mStart, mDelta;
};

#endif // ARC_H
