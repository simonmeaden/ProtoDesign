#include "optionswidgets.h"

#include "unitwidgets.h"

#include "global.h"

const QString Options::HORIZONTALBORDER = "horizontalBorder";
const QString Options::VERTICALBORDER = "verticalBorder";
const QString Options::HORIZONTALOFFSET = "horizontalOffset";
const QString Options::VERTICALOFFSET = "verticalOffset";
const QString Options::UNIT = "units";
const QString Options::ROWPERCENT = "rowpercent";
const QString Options::GAPPERCENT = "gappercent";
const QString Options::PIXELSPERUNIT = "pixelsPerUnit";

Options::Options(QObject *parent) :
    QObject(parent) {

}

int Options::rowPercent() {
    pSettings->beginGroup("grid");
    int value = pSettings->value(Options::ROWPERCENT, 90).toInt();
    pSettings->endGroup();
    return value;
}

int Options::gapPercent() {
    pSettings->beginGroup("grid");
    int value = pSettings->value(Options::GAPPERCENT, 10).toInt();
    pSettings->endGroup();
    return value;
}

int Options::verticalBorder() {
    pSettings->beginGroup("grid");
    int value = pSettings->value(Options::VERTICALBORDER, 0).toDouble();
    pSettings->endGroup();
    return value;
}

int Options::pixelsPerUnit() {
    pSettings->beginGroup("grid");
    int value = pSettings->value(Options::PIXELSPERUNIT, 10).toDouble();
    pSettings->endGroup();
    return value;
}

int Options::horizontalBorder() {
    pSettings->beginGroup("grid");
    int value = pSettings->value(Options::HORIZONTALBORDER, 0).toDouble();
    pSettings->endGroup();
    return value;
}

int Options::verticalOffset() {
    pSettings->beginGroup("grid");
    int value = pSettings->value(Options::VERTICALOFFSET, 0).toDouble();
    pSettings->endGroup();
    return value;
}

int Options::horizontalOffset() {
    pSettings->beginGroup("grid");
    int value = pSettings->value(Options::HORIZONTALOFFSET, 0).toDouble();
    pSettings->endGroup();
    return value;
}

Unit Options::units() {
    pSettings->beginGroup("grid");
    Unit value =  UnitConvert::fromString(pSettings->value(Options::UNIT, "mm").toString());
    pSettings->endGroup();
    return value;
}

void Options::setRowPercent(int value)   {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::ROWPERCENT, value);
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setGapPercent(int value)   {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::GAPPERCENT, value);
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setPixelsPerUnit(int value)   {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::PIXELSPERUNIT, value);
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setVerticalBorder(int value)   {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::VERTICALBORDER, value);
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setHorizontalBorder(int value) {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::HORIZONTALBORDER, value);
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setVerticalOffset(int value)   {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::VERTICALOFFSET, value);
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setHorizontalOffset(int value) {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::HORIZONTALOFFSET, value);
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setUnits(Unit value) {
    pSettings->beginGroup("grid");
    pSettings->setValue(Options::UNIT, UnitConvert::toString(value));
    pSettings->endGroup();
    pSettings->sync();
}

void Options::setLayerColor(int layer, QColor value) {
    pSettings->beginGroup("layers");
    pSettings->beginGroup("layer");
    pSettings->beginGroup(QString::number(layer));
    pSettings->beginGroup("color");
    pSettings->setValue("red", value.toRgb().red());
    pSettings->setValue("green", value.toRgb().green());
    pSettings->setValue("blue", value.toRgb().blue());
    pSettings->setValue("alpha", value.toRgb().alpha());
    pSettings->endGroup(); // color
    pSettings->endGroup(); // layer number
    pSettings->endGroup(); // layer
    pSettings->endGroup(); // layers
    pSettings->sync();
}

QColor Options::layerColor(int layer) {
    pSettings->beginGroup("layers");
    pSettings->beginGroup("layer");
    pSettings->beginGroup(QString::number(layer));
    pSettings->beginGroup("color");
    int defRed, defBlue, defGreen, defAlpha;
    switch (layer) {
    case LAYER_LINE:
        defRed = 0x00;
        defBlue = 0x00;
        defGreen = 0x00;
        defAlpha = 0xFF;
        break;
    case LAYER_WIRE:
        defRed = 0xFF;
        defBlue = 0xFF;
        defGreen = 0xFF;
        defAlpha = 0xFF;
        break;
    case LAYER_WIRE_UNCONNECTED:
        defRed = 0xFF;
        defBlue = 0xFF;
        defGreen = 0x00;
        defAlpha = 0xFF;
        break;
    }

    int r = pSettings->value("red",defRed).toInt();
    int g = pSettings->value("green", defGreen).toInt();
    int b = pSettings->value("blue", defBlue).toInt();
    int a = pSettings->value("alpha", defAlpha).toInt();
    QColor c = QColor(r, g, b, a);

    pSettings->endGroup(); // color
    pSettings->endGroup(); // layer number
    pSettings->endGroup(); // layer
    pSettings->endGroup(); // layers

    return c;
}

void Options::setSchemeColor(GSchemColorScheme scheme, GSchemColorMapID mapid, QColor value) {
    pSettings->beginGroup("gschem");
    QString schm;
    switch(scheme) {
    case GSCHEM_LIGHT:
        schm = "light";
        break;
    case GSCHEM_DARK:
        schm = "dark";
        break;
    case GSCHEM_BW:
        schm = "bw";
        break;
    }
    pSettings->beginGroup(schm);
    pSettings->setValue(QString::number(mapid), value.name());
    pSettings->endGroup(); // scheme
    pSettings->endGroup(); // gschem
    pSettings->sync();
}

QColor Options::schemeColor(GSchemColorScheme scheme, GSchemColorMapID mapid) {
    pSettings->beginGroup("gschem");
    QString schm;
    switch(scheme) {
    case GSCHEM_LIGHT:
        schm = "light";
        break;
    case GSCHEM_DARK:
        schm = "dark";
        break;
    case GSCHEM_BW:
        schm = "bw";
        break;
    }
    pSettings->beginGroup(schm);

    QString color;
    switch (scheme) {
    case GSCHEM_LIGHT:
        switch (mapid) {
        case BACKGROUND_COLOR:
            color = "#f0f0f0";
            break;
        case PIN_COLOR:
            color = "#000000";
            break;
        case NET_ENDPOINT_COLOR:
            color = "#FF0000";
            break;
        case GRAPHIC_COLOR:
            color = "#008B00";
            break;
        case NET_COLOR:
            color = "#0000EE";
            break;
        case ATTRIBUTE_COLOR:
            color = "#000000";
            break;
        case LOGIC_BUBBLE_COLOR:
            color = "#008B8B";
            break;
        case DOTS_GRID_COLOR:
            color = "#7F7F7F";
            break;
        case MESH_GRID_MAJOR_COLOR:
            color = "#E1E1E1";
            break;
        case MESH_GRID_MINOR_COLOR:
            color = "#E8E8E8";
            break;
        case DETACHED_ATTRIBUTE_COLOR:
            color = "#FF0000";
            break;
        case TEXT_COLOR:
            color = "#008b00";
            break;
        case BUS_COLOR:
            color = "#00ee00";
            break;
        case SELECT_COLOR:
            color = "#b22222";
            break;
        case BOUNDINGBOX_COLOR:
            color = "#FFA500";
            break;
        case ZOOM_BOX_COLOR:
            color = "#008B8B";
            break;
        case STROKE_COLOR:
            color = "#A020F0";
            break;
        case LOCK_COLOR:
            color = "#666666";
            break;
        case OUTPUT_BACKGROUND_COLOR:
            color = "";
            break;
        case FREESTYLE1_COLOR:
            color = "";
            break;
        case FREESTYLE2_COLOR:
            color = "";
            break;
        case FREESTYLE3_COLOR:
            color = "";
            break;
        case FREESTYLE4_COLOR:
            color = "";
            break;
        case JUNCTION_COLOR:
            color = "#A020F0";
            break;
        }
        break;
    case GSCHEM_DARK:
        switch (mapid) {
        case BACKGROUND_COLOR:
            color = "#000000";
            break;
        case PIN_COLOR:
            color = "#FFFFFF";
            break;
        case NET_ENDPOINT_COLOR:
            color = "#FF0000";
            break;
        case GRAPHIC_COLOR:
            color = "#00FF00";
            break;
        case NET_COLOR:
            color = "#0000FF";
            break;
        case ATTRIBUTE_COLOR:
            color = "#FFFF00";
            break;
        case LOGIC_BUBBLE_COLOR:
            color = "#00FFFF";
            break;
        case DOTS_GRID_COLOR:
            color = "BEBEBE";
            break;
        case DETACHED_ATTRIBUTE_COLOR:
            color = "#FF0000";
            break;
        case TEXT_COLOR:
            color = "#00FF00";
            break;
        case BUS_COLOR:
            color = "#00FF00";
            break;
        case SELECT_COLOR:
            color = "#FFA500";
            break;
        case BOUNDINGBOX_COLOR:
            color = "#FFA500";
            break;
        case ZOOM_BOX_COLOR:
            color = "#00FFFF";
            break;
        case STROKE_COLOR:
           color = "#E5E5E5";
            break;
        case LOCK_COLOR:
            color = "#BEBEBE";
            break;
        case OUTPUT_BACKGROUND_COLOR:
            color = "";
            break;
        case FREESTYLE1_COLOR:
            color = "";
            break;
        case FREESTYLE2_COLOR:
            color = "";
            break;
        case FREESTYLE3_COLOR:
            color = "";
            break;
        case FREESTYLE4_COLOR:
            color = "";
            break;
        case JUNCTION_COLOR:
            color = "#FFFF00";
            break;
        case MESH_GRID_MAJOR_COLOR:
            color = "1E1E1E";
            break;
        case MESH_GRID_MINOR_COLOR:
            color = "171717";
            break;
        }
        break;
    case GSCHEM_BW:
        switch (mapid) {
        case BACKGROUND_COLOR:
            color = "#F0F0F0";
            break;
        case PIN_COLOR:
            color = "#000000";
            break;
        case NET_ENDPOINT_COLOR:
            color = "#000000";
            break;
        case GRAPHIC_COLOR:
            color = "#000000";
            break;
        case NET_COLOR:
           color = "#000000";
            break;
        case ATTRIBUTE_COLOR:
            color = "#000000";
            break;
        case LOGIC_BUBBLE_COLOR:
            color = "#000000";
            break;
        case DOTS_GRID_COLOR:
            color = "#7F7F7F";
            break;
        case DETACHED_ATTRIBUTE_COLOR:
            color = "#FF0000";
            break;
        case TEXT_COLOR:
            color = "#000000";
            break;
        case BUS_COLOR:
            color = "#000000";
            break;
        case SELECT_COLOR:
            color = "#B22222";
            break;
        case BOUNDINGBOX_COLOR:
            color = "#FFA500";
            break;
        case ZOOM_BOX_COLOR:
            color = "#008B8B8B";
            break;
        case STROKE_COLOR:
            color = "#A020F0";
            break;
        case LOCK_COLOR:
            color = "#666666";
            break;
        case OUTPUT_BACKGROUND_COLOR:
            color = "";
            break;
        case FREESTYLE1_COLOR:
            color = "";
            break;
        case FREESTYLE2_COLOR:
            color = "";
            break;
        case FREESTYLE3_COLOR:
            color = "";
            break;
        case FREESTYLE4_COLOR:
            color = "";
            break;
        case JUNCTION_COLOR:
            color = "000000";
            break;
        case MESH_GRID_MAJOR_COLOR:
            color = "E1E1E1";
            break;
        case MESH_GRID_MINOR_COLOR:
            color = "E8E8E8";
            break;
        }
        break;
    }

    color = pSettings->value(QString::number(mapid), color).toString();
    QColor c = QColor(color);

    pSettings->endGroup(); // scheme number
    pSettings->endGroup(); // gschem

    return c;
}

OptionsFrame::OptionsFrame(Options *options, QWidget *parent) :
    QFrame(parent),
    m_options(options) {
    m_parent = qobject_cast<OptionsDialog*>(parent);
    initBuild();
}

void OptionsFrame::initBuild() {
    QGridLayout *layout = new QGridLayout;
    setLayout(layout);

    tabs = new QTabWidget(this);
    layout->addWidget(tabs, 0, 0);

    tabs->addTab(initBoardGeneral(), tr("General Board Options"));
}

QFrame* OptionsFrame::initBoardGeneral() {
    QFrame *boardGeneral  = new QFrame(this);
    QFormLayout *generalLayout = new QFormLayout;
    boardGeneral->setLayout(generalLayout);

    UnitComboBox *unitBox = new UnitComboBox(this);
    generalLayout->addRow(tr("Unit Type :"), unitBox);
    connect(unitBox, SIGNAL(unitHasChanged(Unit)), this, SLOT(setUnit(Unit)));

    UnitSpinBox *horOffsetBox = new UnitSpinBox(this);
    generalLayout->addRow(tr("Board &Horizontal Offset :"), horOffsetBox);
    horOffsetBox->setToolTip(tr("This allows the user to define a left offset in Units from the top corner of the page."));
    connect(horOffsetBox, SIGNAL(valueHasChanged(int)), this, SLOT(setHorizontalOffset(int)));
    connect(unitBox, SIGNAL(unitHasChanged(Unit)), horOffsetBox, SLOT(unitChanged(Unit)));

    UnitSpinBox *vertOffsetBox = new UnitSpinBox(this);
    generalLayout->addRow(tr("Board &Vertical Offset :"), vertOffsetBox);
    vertOffsetBox->setToolTip(tr("This allows the user to define a top offset in Units from the top corner of the page."));
    connect(vertOffsetBox, SIGNAL(valueHasChanged(int)), this, SLOT(setVerticalOffset(int)));
    connect(unitBox, SIGNAL(unitHasChanged(Unit)), vertOffsetBox, SLOT(unitChanged(Unit)));

    UnitSpinBox *horBorderBox = new UnitSpinBox(this);
    generalLayout->addRow(tr("Board Horizontal Border :"), horBorderBox);
    horBorderBox->setToolTip("This specifies a left and right border around the prototype board.");
    connect(horBorderBox, SIGNAL(valueHasChanged(int)), this, SLOT(setHorizontalBorder(int)));
    connect(unitBox, SIGNAL(unitHasChanged(Unit)), horBorderBox, SLOT(unitChanged(Unit)));

    UnitSpinBox *vertBorderBox = new UnitSpinBox(this);
    generalLayout->addRow(tr("Board Vertical Border :"), vertBorderBox);
    vertBorderBox->setToolTip("This specifies a top and bottom border around the prototype board.");
    connect(vertBorderBox, SIGNAL(valueHasChanged(int)), this, SLOT(setVerticalBorder(int)));
    connect(unitBox, SIGNAL(unitHasChanged(Unit)), vertBorderBox, SLOT(unitChanged(Unit)));

    QSpinBox *rowPercentBox  = new QSpinBox(this);
    rowPercentBox->setMinimum(0);
    rowPercentBox->setMaximum(100);
    rowPercentBox->setSingleStep(1);
    generalLayout->addRow(tr("Strip width (% of pitch) :"), rowPercentBox);
    connect(rowPercentBox, SIGNAL(valueChanged(int)), this, SLOT(setRowPercent(int)));

    gapPercentBox  = new QSpinBox(this);
    gapPercentBox->setReadOnly(true);
    generalLayout->addRow(tr("Gap width (% of pitch) :"), gapPercentBox);

    QDoubleSpinBox *pixelsPerUnitBox  = new QDoubleSpinBox(this);
    pixelsPerUnitBox->setMinimum(0);
    pixelsPerUnitBox->setMaximum(1000);
    pixelsPerUnitBox->setSingleStep(1);
    generalLayout->addRow(tr("Pixels per unit :"), pixelsPerUnitBox);
    connect(pixelsPerUnitBox, SIGNAL(valueChanged(int)), this, SLOT(setPixelsPerUnit(int)));

    Unit units = m_options->units();
    int hOffset = UnitConvert::fromInternal(units, m_options->horizontalOffset());
    int vOffset = UnitConvert::fromInternal(units, m_options->verticalOffset());
    int hBorder = UnitConvert::fromInternal(units, m_options->horizontalBorder());
    int vBorder = UnitConvert::fromInternal(units, m_options->verticalBorder());
    int pixUnit = m_options->pixelsPerUnit();
    int gap = m_options->gapPercent();
    int row = m_options->rowPercent();

    unitBox->setCurrentIndex(UnitConvert::indexOf(units));
    horOffsetBox->setValue(hOffset);
    vertOffsetBox->setValue(vOffset);
    horBorderBox->setValue(hBorder);
    vertBorderBox->setValue(vBorder);
    rowPercentBox->setValue(row);
    gapPercentBox->setValue(gap);
    pixelsPerUnitBox->setValue(pixUnit);
    return boardGeneral;
}

void OptionsFrame::setUnit(Unit value) {
    m_parent->m_unit = value;
}

void OptionsFrame::setRowPercent(int value) {
    if (value < 0 || value > 100) return;
    m_parent->m_rowPercent = value;
    m_parent->m_gapPercent = 100 - value;
    gapPercentBox->setValue(m_parent->m_gapPercent);
}

void OptionsFrame::setPixelsPerUnit(int value) {
    m_parent->m_pixelsPerUnit = value;
}

void OptionsFrame::setHorizontalBorder(int value) {
    m_parent->m_hBorder = value;
}

void OptionsFrame::setVerticalBorder(int value) {
    m_parent->m_vBorder = value;
}

void OptionsFrame::setHorizontalOffset(int value) {
    m_parent->m_hOffset = value;
}

void OptionsFrame::setVerticalOffset(int value) {
    m_parent->m_vOffset = value;
}

OptionsDialog::OptionsDialog(Options *options, QWidget *parent) :
    QDialog(parent),
    m_options(options) {
    initBuild();
}


void OptionsDialog::initBuild() {
    QGridLayout *layout = new QGridLayout;
    setLayout(layout);

    frame = new OptionsFrame(m_options, this);
    layout->addWidget(frame, 0, 0, 1, 2);

    QPushButton *acceptBtn = new QPushButton("Accept changes if any");
    layout->addWidget(acceptBtn, 1, 0);
    connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelBtn = new QPushButton("Cancel changes if any");
    layout->addWidget(cancelBtn, 1, 1);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}
