#include "prototypeboard.h"
#include "track.h"
#include <units.h>

/*!
 * \class PrototypeBoard
 * \brief Base class of all prototype board classes.
 *
 * The base class of all of the prototype board classes.
 *
 * Subclasses need to set the individual tracks manually. For stripboard types
 * with nonstandard number of rows or columns or partial boards you should extend
 * \c Stripboard rather than \c PrototypeBoard.
 *
 * \code
 * \endcode
 *
 *
 * \param settings The standard setting values.
 * \param parent The parent class.
 * \sa StripBoard, Stripboard_01Pitch_60x35
 */
PrototypeBoard::PrototypeBoard(Unit unit, Options *options, QWidget *parent) :
    QFrame(parent),
    mOptions(options),
    mUnits(unit),
    mDescription(tr("0.1\" pitch, 35 strips x 60 columns Stripboard.")) {
    pSchematic = NULL;
}

PrototypeBoard::~PrototypeBoard() {
}

QSize PrototypeBoard::sizeHint() {

    QSize hint;
    hint.setWidth(int(mBoardWidth));
    hint.setHeight(int(mBoardHeight));

    return hint;
}

void PrototypeBoard::paint(QPainter &painter) {
    painter.save();

    QPen pen(Hole::darkkhaki);
    pen.setWidth(2);
    painter.setPen(pen);
    QBrush brush(Hole::darkkhaki);
    painter.setBrush(brush);
    QRectF rect(
        UnitConvert::internalToScreen(mOffsetH),
        UnitConvert::internalToScreen(mOffsetV),
        UnitConvert::internalToScreen(mBoardWidth),
        UnitConvert::internalToScreen(mBoardHeight)
    );
    painter.drawRect(rect);

    Track* track;
    for (int i = 0; i < mTracks.size(); i++) {
        track = mTracks.at(i);
        track->paint(painter);
    }

    // TODO draw board packages.
    if (pSchematic) {
//        QList<QString> parts = pSchematic->parts();
//        QList<PositionedPackage*> packages;
//        PositionedPackage *package;
//        QString partName;
//        for (int i = 0; i < parts.size(); i++) {
//            partName = parts.at(i);

//            packages = mPackages.values(partName);

//            for (int j = 0; j < packages.size(); j++) {
//                package = packages.at(j);
//                package->package()->paint(painter, package->offset());
//            }
//        }
    }

    painter.restore();
}

void PrototypeBoard::mousePressEvent(QMouseEvent *evt) {
    int x = evt->x();
    int y = evt->y();

//    PositionedPackage *package;
//    QListIterator<QString> itName(pSchematic->parts());
//    while (itName.hasNext()) {
//        QListIterator<PositionedPackage*> itPackage(mPackages.values(itName.next()));
//        while (itPackage.hasNext()) {
//            package = itPackage.next();
//            if (package->isIn(UnitConvert::screenToInternal(x), UnitConvert::screenToInternal(y))) {
//                pSelectedPackage = package;
//                mStartX = x;
//                mStartY = y;
//            }
//        }
//    }
}

void PrototypeBoard::mouseMoveEvent(QMouseEvent *evt) {
    int x = evt->x();
    int y = evt->y();

//    if (pSelectedPackage) {
//        int dx = x - mStartX;
//        int dy = y - mStartY;
//        pSelectedPackage->setMouseOffset(QPointF(UnitConvert::screenToInternal(dx), UnitConvert::screenToInternal(dy)));
//        update();
//    }
}

void PrototypeBoard::mouseReleaseEvent(QMouseEvent */*evt*/) {
//    if (pSelectedPackage) {
//        pSelectedPackage->updatePosition();
//        pSelectedPackage = 0x0;
//        mStartX = 0.0;
//        mStartY = 0.0;
//        update();
//    }
}

void PrototypeBoard::setSchematic(Schematic *schematic) {
    pSchematic = schematic;
    repositionPackages();
    update();
}

void PrototypeBoard::loadSettings() {

    mPixelsPerUnit = mOptions->pixelsPerUnit();
    mHBorderWidth = mOptions->horizontalBorder(); // in pixels
    mVBorderWidth =  mOptions->verticalBorder(); // in pixels
    mOffsetH =  mOptions->horizontalOffset(); // in pixels
    mOffsetV =  mOptions->verticalOffset(); // in pixels

}

//NonPositionedPackage::NonPositionedPackage(QString name, Package *package, QObject *parent) :
//  QObject(parent), mName(name), pPackage(package) {

//}

//PositionedPackage::PositionedPackage(QString name, Package *package, QObject *parent) :
//  NonPositionedPackage(name, package, parent) {

//}

//void PositionedPackage::setOffset(int x, int y) {
//    mOffset = QPointF(x, y);
//}

//void PositionedPackage::setOffset(QPointF offset) {
//    mOffset = offset;
//}

//void PositionedPackage::setMouseOffset(int x, int y) {
//    mMouseOffset = QPointF(x, y);
//}

//void PositionedPackage::setMouseOffset(QPointF offset) {
//    mMouseOffset = offset;
//}

//void PositionedPackage::updatePosition() {
//    mOffset += mMouseOffset;
//    mMouseOffset.setX(0);
//    mMouseOffset.setY(0);
//}

//bool PositionedPackage::isIn(int x, int y) {
//    MinMaxShape *minmax;
//    Object *object;
//    int minX, minY, maxX, maxY;

//    for (int i = 0; i < pPackage->objects().size(); i++) {

//        object = pPackage->objects().at(i);
//        minmax = dynamic_cast<MinMaxShape*>(object);
//        if (minmax) {
//            minX = mOffset.x() + minmax->x1();
//            minY = mOffset.y() + minmax->y1();
//            maxX = mOffset.x() + minmax->x2();
//            maxY = mOffset.y() + minmax->y2();

//            if (x >= minX && x <= maxX && y <= minY && y <= maxY)
//                return true;
//        }

//    }
//    return false;
//}
