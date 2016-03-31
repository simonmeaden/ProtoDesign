#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>

#include "object/xyshape.h"
#include "object/shape.h"

class Component : public XYShape, public AngledShape, public MirrorShape {
    Q_OBJECT
public:
    Component (QObject*, int, int, int, bool);
    ~Component();

    bool selectable();
    QString baseName();
    virtual void paint(QPainter &painter, QPointF offset);
    virtual bool isIn(int, int);

public slots:
    void setSelectable(bool);
    void setBaseName(QString);

protected:
    bool bSelectable;
    QString mBaseName;


};


#endif // COMPONENT_H
