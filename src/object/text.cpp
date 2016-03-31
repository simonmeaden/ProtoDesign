#include "text.h"

Text::Text(QObject *parent, int x, int y, int angle, QColor color) :
    XYShape(parent, x, y),
    AngledShape(angle),
    ColorShape(color) {

}

Text::~Text() {}

bool Text::isIn(int, int) {
    // TODO
    return false;
}

void Text::paint(QPainter &painter, QPointF offset) {
    // TODO
}

void Text::setPointSize(int size) {
    mPointSize = size;
}

int Text::pointSize() {
    return mPointSize;
}

void Text::setShowName(bool show) {
    bShowName = show;
}

bool Text::showName() {
    return bShowName;
}

void Text::setShowValue(bool show) {
    bShowValue = show;
}

bool Text::showValue() {
    return bShowValue;
}

void Text::setVisible(bool visible) {
    bVisible = visible;
}

bool Text::visible() {
    return bVisible;
}

void Text::setAlignment(Qt::Alignment alignment) {
    mAlignment = alignment;
}

Qt::Alignment Text::alignment() {
    return mAlignment;
}

int Text::attributeCount() {
    return mAttributes.size();
}

int Text::stringCount() {
    return mText.size();
}

void Text::addString(QString text) {
    mText.append(text);
}

QString Text::string(int index) {
    return mText.at(index);
}

void Text::setAttribute(QString attribute, QString value) {
    QPair<QString, QString> a(attribute, value);
    mAttributes.append(a);
}

QString Text::attribute(int index) {
    QPair<QString, QString> a = mAttributes.at(index);
    return a.first;
}

QString Text::value(int index) {
    QPair<QString, QString> a = mAttributes.at(index);
    return a.second;
}
