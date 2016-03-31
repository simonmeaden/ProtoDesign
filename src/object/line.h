#ifndef LINE_H
#define LINE_H

#include <QObject>

#include "minmaxshape.h"
#include "shape.h"

class Line : public MinMaxShape, public ColorShape, public LineWidth {
    Q_OBJECT
public:
    explicit Line(QObject*, int, int, int, int, int = 1, QColor=QColor());
    ~Line();

    virtual void paint(QPainter &painter, QPointF offset);
    virtual bool isIn(int, int);

protected:
    int mWidth;
};

#endif // LINE_H
