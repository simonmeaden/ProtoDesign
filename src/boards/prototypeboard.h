#ifndef PROTOTYPEBOARD_H
#define PROTOTYPEBOARD_H

#include <QFrame>
#include <QMap>
#include <QMultiMap>
#include <QPainter>
#include <QPointF>
#include <QMouseEvent>

#include "schematic.h"
#include "optionswidgets.h"

class QSettings;
class Track;

//class NonPositionedPackage : public QObject {
//    Q_OBJECT
//public:
//    NonPositionedPackage(QString name, Package *package, QObject *parent);

//protected:
//  QString mName;
//  Package *pPackage;

//};

//class PositionedPackage : public NonPositionedPackage {
//  Q_OBJECT
//public:
//  PositionedPackage(QString name, Package *package, QObject *parent);

//  void setOffset(int x, int y);
//  void setOffset(QPointF offset);
//  void setMouseOffset(int x, int y);
//  void setMouseOffset(QPointF offset);
//  void updatePosition();
//  QPointF offset() { return mOffset + mMouseOffset; }


//  bool isIn(int x, int y);
//  Package* package() { return pPackage; }

//protected:
//  QPointF mOffset;
//  QPointF mMouseOffset;

//};


class PrototypeBoard : public QFrame {
    Q_OBJECT
public:

    explicit PrototypeBoard(Unit unit, Options *options, QWidget *parent = 0);
    ~PrototypeBoard();

    QString description() { return mDescription; }
    void setDescription(QString description) { mDescription = description; }
    virtual QSize sizeHint();
    virtual QSize minimumSizeHint() { return sizeHint(); }

signals:

public slots:
    virtual void mousePressEvent(QMouseEvent *evt);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *evt);
    virtual void paint(QPainter &painter);
    virtual void loadSettings();
    virtual void splitTrack(int row, int col)=0;
    virtual void splitTrackBetween(int row1, int col1, int row2, int col2)=0;
    virtual void setSchematic(Schematic *schematic);


protected:
    Options *mOptions;
    Unit mUnits;
    QList<Track* >mTracks;
    int mOffsetV, mOffsetH, mBoardWidth, mBoardHeight;
    int mHBorderWidth, mVBorderWidth;
    int mPixelsPerUnit;
    QPointF mInitialPackageOffset;
    QString mDescription;
    Schematic *pSchematic;
//    PositionedPackage *pSelectedPackage;
//    QMultiMap<QString, PositionedPackage*> mPackages;
    int mStartX, mStartY;

    virtual void repositionPackages()=0;

private:


};

#endif // PROTOTYPEBOARD_H
