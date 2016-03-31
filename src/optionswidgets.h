#ifndef OPTIONSFRAME_H
#define OPTIONSFRAME_H

#include <QFrame>
#include <QTabWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QColor>

#include "units.h"

class OptionsDialog;

class Options : public QObject {
    Q_OBJECT
public:
    Options(QObject *parent = 0);

    const static QString HORIZONTALBORDER;
    const static QString VERTICALBORDER;
    const static QString HORIZONTALOFFSET;
    const static QString VERTICALOFFSET;
    const static QString UNIT;
    const static QString ROWPERCENT;
    const static QString GAPPERCENT;
    const static QString PIXELSPERUNIT;

    const static int LAYER_LINE = 9;
    const static int LAYER_WIRE = 10;
    const static int LAYER_WIRE_UNCONNECTED = 11;

    enum GSchemColorScheme {
        GSCHEM_DARK,
        GSCHEM_LIGHT,
        GSCHEM_BW,
    };

    enum GSchemColorMapID {
        BACKGROUND_COLOR = 0,
        PIN_COLOR = 1,
        NET_ENDPOINT_COLOR = 2,
        GRAPHIC_COLOR = 3,
        NET_COLOR = 4,
        ATTRIBUTE_COLOR = 5,
        LOGIC_BUBBLE_COLOR = 6,
        DOTS_GRID_COLOR = 7,
        DETACHED_ATTRIBUTE_COLOR = 8,
        TEXT_COLOR = 9,
        BUS_COLOR = 10,
        SELECT_COLOR = 11,
        BOUNDINGBOX_COLOR = 12,
        ZOOM_BOX_COLOR = 13,
        STROKE_COLOR = 14,
        LOCK_COLOR = 15,
        OUTPUT_BACKGROUND_COLOR = 16,
        FREESTYLE1_COLOR = 17,
        FREESTYLE2_COLOR = 18,
        FREESTYLE3_COLOR = 19,
        FREESTYLE4_COLOR = 20,
        JUNCTION_COLOR = 21,
        MESH_GRID_MAJOR_COLOR = 22,
        MESH_GRID_MINOR_COLOR = 23,
    };

    enum GSchemShow {
       SHOW_NAME_VALUE = 0, // (show both name and value of an attribute)
       SHOW_VALUE = 1,      // (show only the value of an attribute)
       SHOW_NAME = 2,       //
    };

    int rowPercent();
    int gapPercent();
    int verticalBorder();
    int horizontalBorder();
    int verticalOffset();
    int horizontalOffset();
    int pixelsPerUnit();
    Unit units();
    QColor layerColor(int);
    QColor schemeColor(GSchemColorScheme, GSchemColorMapID);

public slots:
    void setVerticalBorder(int);
    void setHorizontalBorder(int);
    void setVerticalOffset(int);
    void setHorizontalOffset(int);
    void setPixelsPerUnit(int);
    void setUnits(Unit);
    void setRowPercent(int);
    void setGapPercent(int);

    void setLayerColor(int, QColor);
    void setSchemeColor(GSchemColorScheme, GSchemColorMapID, QColor);

protected:

};

class OptionsFrame : public QFrame {
    Q_OBJECT
public:
    explicit OptionsFrame(Options *options, QWidget *parent = 0);

signals:
    void rowPercentChanged(int value);
    void gapPercentChanged(int value);

public slots:

protected:
    OptionsDialog *m_parent;
    Options *m_options;
    QTabWidget *tabs;
    QSpinBox *gapPercentBox;

    void initBuild();
    QFrame* initBoardGeneral();

protected slots:
    void setPixelsPerUnit(int value);
    void setUnit(Unit value);
    void setRowPercent(int value);
    void setHorizontalOffset(int value);
    void setVerticalOffset(int value);
    void setHorizontalBorder(int value);
    void setVerticalBorder(int value);

};

class OptionsDialog : public QDialog {
    Q_OBJECT
public:
    OptionsDialog(Options *options, QWidget *parent);

    int pixelsPerUnit() { return m_pixelsPerUnit; }
    int rowPercent() { return m_rowPercent; }
    int gapPercent() { return m_gapPercent; }
    int horizontalOffset() { return m_hOffset; }
    int verticalOffset() { return m_vOffset; }
    int horizontalBorder() { return m_hBorder; }
    int verticalBorder() { return m_vBorder; }
    Unit units() { return m_unit; }

protected:
    Options *m_options;
    OptionsFrame *frame;
    int m_hOffset, m_vOffset, m_hBorder, m_vBorder, m_pixelsPerUnit;
    int m_rowPercent, m_gapPercent;
    Unit m_unit;

    void initBuild();

    friend class OptionsFrame;

};

#endif // OPTIONSFRAME_H
