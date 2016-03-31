#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QList>
#include <QMultiMap>
#include <QPainter>
#include <QRect>

#include <limits>

#include "object/shape.h"
#include "units.h"

class Grid : public QObject {
    Q_OBJECT
public:
    enum Style { Lines, Dots, None };
    explicit Grid();
    ~Grid();

    void setGridUnit(Unit unit);
    void setAltGridUnit(Unit unit);
    void setGridSpacing(qreal spacing);
    void setAltGridSpacing(qreal spacing);
    Unit gridUnit();
    Unit altGridUnit();
    qreal gridSpacing();
    qreal altGridSpacing();
    void setStyle(Style style);
    Style style();

    static Style fromString(QString);

protected:
    qreal mGridSpacing, mAltGridSpacing;
    Unit mGridUnit, mAltGridUnit;
    Style mGridStyle;


};;

#endif // GRID_H
