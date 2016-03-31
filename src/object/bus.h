#ifndef BUS_H
#define BUS_H

#include <QObject>

#include "object/minmaxshape.h"
#include "object/shape.h"

class Bus : public MinMaxShape, public ColorShape {
    Q_OBJECT
public:
    explicit Bus(QObject*, int, int, int, int, QColor, int);
    ~Bus();

    int ripperDir();
    virtual void paint(QPainter &painter, QPointF offset);
    bool isIn(int, int);

public slots:
    void setRipperDir(int);


protected:
    int mRipperDir;
};

#endif // BUS_H
