#include "gschemschematic_p.h"
#include "gschemschematic.h"

#include "object/line.h"
#include "object/text.h"
#include "object/rectangle.h"
#include "object/path.h"
#include "object/circle.h"
#include "object/arc.h"
#include "object/picture.h"
#include "object/net.h"
#include "object/bus.h"
#include "object/component.h"
#include "object/pin.h"
#include "object/shape.h"

GSchemSchematicPrivate::GSchemSchematicPrivate(GSchemSchematic *q) :
    q_ptr(q),
    bCompoundComponent(false) {

}

GSchemSchematicPrivate::~GSchemSchematicPrivate() {}


bool GSchemSchematicPrivate::read(QString filename) {

    QFile file(filename);
    if (file.exists() && file.open(QFile::ReadWrite | QFile::Text)) {
        QTextStream *is = new QTextStream(&file);
        QString line, type;
        QStringList parts;
        Shape *lastShape = Q_NULLPTR;

        while (!is->atEnd()) {
            line = is->readLine();
            parts = line.split(" ", QString::SkipEmptyParts);
            type = parts.at(0);

            if (type == "v") { // version string
                mVersion = parts.at(1);
                mFileFormatVersion = parts.at(2).toInt();
                continue;
            } else if (type == "L") { // line
                Line *l = parseLine(parts);
                if (bCompoundComponent) {
                   lastShape->addShape(l); // is a compond component add to component
                } else {
                    addShape(l); // otherwise add to schematic
                    lastShape = l; // set this component as possible compound component.
                }
            } else if (type == "T") { // text
                Text *t = parseText(parts, is);
                if (bCompoundComponent) {
                    lastShape->addShape(t);
                } else {
                    addShape(t);
                    lastShape = t;
                }
            } else if (type == "B") { // box
                Rectangle *r = parseBox(parts);
                if (bCompoundComponent) {
                    lastShape->addShape(r);
                } else {
                    addShape(r);
                    lastShape = r;
                }
             } else if (type == "V") { // circle
                Circle *c = parseCircle(parts);
                if (bCompoundComponent) {
                    lastShape->addShape(c);
                } else {
                    addShape(c);
                    lastShape = c;
                }
            } else if (type == "A") { // arc
                Arc *a = parseArc(parts);
                if (bCompoundComponent) {
                    lastShape->addShape(a);
                } else {
                    addShape(a);
                    lastShape = a;
                }
            } else if (type == "P") { // pin
                Pin *p = parsePin(parts);
                if (bCompoundComponent) {
                    lastShape->addShape(p);
                } else {
                    addShape(p);
                    lastShape = p;
                }
            } else if (type == "G") { // picture
                Picture *p = parsePicture(parts, is);
                if (bCompoundComponent) {
                    lastShape->addShape(p);
                } else {
                    addShape(p);
                    lastShape = p;
                }
            } else if (type == "N") { // net
                Net *n = parseNet(parts);
                if (bCompoundComponent) {
                    lastShape->addShape(n);
                } else {
                    addShape(n);
                    lastShape = n;
                }
             } else if (type == "U") { // bus
                Bus *b = parseBus(parts);
                if (bCompoundComponent) {
                    lastShape->addShape(b);
                } else {
                    addShape(b);
                    lastShape = b;
                }
            } else if (type == "C") { // component
                Component *c = parseComponent(parts);
                if (bCompoundComponent) {
                    lastShape->addShape(c);
                } else {
                    addShape(c);
                    lastShape = c;
                }
            } else if (type == "H") { // path
                Path *c = parsePath(parts, is);
                if (bCompoundComponent) {
                    lastShape->addShape(c);
                } else {
                    addShape(c);
                    lastShape = c;
                }
            } else if (type == "{") { // compound shape start
                bCompoundComponent = true;
            } else if (type == "}") { // compound shape end
                bCompoundComponent = false;
            }
        }
    }
    return false;
}

void GSchemSchematicPrivate::addShape(Shape *shape) {
    mShapes.append(shape);
}


void GSchemSchematicPrivate::removeShape(Shape *shape) {
    mShapes.removeOne(shape);
}


Pin* GSchemSchematicPrivate::parsePin(QStringList parts) {
    int x1, y1, x2, y2;
    int color, pintype, whichend;

    x1 = parts.at(1).toInt();
    y1 = parts.at(2).toInt();
    x2 = parts.at(3).toInt();
    y2 = parts.at(4).toInt();
    color = parts.at(5).toInt();
    pintype = parts.at(6).toInt();
    whichend = parts.at(7).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Pin *p = new Pin(q_ptr, x1, y1, x2, y2, c, pintype, whichend);

    return p;
}

Path* GSchemSchematicPrivate::parsePath(QStringList parts, QTextStream *is) {
    int width, color, capstyle, dashstyle, dashlength = -1, dashspace = -1;
    int fillType/*, fillWidth, angle1, pitch1, angle2, pitch2*/, numLines;

    color = parts.at(1).toInt();
    width = parts.at(2).toInt();
    capstyle = parts.at(3).toInt();
    dashstyle = parts.at(4).toInt();
    dashlength = parts.at(5).toInt();
    dashspace = parts.at(6).toInt();
    fillType = parts.at(7).toInt();
//    fillWidth = parts.at(8).toInt();
//    angle1 = parts.at(9).toInt(); // TODO actually use these. Just using the
//    pitch1 = parts.at(10).toInt(); // Qt cross hatch and diagonal patterns.
//    angle2 = parts.at(11).toInt();
//    pitch2 = parts.at(12).toInt();
    numLines = parts.at(13).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Path *p = new Path(q_ptr, c, width);

    Qt::PenCapStyle capStyle = toCapStyle(capstyle);
    p->setCapstyle(capStyle);
    QPair<Qt::PenStyle, QVector<qreal> > penStyle = toPenStyle(dashstyle, dashlength, dashspace);
    p->setPenStyle(penStyle.first, penStyle.second);

    switch (fillType) {
    case 0:
        p->setFillType(Qt::NoBrush);
        break;
    case 1:
        p->setFillType(Qt::SolidPattern);
        break;
    case 2:
        p->setFillType(Qt::DiagCrossPattern);
        break;
    case 3:
        p->setFillType(Qt::BDiagPattern);
        break;
    case 4: // ot used
    default:
        break;
    }

    QString line, part;
    QStringList partList;
    for (int i = 0; i < numLines; i++) {
        line = is->readLine();
        partList = line.split(QRegularExpression("[,\\s+]"), QString::SkipEmptyParts);

        QString type = partList.at(0);
        partList.removeFirst();

        if (type == "M") {
            while (partList.size() >= 2) {
                p->addMove(partList.at(0).toInt(), partList.at(1).toInt());
                partList.removeFirst();
                partList.removeFirst();
            }
        } else if (type == "L") {
            while (partList.size() >= 2) {
                p->addLine(partList.at(0).toInt(), partList.at(1).toInt());
                partList.removeFirst();
                partList.removeFirst();
            }
        } else if (type == "C") {
            while (partList.size() >= 6) {
                p->addCurve(partList.at(0).toInt(), partList.at(1).toInt(),
                            partList.at(2).toInt(), partList.at(3).toInt(),
                            partList.at(4).toInt(), partList.at(5).toInt());
                partList.removeFirst();
                partList.removeFirst();
                partList.removeFirst();
                partList.removeFirst();
                partList.removeFirst();
                partList.removeFirst();
            }
        } else if (type.toUpper() == "Z") { // close can be upper or lowercase Z
            p->closePath();
        }

    }

    return p;
}

Component* GSchemSchematicPrivate::parseComponent(QStringList parts) {
    int x, y, selectable,angle, mirrored;
    QString basename;

    x = parts.at(1).toInt();
    y = parts.at(2).toInt();
    selectable = parts.at(3).toInt();
    angle = parts.at(4).toInt();
    mirrored = parts.at(5).toInt();
    basename = parts.at(6);

    Component *c = new Component(q_ptr, x, y, angle, mirrored);
    c->setSelectable(selectable == 1 ? true : false);
    c->setBaseName(basename);

    return c;

}

Rectangle* GSchemSchematicPrivate::parseBox(QStringList parts) {
    int x, y, width, height, lineWidth;
    int color, capstyle, dashstyle, dashlength = -1, dashspace = -1;
    int fillType/*, fillWidth, angle1, pitch1, angle2, pitch2*/;

    x = parts.at(1).toInt();
    y = parts.at(2).toInt();
    width = parts.at(3).toInt();
    height = parts.at(4).toInt();
    color = parts.at(5).toInt();
    lineWidth = parts.at(6).toInt();
    capstyle = parts.at(7).toInt();
    dashstyle = parts.at(8).toInt();
    dashlength = parts.at(9).toInt();
    dashspace = parts.at(10).toInt();
    fillType = parts.at(11).toInt();
//    fillWidth = parts.at(12).toInt();
//    angle1 = parts.at(13).toInt(); // TODO actually use these. Just using the
//    pitch1 = parts.at(14).toInt(); // Qt cross hatch and diagonal patterns.
//    angle2 = parts.at(15).toInt();
//    pitch2 = parts.at(16).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Rectangle *r = new Rectangle(q_ptr, x, y, x + width, y + height, lineWidth, c);

    Qt::PenCapStyle capStyle = toCapStyle(capstyle);
    r->setCapstyle(capStyle);
    QPair<Qt::PenStyle, QVector<qreal> > penStyle = toPenStyle(dashstyle, dashlength, dashspace);
    r->setPenStyle(penStyle.first, penStyle.second);
    switch (fillType) {
    case 0:
        r->setFillType(Qt::NoBrush);
        break;
    case 1:
        r->setFillType(Qt::SolidPattern);
        break;
    case 2:
        r->setFillType(Qt::DiagCrossPattern);
        break;
    case 3:
        r->setFillType(Qt::BDiagPattern);
        break;
    case 4: // ot used
    default:
        break;
    }

    return r;
}

Net* GSchemSchematicPrivate::parseNet(QStringList parts) {
    int x1, y1, x2, y2, color;

    x1 = parts.at(1).toInt();
    y1 = parts.at(2).toInt();
    x2 = parts.at(3).toInt();
    y2 = parts.at(4).toInt();
    color = parts.at(5).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Net *n = new Net(q_ptr, x1, y1, x2, y2, c);

    return n;
}

Bus* GSchemSchematicPrivate::parseBus(QStringList parts) {
    int x1, y1, x2, y2, color, ripperdir;

    x1 = parts.at(1).toInt();
    y1 = parts.at(2).toInt();
    x2 = parts.at(3).toInt();
    y2 = parts.at(4).toInt();
    color = parts.at(5).toInt();
    ripperdir = parts.at(6).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Bus *n = new Bus(q_ptr, x1, y1, x2, y2, c, ripperdir);

    return n;
}

Picture* GSchemSchematicPrivate::parsePicture(QStringList parts, QTextStream *is) {
    int x, y, width, height, angle, mirrored, embedded;

    x = parts.at(1).toInt();
    y = parts.at(2).toInt();
    width = parts.at(3).toInt();
    height = parts.at(4).toInt();
    angle = parts.at(5).toInt();
    mirrored = parts.at(6).toInt();
    embedded = parts.at(7).toInt(); // TODO not yet supported

    Picture *p = new Picture(q_ptr, x, y, width, height, angle, mirrored);

    switch (mirrored) {
    case 0:
        p->setMirrored(false);
        break;
    case 1:
        p->setMirrored(true);
        break;
    }

    QString line;
    switch (embedded) {
    case 0:
        line = is->readLine();
        p->setEmbedded(false);
        p->setFilename(line);
        break;
    case 1:
        p->setEmbedded(true);
        // TODO not yet supported by GSchem
        break;
    }

    return p;
}

Circle* GSchemSchematicPrivate::parseCircle(QStringList parts) {
    int x, y, width, radius;
    int color, capstyle, dashstyle, dashlength = -1, dashspace = -1;
    int fillType/*, fillWidth, angle1, pitch1, angle2, pitch2*/;

    x = parts.at(1).toInt();
    y = parts.at(2).toInt();
    radius = parts.at(3).toInt();
    color = parts.at(4).toInt();
    width = parts.at(5).toInt();
    capstyle = parts.at(6).toInt();
    dashstyle = parts.at(7).toInt();
    dashlength = parts.at(8).toInt();
    dashspace = parts.at(9).toInt();
    fillType = parts.at(10).toInt();
//    fillWidth = parts.at(11).toInt();
//    angle1 = parts.at(12).toInt();
//    pitch1 = parts.at(13).toInt();
//    angle2 = parts.at(14).toInt();
//    pitch2 = parts.at(15).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Circle *r = new Circle(q_ptr, x, y, radius, width, c);

    Qt::PenCapStyle capStyle = toCapStyle(capstyle);
    r->setCapstyle(capStyle);
    QPair<Qt::PenStyle, QVector<qreal> > penStyle = toPenStyle(dashstyle, dashlength, dashspace);
    r->setPenStyle(penStyle.first, penStyle.second);
    switch (fillType) {
    case 0:
        r->setFillType(Qt::NoBrush);
        break;
    case 1:
        r->setFillType(Qt::SolidPattern);
        break;
    case 2:
        r->setFillType(Qt::DiagCrossPattern);
        break;
    case 3:
        r->setFillType(Qt::BDiagPattern);
        break;
    case 4: // ot used
    default:
        break;
    }

    return r;
}

Arc* GSchemSchematicPrivate::parseArc(QStringList parts) {
    int x, y, radius, startAngle, sweepAngle, width;
    int color, capstyle, dashstyle, dashlength = -1, dashspace = -1;

    x = parts.at(1).toInt();
    y = parts.at(2).toInt();
    radius = parts.at(3).toInt();
    startAngle = parts.at(4).toInt() * 16; // angles in gschem are defined in 1/16 degree
    sweepAngle = parts.at(5).toInt() * 16;
    width = parts.at(6).toInt();
    color = parts.at(7).toInt();
    capstyle = parts.at(8).toInt();
    dashstyle = parts.at(9).toInt();
    dashlength = parts.at(10).toInt();
    dashspace = parts.at(11).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Arc *r = new Arc(q_ptr, x, y, radius, width, startAngle, sweepAngle, c);

    Qt::PenCapStyle capStyle = toCapStyle(capstyle);
    r->setCapstyle(capStyle);
    QPair<Qt::PenStyle, QVector<qreal> > penStyle = toPenStyle(dashstyle, dashlength, dashspace);
    r->setPenStyle(penStyle.first, penStyle.second);

    return r;
}

Text* GSchemSchematicPrivate::parseText(QStringList parts, QTextStream *is) {
    int x, y, color, size, visibility, showNameValue;
    int angle, alignment, numLines;
    QStringList text;

    x = parts.at(1).toInt();
    y = parts.at(2).toInt();
    color = parts.at(3).toInt();
    size = parts.at(4).toInt();
    visibility = parts.at(5).toInt();
    showNameValue = parts.at(6).toInt();
    angle = parts.at(7).toInt();
    alignment = parts.at(8).toInt();
    numLines = parts.at(9).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Text *t = new Text(q_ptr, x, y, angle, c);

    if (size < 2) size = 2; // minimum size is 2 points
    t->setPointSize(size);
    t->setVisible(visibility == 0 ? false : true); // INVISIBLE == 0, VISIBLE == 1

    switch (showNameValue) {
    case 0: // SHOW NAME VALUE
        t->setShowName(true);
        t->setShowValue(true);
        break;
    case 1: // SHOW VALUE
        t->setShowName(false);
        t->setShowValue(true);
        break;
    case 2: // SHOW NAME
        t->setShowName(true);
        t->setShowValue(false);
        break;
    default:
        t->setShowName(false);
        t->setShowValue(false);
        break;
    }

    switch (alignment) {
    case 0:
        t->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
        break;
    case 1:
        t->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        break;
    case 2:
        t->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        break;
    case 3:
        t->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        break;
    case 4:
        t->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        break;
    case 5:
        t->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        break;
    case 6:
        t->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        break;
    case 7:
        t->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        break;
    case 8:
        t->setAlignment(Qt::AlignRight | Qt::AlignTop);
        break;
    default:
        break;
    }
    for (int i = 0; i < numLines; i++) {
        QString line = is->readLine();
        if (line.contains("=")) {
            QStringList att = line.split("=", QString::SkipEmptyParts);
            t->setAttribute(att.at(0), att.at(1)); // text between two \_ are overlined
        } else {
            t->addString(line);
        }
    }
    return t;
}

Line* GSchemSchematicPrivate::parseLine(QStringList parts) {
    int x1, y1, x2, y2, width;
    int color, capstyle, dashstyle, dashlength = -1, dashspace = -1;

    x1 = parts.at(1).toInt();
    y1 = parts.at(2).toInt();
    x2 = parts.at(3).toInt();
    y2 = parts.at(4).toInt();
    color = parts.at(5).toInt();
    width = parts.at(6).toInt();
    capstyle = parts.at(7).toInt();
    dashstyle = parts.at(8).toInt();
    dashlength = parts.at(9).toInt();
    dashspace = parts.at(10).toInt();

    QColor c = pOptions->schemeColor(Options::GSCHEM_LIGHT, (Options::GSchemColorMapID)color);

    Line *l = new Line(q_ptr, x1, y1, x2, y2, width, c);

    Qt::PenCapStyle capStyle = toCapStyle(capstyle);
    l->setCapstyle(capStyle);
    QPair<Qt::PenStyle, QVector<qreal> > penStyle = toPenStyle(dashstyle, dashlength, dashspace);
    l->setPenStyle(penStyle.first, penStyle.second);

    return l;
}

Qt::PenCapStyle GSchemSchematicPrivate::toCapStyle(int style) {
    Qt::PenCapStyle mCapStyle;
    switch(style) {
    case 0:
        mCapStyle = Qt::FlatCap;
        break;
    case 1:
        mCapStyle = Qt::SquareCap;
        break;
    case 2:
        mCapStyle = Qt::RoundCap;
        break;
    default:
        mCapStyle = Qt::FlatCap;
        break;
    }
    return mCapStyle;
}

QPair<Qt::PenStyle, QVector<qreal> >  GSchemSchematicPrivate::toPenStyle(int style, int length, int gap) {
    Qt::PenStyle mPenStyle;
    QVector<qreal> mDashStyle;
    switch(style) {
    case 1: // DOTTED
        mPenStyle = Qt::DotLine;
        break;
    case 2: // DASHED
        mPenStyle = Qt::CustomDashLine;
        mDashStyle.append(int(length));
        mDashStyle.append(int(gap));
        break;
    case 3: // CENTER - basically dash-dot
        mPenStyle = Qt::CustomDashLine;
        mDashStyle.append(int(length));
        mDashStyle.append(int(gap));
        mDashStyle.append(1.0);
        mDashStyle.append(int(gap));
        break;
    case 4: // PHANTOM - basically dash-dot-dot
        mPenStyle = Qt::CustomDashLine;
        mDashStyle.append(int(length));
        mDashStyle.append(int(gap));
        mDashStyle.append(1.0);
        mDashStyle.append(int(gap));
        mDashStyle.append(1.0);
        mDashStyle.append(int(gap));
        break;
    case 0: // SOLID
    default:
        mPenStyle = Qt::SolidLine;
        mDashStyle.clear();
        break;
    }
    return QPair<Qt::PenStyle, QVector<qreal> >(mPenStyle, mDashStyle);
}
