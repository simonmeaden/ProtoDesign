#include "shape.h"

Shape::Shape(QObject *parent) : QObject(parent) {

}

Shape::~Shape() {}

void Shape::setLayer(int layer) {
    mLayer = layer;
}

int Shape::layer() {
    return mLayer;
}

void Shape::setCapstyle(Qt::PenCapStyle style) {
    mCapStyle = style;
}

Qt::PenCapStyle Shape::capStyle() {
    return mCapStyle;
}

void Shape::setPenStyle(Qt::PenStyle style, QVector<qreal> dash) {
    mPenStyle = style;
    mDashStyle = dash;
}

Qt::PenStyle Shape::penStyle() {
    return mPenStyle;
}

void Shape::addShape(Shape *shape) {
    mShapes.append(shape);
}

void Shape::removeShape(Shape *shape) {
    mShapes.removeOne(shape);
}

FilledShape::FilledShape() {

}

FilledShape::~FilledShape() {}


void FilledShape::setFillType(Qt::BrushStyle brushStyle) {
    mFillType = brushStyle;
}

Qt::BrushStyle FilledShape::fillType() {
    return mFillType;
}

AngledShape::AngledShape(int angle) :
    mAngle(angle) {

}
AngledShape::~AngledShape() {}

void AngledShape::setAngle(int angle) {
    mAngle = angle;
}

int AngledShape::angle() {
    return mAngle;
}

ColorShape::ColorShape(QColor color) :
    mColor(color) {
    mColor = color;
}

ColorShape::~ColorShape() {}

void ColorShape::setColor(QColor color) {
    mColor = color;
}

QColor ColorShape::color() {
    return mColor;
}

int ColorShape::fromColor(QColor color) {
    int red   = color.red();
    int green = color.green();
    int blue  = color.blue();
    int alpha = color.alpha();
    int c = (alpha << 24) + (blue << 16) + (green << 8) + red;
    return c;
}

QColor ColorShape::toColor(int color) {
    int red   = color && 0xFF;
    int green = (color >> 8) && 0xFF;
    int blue  = (color >> 16) & 0xFF;
    int alpha = (color >> 24) && 0xFF;
    return QColor(red, green, blue, alpha);
}

LineWidth::LineWidth(int lineWidth) :
    mLineWidth(lineWidth) {

}

LineWidth::~LineWidth() {}

void LineWidth::setLineWidth(int lineWidth) {
    mLineWidth = lineWidth;
}

int LineWidth::lineWidth() {
    return mLineWidth;
}

MirrorShape::MirrorShape(bool mirrored) :
    bMirrored(mirrored) {

}

MirrorShape::~MirrorShape() {}

void MirrorShape::setMirrored(bool mirrored) {
    bMirrored = mirrored;
}

bool MirrorShape::mirrored() {
    return bMirrored;
}
