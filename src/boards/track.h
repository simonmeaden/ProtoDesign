#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QMap>
#include <QPainter>
#include <QBrush>
#include <QRect>
#include <QPoint>
#include <QSettings>
#include <QPair>

#include "optionswidgets.h"

class Track;

class Hole : public QObject {
  Q_OBJECT
public:
    enum Split {
        HOLE,
      CUT
     };
    enum Position {
      START, // Only used when looping
      RIGHT,
      LEFT,
      UP,
      DOWN,
      DOWN_LEFT,
      DOWNRIGHT,
      UPLEFT,
      UPRIGHT,
      END, // Only used when looping
    };

    explicit Hole(Track *track, int x, int y, QObject *parent = 0);

    Track *m_track;
    QPointF m_centre;
    int m_x, m_y, m_row, m_col;
    float m_halfRowWidth, m_halfColumnWidth, m_halfHoleRadius;
    float angle;

    static const QColor darkkhaki;
    static const QColor white;
    static const QColor black;
    static const QColor transparent;
    static const QColor red;
    static const QColor blue;
    static const QColor green;

    QMap<Position, Hole*> m_connections;

    void paintHole(QPainter &painter);
    void paintSplitHole(QPainter &painter);
    static void paintSplitCut(QPair<Hole *, Hole *> holes, QPainter &painter);
    void clear();
    bool has(Position position);
    Hole* get(Position position);
    bool removeConnection(Position position);
    bool removeConnection(Hole *hole);
    void addConnection(Position position, Hole *hole);
    Track* buildTrack(Track *originalTrack);
    void buildTrack(Track *originalTrack, Track *newTrack);
};


class Track : public QObject {
    Q_OBJECT
public:

    explicit Track(QObject *parent = 0);
    explicit Track(float vPitch, float hPitch, Options *options, QObject *parent = 0);
    ~Track();

    static void loadSettings(Options *options, float gap, float rowWidth, float columnWidth, float holeRadius);
    int count();
    bool addHole(int x, int y);
    bool addHole(Hole *hole);
    bool contains(int row, int col);
    bool contains(Hole* hole);
    Hole* hole(int row, int col);
    void link();
    void remove(Hole* hole);

    void paint(QPainter &painter);

    static float m_rowWidth, m_columnWidth, m_hBorderWidth, m_vBorderWidth;
    static float m_gap, m_offsetH, m_offsetV, m_holeRadius, m_pixelsPerUnit, m_vPitch, m_hPitch;
    static float m_colDiff, m_rowDiff;

signals:

public slots:

protected:
    static Options *m_options;
    QList<Hole*> m_holes;
    static QPointF centre;

    bool isValid(Hole *hole);

};

#endif // TRACK_H
