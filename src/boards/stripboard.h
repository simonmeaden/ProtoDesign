#ifndef STRIPBOARD_H
#define STRIPBOARD_H

#include <QMap>
#include <QPainter>
#include <QPair>
#include <QSize>

#include "prototypeboard.h"
#include "track.h"
#include "units.h"


class Stripboard : public PrototypeBoard {
    Q_OBJECT
public:
    explicit Stripboard(Unit unit, int rows, int columns, int vPitch, int hPitch, int holeRadius, Options *options, QWidget *parent=0);
  ~Stripboard();

signals:

public slots:
    void setSize(Unit units, int rows, int columns, int vPitch, int hPitch, int holeRadius);
    virtual void splitTrack(int row, int col);
    virtual void splitTrackBetween(int row1, int col1, int row2, int col2);
    virtual void loadSettings();
    virtual void paintEvent(QPaintEvent *);

protected:
    int mRows, mColumns;
    int mVPitch, mHPitch, mHoleRadius;
    int mRowWidth, mColumnWidth, mGap;
    QList<Hole*> mCutHoles;
    QList<QPair<Hole*, Hole*> > mCuts;

    void paintSplitHole(QPainter &painter);
    void paintSplitCut(QPair<Hole *, Hole *> holes, QPainter &painter);
    virtual void repositionPackages();
};

#endif // STRIPBOARD_H
