#include "grid.h"

Grid::Grid() :
    mGridStyle(Lines) {

}

Grid::~Grid() {}

void Grid::setGridUnit(Unit unit) {
    mGridUnit = unit;
}

void Grid::setGridSpacing(qreal spacing) {
    mGridSpacing = spacing;
}

Unit Grid::gridUnit() {
    return mGridUnit;
}

qreal Grid::gridSpacing() {
    return mGridSpacing;
}

void Grid::setAltGridUnit(Unit unit) {
    mAltGridUnit = unit;
}

void Grid::setAltGridSpacing(qreal spacing) {
    mAltGridSpacing = spacing;
}

Unit Grid::altGridUnit() {
    return mAltGridUnit;
}

qreal Grid::altGridSpacing() {
    return mAltGridSpacing;
}

void Grid::setStyle(Style style) {
    mGridStyle = style;
}

Grid::Style Grid::style() {
    return mGridStyle;
}

Grid::Style Grid::fromString(QString style) {
    if (style.toLower() == "lines") return Lines;
    else if (style.toLower() == "dots") return Dots;
    else return None;
}
