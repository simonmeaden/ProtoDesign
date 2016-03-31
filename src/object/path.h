#ifndef PATH_H
#define PATH_H


#include <QObject>

#include "object/shape.h"

class PathType : public QObject {
public:
    explicit PathType(QObject *parent);
    ~PathType();

};

class XYTo : public PathType {
public:
    explicit XYTo(QObject *parent, int x, int y);
    ~XYTo();

    int x() { return mX; }
    int y() { return mY; }

protected:
    int mX, mY;
};

class MoveTo : public XYTo {
    Q_OBJECT
public:
    explicit MoveTo(QObject *parent, int x, int y);
    ~MoveTo();

};

class LineTo : public XYTo {
    Q_OBJECT
public:
    explicit LineTo(QObject *parent, int x, int y);
    ~LineTo();
};

class CurveTo : public PathType {
    Q_OBJECT
public:
    explicit CurveTo(QObject *parent, int x1, int y1, int x2, int y2, int x3, int y3);
    ~CurveTo();

    int x1() { return mX1; }
    int y1() { return mY1; }
    int x2() { return mX2; }
    int y2() { return mY2; }
    int x3() { return mX3; }
    int y3() { return mY3; }

protected:
    int mX1, mY1, mX2, mY2, mX3, mY3;
};

class ClosePath : public PathType {
public:
    explicit ClosePath(QObject *parent);
    ~ClosePath();
};


class Path : public Shape, public FilledShape, public ColorShape, public LineWidth {
    Q_OBJECT

public:
    explicit Path(QObject*, QColor, int);
    ~Path();

    virtual void paint(QPainter &painter, QPointF offset);
    bool isIn(int, int);

    QList<PathType*> path();

public slots:
    void addMove(int x, int y);
    void addLine(int x, int y);
    void addCurve(int x1, int y1, int x2, int y2, int x3, int y3);
    void closePath();

protected:
    QList<PathType*> mPath;

};


#endif // PATH_H
