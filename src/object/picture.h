#ifndef PICTURE_H
#define PICTURE_H

#include <QObject>

#include "object/minmaxshape.h"
#include "object/shape.h"

class Picture : public MinMaxShape, public AngledShape, public MirrorShape {
    Q_OBJECT
public:
    explicit Picture(QObject*, int, int, int, int, int, bool);
    ~Picture();

    QString filename();
    bool embedded();
    virtual void paint(QPainter &painter, QPointF offset);
    bool isIn(int, int);

public slots:
    void setFilename(QString);
    void setEmbedded(bool);

protected:
    QString mFilename;
    bool bEmbedded, bMirrored;
};

#endif // PICTURE_H
