#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QColor>
#include <QPainter>
#include <QVector>

class FilledShape {
public:
    explicit FilledShape();
    ~FilledShape();

    Qt::BrushStyle fillType();
    void setFillType(Qt::BrushStyle);

protected:
    Qt::BrushStyle mFillType;

};

class AngledShape {
public:
    AngledShape(int);
    ~AngledShape();

    int angle();
    void setAngle(int);

protected:
    int mAngle;
};

class ColorShape {
public:
    ColorShape(QColor);
    ~ColorShape();

    QColor color();
    void setColor(QColor);

    static int fromColor(QColor);
    static QColor toColor(int);

protected:
    QColor mColor;
};

class LineWidth {
public:
    explicit LineWidth(int);
    ~LineWidth();

    int lineWidth();
    void setLineWidth(int);

protected:
    int mLineWidth;

};

class MirrorShape {
public:
    explicit MirrorShape(bool);
    ~MirrorShape();

    bool mirrored();
    void setMirrored(bool);

protected:
    bool bMirrored;

};

class Shape : public QObject {
    Q_OBJECT
public:
    explicit Shape(QObject *parent);
    ~Shape();

    virtual void paint(QPainter &, QPointF offset)=0;
    virtual bool isIn(int, int)=0;
    Qt::PenCapStyle capStyle();
    Qt::PenStyle penStyle();
    int layer();

public slots:
    void setCapstyle(Qt::PenCapStyle);
    void setPenStyle(Qt::PenStyle, QVector<qreal>);
    void setLayer(int layer);
    void addShape(Shape*);
    void removeShape(Shape*);

protected:
    Qt::PenCapStyle mCapStyle;
    Qt::PenStyle mPenStyle;
    QVector<qreal> mDashStyle;
    int mLayer;
    QList<Shape*> mShapes;


};

#endif // SHAPE_H
