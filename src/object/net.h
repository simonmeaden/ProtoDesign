#ifndef NET_H
#define NET_H

#include "object/minmaxshape.h"
#include "object/shape.h"

class Net : public MinMaxShape, public ColorShape {
    Q_OBJECT
public:
    explicit Net(QObject*, int, int, int, int, QColor);
    ~Net();

    virtual void paint(QPainter &painter, QPointF offset);
    bool isIn(int, int);

};

#endif // NET_H
