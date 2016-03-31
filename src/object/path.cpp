#include "path.h"

#include "units.h"

PathType::PathType(QObject *parent) : QObject(parent) {}

PathType::~PathType() {}

XYTo::XYTo(QObject *parent, int x, int y) :
    PathType(parent),
    mX(x),
    mY(y) {

}

XYTo::~XYTo() {}

MoveTo::MoveTo(QObject *parent, int x, int y) :
    XYTo(parent, x, y) {

}

MoveTo::~MoveTo() {}

LineTo::LineTo(QObject *parent, int x, int y) :
    XYTo(parent, x, y) {

}

LineTo::~LineTo() {}


CurveTo::CurveTo(QObject *parent, int x1, int y1, int x2, int y2, int x3, int y3) :
    PathType(parent),
    mX1(x1),
    mY1(y1),
    mX2(x2),
    mY2(y2),
    mX3(x3),
    mY3(y3) {

}

CurveTo::~CurveTo() {}

ClosePath::ClosePath(QObject *parent) :
    PathType(parent) {

}
ClosePath::~ClosePath() {}


Path::Path(QObject *parent, QColor color, int lineWidth) :
    Shape(parent),
    ColorShape(color),
    LineWidth(lineWidth) {

}

Path::~Path() {}

void Path::addMove(int x, int y) {
    MoveTo *move = new MoveTo(this, x, y);
    mPath.append(move);
}

void Path::addLine(int x, int y) {
    LineTo *line = new LineTo(this, x, y);
    mPath.append(line);
}

void Path::addCurve(int x1, int y1, int x2, int y2, int x3, int y3) {
    CurveTo *curve = new CurveTo(this, x1, y1, x2, y2, x3, y3);
    mPath.append(curve);
}

void Path::closePath() {
    ClosePath *close = new ClosePath(this);
    mPath.append(close);
}

void Path::paint(QPainter &painter, QPointF offset) {
    painter.save();

    // TODO
//    int x1 = UnitConvert::internalToScreen(mX1);
//    int y1 = UnitConvert::internalToScreen(mY1);
//    int x2 = UnitConvert::internalToScreen(mX2);
//    int y2 = UnitConvert::internalToScreen(mY2);

//    QPen pen = painter.pen();
//    pen.setColor(QColor("black"));
//    painter.setPen(pen);
//    QRect rect(QPoint(x1, y1), QPoint(x2, y2));
//    painter.drawRect(rect);

    painter.restore();
}

bool Path::isIn(int, int) {
    // TODO
    return false;
}

QList<PathType*> Path::path() {
    return mPath;
}
