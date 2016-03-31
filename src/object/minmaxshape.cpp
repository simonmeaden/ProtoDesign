#include "minmaxshape.h"

#include "units.h"

MinMaxShape::MinMaxShape(QObject *parent) : Shape(parent) {
    mX1 = std::numeric_limits<int>::max();
    mY1 = std::numeric_limits<int>::max();
    mX2 = -std::numeric_limits<int>::max();
    mY2 = -std::numeric_limits<int>::max();
}

MinMaxShape::MinMaxShape(QObject *parent, int x1, int y1, int x2, int y2) :
    Shape(parent),
    mX1(UnitConvert::toInternal(MIL, x1)),
    mX2(UnitConvert::toInternal(MIL, x2)),
    mY1(UnitConvert::toInternal(MIL, y1)),
    mY2(UnitConvert::toInternal(MIL, y2)) {

}

MinMaxShape::~MinMaxShape() {}

int MinMaxShape::x1() {
    return mX1;
}
int MinMaxShape::x2() {
    return mX2;
}
int MinMaxShape::y1() {
    return mY1;
}
int MinMaxShape::y2() {
    return mY2;
}

void MinMaxShape::regularise(int x1, int y1, int x2, int y2) {
    int m_x1, m_y1, m_x2, m_y2;
    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
    mX1 = m_x1 < mX1 ? m_x1 : mX1;
    mY1 = m_y1 < mY1 ? m_y1 : mY2;
    mX2 = m_x1 > mX2 ? m_x1 : mX1;
    mY2 = m_y1 > mY2 ? m_y1 : mY2;
    mX1 = m_x2 < mX1 ? m_x2 : mX1;
    mY1 = m_y2 < mY1 ? m_y2 : mY2;
    mX2 = m_x2 > mX2 ? m_x2 : mX1;
    mY2 = m_y2 > mY2 ? m_y2 : mY2;
}

bool MinMaxShape::isIn(int, int) {
    return false;
}
