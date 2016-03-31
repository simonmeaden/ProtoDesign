#include <QPen>
#include <QBrush>
#include <QColor>
#include <qmath.h>

#include "stripboard.h"
#include "track.h"
#include "values.h"


/*!
 * \class Stripboard
 * \brief Base class of all prototype board classes.
 *
 * The base class of all of the prototype board classes.
 *
 * Subclasses should set the number of rows and columns where the strips of copper
 * run along the rows, the vertical and horizontal pitch and the measurement units.
 * All subclasses should supply a textual description. This is used to add special
 * board types to the menus. For new stripboard types, extend \c Stripboard, for non
 * standard prototype boards where the strips do not run the full length of the board
 * should instead extend \c PrototypeBoard.
 *
 * \code
 * Stripboard_01Pitch_60x35::Stripboard_01Pitch_60x35(QWidget *parent) : Stripboard(parent) {
 *   setDescription(tr("0.1\" pitch, 35 strips x 60 columns Stripboard."));
 *   setSize(35, 60, 0.1, 0.1, INCH);
 * }
 * \endcode
 *
 *
 * \param settings The standard setting values.
 * \param parent The parent class.
 * \sa PrototypeBoard, Stripboard_01Pitch_60x35
 */
Stripboard::Stripboard(Unit unit, int rows, int columns, int vPitch, int hPitch, int holeRadius, Options *options, QWidget *parent) : PrototypeBoard(unit, options, parent),
    mRows(rows),
    mColumns(columns),
    mVPitch(UnitConvert::toInternal(unit, vPitch)),
    mHPitch(UnitConvert::toInternal(unit, hPitch)),
    mHoleRadius(UnitConvert::toInternal(unit, holeRadius)) {
    loadSettings();
}

Stripboard::~Stripboard() {
    qDeleteAll(mCutHoles);
    mCuts.clear();
}


void Stripboard::setSize(Unit units, int rows, int columns, int vPitch, int hPitch, int holeRadius) {
    // TODO make changes if board already partly done.
    mUnits = units;
    mRows = rows;
    mColumns = columns;
    mHPitch = UnitConvert::toInternal(mUnits, hPitch);
    mVPitch = UnitConvert::toInternal(mUnits, vPitch);
    mHoleRadius = UnitConvert::toInternal(mUnits, holeRadius);

    // Create tracks.
    Track *track;
    for (int row = 0; row < mRows; row++) {
        track = new Track(mVPitch, mHPitch, mOptions, this);
        for (int col = 0; col < mColumns; col++) {
            track->addHole(row, col);
        }
        track->link();

        mTracks.append(track);
    }


    mBoardWidth = mOffsetH + ((mColumns - 1) * (mColumnWidth + mGap) - mGap) + (2 * mHBorderWidth);
    mBoardHeight = mOffsetV + ((mRows - 1) * (mRowWidth + mGap) - mGap) + (2 * mVBorderWidth);
}

void Stripboard::splitTrack(int row, int col) {
    Track *track;
    QListIterator<Track*> it(mTracks);
    while (it.hasNext()) {
        track = it.next();

        if (track->contains(row, col)) {

            Hole *hole = track->hole(row, col);
            mCutHoles.append(hole);
            track->remove(hole);

            Hole *nextHole;

            for (int p = Hole::START + 1; p < Hole::END; p++) {
                nextHole = hole->get(static_cast<Hole::Position>(p));
                if (nextHole != NULL) {
                    nextHole->removeConnection(hole); // remove this hole from the opposing connection list
                }
            }

            for (int p = Hole::START + 1; p < Hole::END; p++) {
                nextHole = hole->get(static_cast<Hole::Position>(p));
                if (nextHole != NULL) {
                    Track *t = nextHole->buildTrack(track); // build a new track from the opposing hole.
                    if (t != NULL)
                        mTracks.append(t); // add the new track from the opposing hole.
                }
            }

            mTracks.removeOne(track);
        }
    }
}

void Stripboard::splitTrackBetween(int row1, int col1, int row2, int col2) {

    // if the same hole then use the other form of split
    if (row1 == row2 && col1 == col2)
        splitTrack(row1, col1);
    else {

        Track *track, *t;
        QListIterator<Track*> it(mTracks);
        while (it.hasNext()) {
            t = it.next();
            if (t->contains(row1, col1) && t->contains(row2, col2))
                track = t;
        }

        if (qAbs<int>(row1 - row2) <= 1 && // they must be next to each other
                qAbs<int>(col1 - col2) <= 1 ) {
            // find both holes.
            Hole *hole1 = track->hole(row1, col1);
            Hole *hole2 = track->hole(row2, col2);

            // create the cut between them and store it.
            QPair<Hole*, Hole*> cut;
            cut.first = hole1;
            cut.second = hole2;
            mCuts.append(cut);

            // break the connection between them
            hole1->removeConnection(hole2);
            hole2->removeConnection(hole1);

            // build the two new tracks.
            Track *t1 = hole1->buildTrack(track);
            Track *t2 = hole2->buildTrack(track);

            // remove the old track.
            mTracks.removeOne(track);

            // add the two new ones.
            mTracks.append(t1);
            mTracks.append(t2);
        }
    }
}

void Stripboard::paintEvent(QPaintEvent */*event*/) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    PrototypeBoard::paint(painter);

    QPair<Hole*, Hole*> pair;
    int i;
    for (i = 0; i < mCuts.size(); i++) {
        pair = mCuts.at(i);
        Hole::paintSplitCut(pair, painter);
    }

    for (i = 0; i < mCutHoles.size(); i++) {
        mCutHoles.at(i)->paintSplitHole(painter);
    }
}

void Stripboard::loadSettings() {

    PrototypeBoard::loadSettings();

    mGap = mVPitch * (mOptions->gapPercent() / 100.0); // in pixels

    mRowWidth = mVPitch - mGap; // in pixels
    mColumnWidth = mHPitch; // in pixels

    int pOffsetX = UnitConvert::toInternal(MM, 50.0f);
    int pOffsetY = UnitConvert::toInternal(MM, 50.0f);

    mBoardWidth = mOffsetH + ((mColumns - 1) * (mColumnWidth + mGap) - mGap) + (2 * mHBorderWidth);
    mBoardHeight = mOffsetV + ((mRows - 1) * (mRowWidth + mGap) - mGap) + (2 * mVBorderWidth);
    mInitialPackageOffset = QPointF(mOffsetH + mBoardWidth + pOffsetX, mOffsetV + pOffsetY);

    // Create tracks.
    Track *track;
    Track::loadSettings(mOptions, mGap, mRowWidth, mColumnWidth, mHoleRadius);
    for (int row = 0; row < mRows; row++) {
        track = new Track(mVPitch, mHPitch, mOptions, this);
        for (int col = 0; col < mColumns; col++) {
            track->addHole(row, col);
        }
        track->link();

        mTracks.append(track);
    }
}

void Stripboard::repositionPackages() {
    if (pSchematic) {
//        QList<Package*> packages;
//        Package *package;
//        PositionedPackage *posit;
//        QString name;

//        QListIterator<QString> itName(pSchematic->parts());
//        while(itName.hasNext()) {

//            name = itName.next();
//            packages = pSchematic->packages(name);

//            QListIterator<Package*> itPackage(packages);
//            while (itPackage.hasNext()) {
//                package = itPackage.next();

//                posit = new PositionedPackage(name, package, this);
//                posit->setOffset(mInitialPackageOffset);
//                mPackages.insert(name, posit);
//            }
//        }
    }
}

