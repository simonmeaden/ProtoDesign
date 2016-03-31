#ifndef WIRE_H
#define WIRE_H

#include "object/line.h"
class Wire : public Line {
    Q_OBJECT
public:
    explicit Wire(QObject*, int x1, int y1, int x2, int y2);
    ~Wire();

    virtual void paint(QPainter &painter, QPointF offset);
    bool isIn(int, int);

};

#endif // WIRE_H
