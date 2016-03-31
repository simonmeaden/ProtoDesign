#include "eaglecadschematic.h"
#include "object/grid.h"


EagleCadSchematic::EagleCadSchematic(QObject *parent) :
    QObject(parent),
    Schematic() {
}

EagleCadSchematic::~EagleCadSchematic() {
}

bool EagleCadSchematic::read(QString filename) {
    QFile file(filename);
    if (file.exists() && file.open(QFile::ReadWrite | QFile::Text)) {
        QTextStream *is = new QTextStream(&file);
        QString line, type;
        Shape *lastShape = Q_NULLPTR;
        int errorLine, errorColumn;
        QString errorStr, msg;

        QDomDocument *pDocument = new QDomDocument();
        if (!pDocument->setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
            msg = QString("ProtoDesign : Parse error at line %1, column %2:\n%3")
                  .arg(errorLine)
                  .arg(errorColumn)
                  .arg(errorStr);
            qWarning() << msg;
            return false;
        }

        QDomElement root = pDocument->documentElement();
        if (root.tagName() != "eagle") {
            msg = QString("ProtoDesign : The file is not an Eagle Schematic file.");
            qWarning() << msg;

            return false;
        } else if (root.hasAttribute("version")) {
            QStringList version = root.attribute("version").split(".");
            if (version.size() != 3 || version[0].toInt() < 6) {
                msg = QString("ProtoDesign : Only Eagle version 6 or later supported..");
                qWarning() << msg;

                return false;
            } else {
                mVersionMajor = version[0].toInt();
                mVersionMinor = version[1].toInt();
                mVersionBuild = version[2].toInt();
            }
        }

        QDomElement drawing = root.firstChildElement("drawing");
        if (drawing.isNull()) {
            msg = QString("ProtoDesign : Parse error - faulty Eagle Schematic file.");
            qWarning() << msg;

            return false;
        }

        QDomElement schematic = drawing.firstChildElement("schematic");
        if (drawing.isNull()) {
            msg = QString("ProtoDesign : Parse error - faulty Eagle Schematic file.");
            qWarning() << msg;

            return false;
        }

        QDomElement child = schematic.firstChildElement("grid");
        if (!child.isNull()) {
            parseGrid(child);
        }

        // TODO Read all parts
        // TODO Read appropriate DeviceSets/Devices/Symbols into internal library????
        // TO convert to internal types.
        QString name, library, deviceSet, device, value;

        QDomElement parts = schematic.firstChildElement("parts");
        QDomElement part = parts.firstChildElement("part");
        while (!part.isNull()) {
            name = part.attribute("name");
            library = part.attribute("library");
            deviceSet = part.attribute("deviceset");
            device = part.attribute("device");
            value = part.attribute("value");
            // TODO now go find it in A) the internal library.
            // and B) somewhere in this file.

        }


    }
}

void EagleCadSchematic::parseGrid(QDomElement elem) {
    QString attr;

    attr = elem.attribute("distance");
    if (!attr.isEmpty())
        pGrid->setGridSpacing(attr.toDouble());

//    attr = elem.attribute("unitdist");
//    if (!attr.isEmpty())
//        pGrid->setGridUnit(UnitConvert::fromString(attr));

    attr = elem.attribute("unit");
    if (!attr.isEmpty())
        pGrid->setGridUnit(UnitConvert::fromString(attr));

    attr = elem.attribute("style");
    if (!attr.isEmpty())
        pGrid->setStyle(Grid::fromString(attr));

//    attr = elem.attribute("multiple");
//    if (!attr.isEmpty())
//        grid->m_multiple = attr.toInt();

//    attr = elem.attribute("display");
//    if (!attr.isEmpty())
//        grid->m_display = (attr.toLower() == "no" ? false : true);

    attr = elem.attribute("altdistance");
    if (!attr.isEmpty())
        pGrid->setAltGridSpacing(attr.toDouble());

//    attr = elem.attribute("altunitdist");
//    if (!attr.isEmpty())
//        grid->m_altUnitDistance = grid->unitFromString(attr);

    attr = elem.attribute("altunit");
    if (!attr.isEmpty())
        pGrid->setAltGridUnit(UnitConvert::fromString(attr));
}


