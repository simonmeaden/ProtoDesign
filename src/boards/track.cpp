#include <QtMath>

#include "track.h"
#include "units.h"


const QColor Hole::darkkhaki("darkkhaki");
const QColor Hole::white("white");
const QColor Hole::black("black");
const QColor Hole::transparent("transparent");
const QColor Hole::red("red");
const QColor Hole::blue("blue");
const QColor Hole::green("green");

float Track::m_rowWidth = 0.0;
float Track::m_columnWidth = 0.0;
float Track::m_hBorderWidth = 0.0;
float Track::m_vBorderWidth = 0.0;
float Track::m_gap = 0.0;
float Track::m_offsetH = 0.0;
float Track::m_offsetV;
float Track::m_colDiff = 0.0;
float Track::m_rowDiff = 0.0;
float Track::m_holeRadius = 0.0;
float Track::m_pixelsPerUnit = 0.0;
float Track::m_vPitch = 0.0;
float Track::m_hPitch = 0.0;
Options* Track::m_options = NULL;
QPointF Track::centre(0.0f, 0.0f);

Hole::Hole(Track *track, int row, int col, QObject *parent) : m_track(track), m_row(row), m_col(col) {
    m_x = Track::m_offsetH + Track::m_hBorderWidth + (m_col * (Track::m_columnWidth + Track::m_gap));
    m_y = Track::m_offsetV + Track::m_vBorderWidth + (m_row * (Track::m_rowWidth + Track::m_gap));
    m_halfRowWidth = Track::m_rowWidth / 2.0f;
    m_halfColumnWidth = Track::m_columnWidth / 2.0f;
    m_halfHoleRadius = Track::m_holeRadius / 2.0f;
    m_centre.setX(UnitConvert::internalToScreen(m_x + m_halfColumnWidth));
    m_centre.setY(UnitConvert::internalToScreen(m_y + m_halfRowWidth));

    if (Track::m_rowWidth > 0)
        angle = qRadiansToDegrees(qAtan(Track::m_columnWidth / Track::m_rowWidth));

    clear();
}

void Hole::clear() {
    m_connections.clear();
}

bool Hole::has(Position position) {
    Hole *hole = m_connections[position];
    if (hole)
        return true;
    return false;
}

Hole * Hole::get(Position position) {
    Hole *hole = m_connections.value(position, 0x0);
    return hole;
}

/*!
 * \brief Removes the connection at the supplied \c Position from the connection list.
 *
 * \param position the \c Position to remove.
 * \return \c true if a connection was removed, otherwise \c false.
 */
bool Hole::removeConnection(Position position) {
    int count = m_connections.remove(position);
    return (count > 0);
}

/*!
 * \brief Removes the connection \c Hole from connection list.
 *
 * \param hole the \c Hole to remove.
 * \return \c true if a connection was removed, otherwise \c false.
 */
bool Hole::removeConnection(Hole *hole) {
    Position p = m_connections.key(hole);
    int count = m_connections.remove(p);
    return (count > 0);
}

void Hole::addConnection(Hole::Position position, Hole *hole) {
    if (hole != NULL)
        m_connections[position] = hole;
}

Track* Hole::buildTrack(Track *originalTrack) {
    Track *t = new Track(originalTrack->parent());
    Hole *nextHole;

    t->addHole(this);
    originalTrack->remove(this);

    for (int p = Hole::START + 1; p < Hole::END; p++) {
        nextHole = get(static_cast<Hole::Position>(p));

        if (nextHole != NULL) {
            originalTrack->remove(nextHole);
            t->addHole(nextHole);
            nextHole->buildTrack(originalTrack, t);
        }
    }
    return t;
}

void Hole::buildTrack(Track *originalTrack, Track *newTrack) {
    Hole *nextHole;

    for (int p = Hole::START + 1; p < Hole::END; p++) {
        nextHole = get(static_cast<Hole::Position>(p));

        if (nextHole != NULL) {
            if (originalTrack->contains(nextHole)) { // check that this is not already assigned.
                if (!newTrack->contains(nextHole)) {
                    newTrack->addHole(nextHole);
                    originalTrack->remove(nextHole);
                    nextHole->buildTrack(originalTrack, newTrack);
                }
            }
        }
    }
}

Track::Track(QObject *parent) : QObject(parent) {
}

Track::Track(float vPitch, float hPitch, Options *options, QObject *parent) :
    QObject(parent) {
    Track::m_vPitch = vPitch;
    Track::m_hPitch = hPitch;
    Track::m_options = options;
}

Track::~Track() {
    qDeleteAll(m_holes);
}

void Track::remove(Hole *hole) {
    m_holes.removeOne(hole);
}

void Track::loadSettings(Options *options, float gap, float rowWidth, float columnWidth, float holeRadius) {
    m_pixelsPerUnit = options->pixelsPerUnit();
    m_hBorderWidth = options->horizontalBorder(); // in pixels
    m_vBorderWidth =  options->verticalBorder(); // in pixels
    m_offsetH =  options->horizontalOffset(); // in pixels
    m_offsetV =  options->verticalOffset(); // in pixels
    m_gap = gap; // in pixels
    m_holeRadius = holeRadius;


    m_rowWidth = rowWidth; // in pixels
    m_columnWidth = columnWidth; // in pixels

    // used when cutting a gap between two adjacent holes
    m_colDiff = ((m_columnWidth / 2) - m_holeRadius);
    m_rowDiff = ((m_rowWidth / 2) - m_holeRadius);

}

int Track::count() {
    return m_holes.size();
}

bool Track::addHole(int x, int y) {
    Hole *hole = new Hole(this, x, y, parent());
    m_holes.append(hole);
    return true;
}

bool Track::addHole(Hole *hole) {
    m_holes.append(hole);
    return true;
}

bool Track::isValid(Hole *hole) {
    // no existing holes.
    if (m_holes.size() == 0) return true;

    Hole* h;
    int x1 = hole->m_x, x2;
    int y1 = hole->m_y, y2;
    QListIterator<Hole*> it(m_holes);
    //firstly the hole cannot be within the list.
    while (it.hasNext()) {
        h = it.next();
        x2 = h->m_x;
        y2 = h->m_y;

        if ((x1 == x2) && (y1 == y2))
            return false;
    }
    // next it must be next to an existing hole.
    it.toFront();
    while (it.hasNext()) {
        h = it.next();
        x2 = h->m_x;
        y2 = h->m_y;

        if (x1 == x2) {
            if ((y1 == y2 - 1) || (y1 == y2 + 1))
                return  true;
        } else if (y1 == y2) {
            if ((x1 == x2 - 1) || (x1 == x2 + 1))
                return true;
        }
    }
    // not valid.
    return false;
}

bool Track::contains(int row, int col) {
    Hole *hole;
    QListIterator<Hole*> it(m_holes);
    while (it.hasNext()) {
        hole = it.next();
        if ((hole->m_col == col) && (hole->m_row == row))
            return true;
    }
    return false;
}

bool Track::contains(Hole *hole) {
    QListIterator<Hole*> it(m_holes);
    while (it.hasNext())
        if (hole == it.next())
            return true;

    return false;
}

Hole* Track::hole(int row, int col) {
    Hole *hole;
    QListIterator<Hole*> it(m_holes);
    while (it.hasNext()) {
        hole = it.next();
        if ((hole->m_col == col) && (hole->m_row == row))
            return hole;
    }
    return 0x0;
}

///*!
// * \brief Splits the track into two, removing the selected hole from both tracks.
// *
// * This is the normal event where a hole is removed to break the track.
// * The track to the left is returned as a new track. All holes in the new track,
// * including the specified hole  are removed from this track. Hence this track
// * contains the remainder to the right of the split.
// *
// * \param col - the position of the track break.
// * \return The track to the left of the split.
// */
//Track* Track::split(int row, int col) {
//    Track *left = new Track(Track::m_vPitch, Track::m_hPitch, m_options, parent());
//    Hole *hole;
//    QListIterator<Hole*> it(m_holes);
//    while (it.hasNext()) {
//        hole = it.next();

//        if (hole->m_row = row, hole->m_col == col) {
//            remove(hole);



//            return left;
//        } else {
//            remove(hole);
//            left->addHole(hole);
//        }
//    }
//    return NULL;
//}

///*!
// * \brief Splits the track into two, The split is between the two holes.
// *
// * This is the special event where an break is required between two adjacent holes.
// * The track must be carefully cut between the holes. The bool right flag indicates whether
// * the break is to the right or left of the specified hole. If true it is to the right, ie
// * the specified hole is if the returned section. If false the specified hole remains
// * in this section. The track to the left is returned as a new track. All holes in the new track,
// * are removed from this track. Hence this track contains the remainder to the right of the split.
// *
// * \param col - the position of the track break.
// * \param right - If true the split is to the right of the specified hole, otherwise it is to the left.
// * \return The track to the left of the split.
// */
//Track* Track::split(int row, int col, bool right) {
//    Track *left = new Track(parent());
//    Hole *hole;
//    QListIterator<Hole*> it(m_holes);
//    while (it.hasNext()) {
//        hole = it.next();

//        if (hole->m_col == col) {
//            if (right) {
//                remove(hole);
//                left->addHole(hole);
//                left->addSplitCut(hole, NULL);
//            } else {
//                left->addSplitCut(NULL, hole);
//            }
//            left->link();
//            link();
//            return left;
//        } else {
//            remove(hole);
//            left->addHole(hole);
//        }
//    }
//    return NULL;
//}

void Track::link() {
    if (m_holes.size() <= 1) return;

    int i, j;
    Hole *hole1, *hole2;
    // clear all existing links for a clean start.
    for (i == 0; i < (m_holes.size() - 1); i++) {
        m_holes.at(i)->clear();
    }
    for (i = 0; i < m_holes.size() - 1; i++) {
        for (j = i + 1; j < m_holes.size(); j++) {
            hole1 = m_holes.at(i);
            hole2 = m_holes.at(j);
            if (hole1->m_col == hole2->m_col) { // vertically above or below.
                if (hole1->m_row == hole2->m_row - 1) { // hole 2 below
                    hole1->addConnection(Hole::Position::DOWN, hole2);
                    hole2->addConnection(Hole::Position::UP, hole1);
                } else if (hole1->m_row == hole2->m_row + 1) { // hole 2 above
                    hole1->addConnection(Hole::Position::UP, hole2);
                    hole2->addConnection(Hole::Position::DOWN, hole1);
                }
            } else if (hole1->m_row == hole2->m_row) { // side by side
                if (hole1->m_col == hole2->m_col - 1) { // hole 2 to right
                    hole1->addConnection(Hole::Position::RIGHT, hole2);
                    hole2->addConnection(Hole::Position::LEFT, hole1);
                } else if (hole1->m_col == hole2->m_col + 1) { // hole 2 to left
                    hole1->addConnection(Hole::Position::LEFT, hole2);
                    hole2->addConnection(Hole::Position::RIGHT, hole1);
                }
            } else if ((hole1->m_col == hole2->m_col + 1) && (hole1->m_row == hole2->m_row + 1)) {
                hole1->addConnection(Hole::Position::UPLEFT, hole2);
                hole2->addConnection(Hole::Position::DOWNRIGHT, hole1);
            } else if ((hole1->m_col == hole2->m_col - 1) && (hole1->m_row == hole2->m_row + 1)) {
                hole1->addConnection(Hole::Position::UPRIGHT, hole2);
                hole2->addConnection(Hole::Position::DOWN_LEFT, hole1);
            } else if ((hole1->m_col == hole2->m_col + 1) && (hole1->m_row == hole2->m_row - 1)) {
                hole1->addConnection(Hole::Position::DOWN_LEFT, hole2);
                hole2->addConnection(Hole::Position::UPRIGHT, hole1);
            } else if ((hole1->m_col == hole2->m_col - 1) && (hole1->m_row == hole2->m_row - 1)) {
                hole1->addConnection(Hole::Position::DOWNRIGHT, hole2);
                hole2->addConnection(Hole::Position::UPLEFT, hole1);
            }
        }
    }
}

void Track::paint(QPainter &painter) {
    Hole *hole;
    for (int i = 0; i < m_holes.size(); i++) {
        hole = m_holes.at(i);
        hole->paintHole(painter);
    }
}

void Hole::paintHole(QPainter &painter) {
    painter.save();

    painter.setPen(Hole::transparent);
    painter.setBrush(Hole::white);

    painter.translate(
        m_centre.x(),
        m_centre.y()
    );

    // draw circular copper around hole.
    painter.drawEllipse(
        QPointF(0.0f, 0.0f),
        UnitConvert::internalToScreen(m_halfColumnWidth),
        UnitConvert::internalToScreen(m_halfRowWidth)
    );

    QRectF rect(-UnitConvert::internalToScreen(0),
                -UnitConvert::internalToScreen(m_halfRowWidth),
                UnitConvert::internalToScreen(m_halfColumnWidth + Track::m_gap),
                UnitConvert::internalToScreen(Track::m_rowWidth)
               );
    float rotation;
    if (has(RIGHT)) {
        rotation = 0;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }
    if (has(DOWNRIGHT)) {
        rotation = angle;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }
    if (has(DOWN)) {
        rotation = 90.0;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }
    if (has(DOWN_LEFT)) {
        rotation = 90.0 + angle;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }
    if (has(LEFT)) {
        rotation = 180.0;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }
    if (has(UPLEFT)) {
        rotation = 180.0 + angle;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }
    if (has(UP)) {
        rotation = 270;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }
    if (has(UPRIGHT)) {
        rotation = 270.0 + angle;
        painter.save();
        painter.rotate(rotation);
        painter.drawRect(rect);
        painter.restore();
    }

    // draw hole
    painter.setPen(Hole::black);
    painter.setBrush(Hole::black);
    painter.drawEllipse(
        QPointF(0.0f, 0.0f),
        UnitConvert::internalToScreen(m_halfHoleRadius),
        UnitConvert::internalToScreen(m_halfHoleRadius)
    );

    painter.restore();
}

void Hole::paintSplitHole(QPainter &painter) {
    painter.save();

    painter.setPen(Hole::red);
    painter.setBrush(Hole::white);
//    painter.drawLine(UnitConvert::internalToScreen(m_x + m_colDiff),
//                     UnitConvert::internalToScreen(m_y + m_rowDiff),
//                     UnitConvert::internalToScreen(m_x + (m_columnWidth - m_colDiff)),
//                     UnitConvert::internalToScreen(m_y + (m_rowWidth - m_rowDiff)));
//    painter.drawLine(UnitConvert::internalToScreen(m_x + (m_columnWidth - m_colDiff)),
//                     UnitConvert::internalToScreen(m_y + m_rowDiff),
//                     UnitConvert::internalToScreen(m_x + m_colDiff),
//                     UnitConvert::internalToScreen(m_y + (m_rowWidth - m_rowDiff)));

    painter.restore();
}

void Hole::paintSplitCut(QPair<Hole*, Hole*> holes, QPainter &painter) {
    if (holes.first == NULL || holes.second == NULL) return;

    painter.save();

    painter.setPen(Hole::red);
    painter.setBrush(Hole::white);
//    if (holes.first != NULL) {
//        painter.drawLine(UnitConvert::internalToScreen(m_x + m_columnWidth + 1),
//                         UnitConvert::internalToScreen(m_y),
//                         UnitConvert::internalToScreen(m_x + m_columnWidth + 1),
//                         UnitConvert::internalToScreen(m_y + m_rowWidth));
//    } else if (holes.second != NULL) {
//        painter.drawLine(UnitConvert::internalToScreen(m_x - m_gap + 1),
//                         UnitConvert::internalToScreen(m_y),
//                         UnitConvert::internalToScreen(m_x  - m_gap + 1),
//                         UnitConvert::internalToScreen(m_y + m_rowWidth));
//    }

    painter.restore();
}
