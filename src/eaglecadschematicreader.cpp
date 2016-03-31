//#include <qstd.h>

#include "eaglecadschematicreader.h"
#include "eaglecadschematic.h"

EagleCadSchematicReader::EagleCadSchematicReader(ProtoDesignDB *db, EagleCadSchematic *schematic, QObject *parent) :
    XmlSchematicReader(schematic, parent),
    p_db(db) {

    m_document = new QDomDocument();
    netIndex = -1;

}

EagleCadSchematicReader::~EagleCadSchematicReader() {

}

void EagleCadSchematicReader::buildSchematic() {
    Part *part;
    DeviceSet *deviceSet;
    Package *package;
    Symbol *symbol;
    Device *device;
    QString library, dsName, deviceName, packageName, partName;
    QListIterator<Part*> itPart(m_parts);
    int i;
    while (itPart.hasNext()) {
        part = itPart.next();
        partName = part->mName;
        library = part->mLibrary;
        dsName = part->mDeviceSet;
        deviceName = part->mDevice;

        m_schematic->addPart(partName);

        deviceSet = getDeviceSet(library, dsName);
        device = deviceSet->getDevice(deviceName);
//        QList<EagleCadECConnect*> connects = device->connects();

        packageName = device->m_package;
        package = getPackage(library, packageName);
        m_schematic->addPackage(partName, package);

        QString symbolName;
        QList<Gate*> gates = deviceSet->getGates();
        QList<Symbol*> symbols;
        Gate *gate;
        for (i = 0; i < gates.size(); i++) {
            gate = gates.at(i);
            symbolName = gate->m_symbol;
            symbol = getSymbol(library, symbolName);
            if (!symbols.contains(symbol)) {
                symbols.append(symbol);
                m_schematic->addSymbol(partName, symbol);
            }
        }

        QList<Segment*> segments = getSegments();
        QString netname;
        if (!segments.isEmpty()) {
            for (i = 0; i < segments.size(); i++) {
                QList<PinRef*> *pinrefs = segments.at(i)->pinRefs();
                netname = netName();
                Link *link = new Link();
                link->m_connections = pinrefs;

                m_schematic->addLink(link);

                segments = nextSegments();
            }
        }

    }
}

QList<Segment*> EagleCadSchematicReader::getSegments() {
    nets = m_segmentsStore.keys();
    netIndex = 0;
    if (netIndex < nets.size()) {
        QList<Segment*> segments = m_segmentsStore.values(nets.at(netIndex));
        return segments;
    }

    return QList<Segment*>();
}

QList<Segment*> EagleCadSchematicReader::nextSegments() {
    netIndex++;
    if (netIndex < nets.size()) {
        QList<Segment*> segments = m_segmentsStore.values(nets.at(netIndex));
        return segments;
    }

    return QList<Segment*>();
}

QString EagleCadSchematicReader::netName() {
    if ((netIndex > -1) && (netIndex < nets.size())) {
        return nets.at(netIndex);
    }
    return QString();
}

Symbol* EagleCadSchematicReader::getSymbol(QString library, QString name) {
    QList<Symbol*> symbolList = m_symbolsStore.values(library);
    QListIterator<Symbol*> it(symbolList);
    Symbol* symbol;

    while (it.hasNext()) {
        symbol = it.next();

        if (symbol->m_name == name) {
            return symbol;
        }
    }

    return NULL;
}

DeviceSet* EagleCadSchematicReader::getDeviceSet(QString library, QString name) {
    QList<DeviceSet*> dsList = m_deviceSetsStore.values(library);
    QListIterator<DeviceSet*> it(dsList);
    DeviceSet* deviceSet;

    while (it.hasNext()) {
        deviceSet = it.next();

        if (deviceSet->m_name == name) {
            return deviceSet;
        }
    }

    return NULL;
}

Package* EagleCadSchematicReader::getPackage(QString library, QString name) {
    QList<Package*> packageList = m_packagesStore.values(library);
    QListIterator<Package*> it(packageList);
    Package* package;

    while (it.hasNext()) {
        package = it.next();

        if (package->name() == name) {
            return package;
        }
    }

    return NULL;
}

bool EagleCadSchematicReader::read(QFile &file) {

    int errorLine, errorColumn;
    QString errorStr;

    if (!m_document->setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        emit information(tr("ProtoDesign"),
                         tr("Parse error at line %1, column %2:\n%3")
                         .arg(errorLine)
                         .arg(errorColumn)
                         .arg(errorStr));
        return false;
    }

    QDomElement root = m_document->documentElement();
    if (root.tagName() != "eagle") {
        emit information(tr("ProtoDesign"),
                         tr("The file is not an Eagle Schematic file."));
        return false;
    } else if (root.hasAttribute("version")) {
        QStringList version = root.attribute("version").split(".");
        if (version.size() != 3 || version[0].toInt() < 6) {
            emit information(tr("ProtoDesign"),
                             tr("Only version 6 or later supported.."));
            return false;
        } else {
            m_versionMajor = version[0].toInt();
            m_versionMinor = version[1].toInt();
            m_versionBuild = version[2].toInt();
        }
    }

    QDomElement drawing = root.firstChildElement("drawing");
    if (drawing.isNull()) {
        emit information(tr("ProtoDesign"),
                         tr("Parse error - faulty Eagle Schematic file."));
        return false;
    }

    QDomElement schematic = drawing.firstChildElement("schematic");
    if (drawing.isNull()) {
        emit information(tr("ProtoDesign"),
                         tr("Parse error - faulty Eagle Schematic file."));
        return false;
    }

    QDomElement child = schematic.firstChildElement("grid");
    if (!child.isNull()) {
        parseGrid(child);
    }

    QDomElement libraries = schematic.firstChildElement("libraries");
    if (drawing.isNull()) {
        emit information(tr("ProtoDesign"),
                         tr("Parse error - faulty Eagle Schematic file."));
        return false;
    } else {
        QDomElement library = libraries.firstChildElement("library");
        if (library.isNull()) {
            emit information(tr("ProtoDesign"),
                             tr("Parse error - faulty Eagle Schematic file."));
            return false;
        }

        while (!library.isNull()) {
            QString attr = library.attribute("name");
            if (!attr.isNull()) {

                m_libraryName = attr;

                child = library.firstChildElement("packages");
                if (!child.isNull()) {
                    parsePackages(m_libraryName, child);
                }

                child = library.firstChildElement("symbols");
                if (!child.isNull()) {
                    parseSymbols(child);
                }

                child = library.firstChildElement("devicesets");
                if (!child.isNull()) {
                    parseDeviceSets(child);
                }

                m_libraryName.clear();

            }
            library = library.nextSiblingElement("library");
        }

        child = schematic.firstChildElement("parts");
        if (!child.isNull()) {
            parseParts(child);
        }

        child = schematic.firstChildElement("sheets");
        if (!child.isNull()) {
            parseSheets(child);
        }

    }

    buildSchematic();

    return true;
}


void EagleCadSchematicReader::parseSegment(QString netName, QDomElement elem) {
    ECSegment *segment = qobject_cast<EagleCadSchematic*>(m_schematic)->newSegment();
    QDomElement child1 = elem.firstChildElement("segment"), child2;
    while (!child1.isNull()) {
        child2 = child1.firstChildElement("label");
        while (!child2.isNull()) {
            parseLabel(child2, segment);
            child2 = child2.nextSiblingElement("label");
        }

        child2 = child1.firstChildElement("wire");
        while (!child2.isNull()) {
            parseWire(child2, segment);
            child2 = child2.nextSiblingElement("wire");
        }

        child2 = child1.firstChildElement("pinref");
        while (!child2.isNull()) {
            parsePinRef(child2, segment);
            child2 = child2.nextSiblingElement("pinref");
        }

        child2 = child1.firstChildElement("junction");
        while (!child2.isNull()) {
            parseJunction(child2, segment);
            child2 = child2.nextSiblingElement("junction");
        }

        child1 = elem.nextSiblingElement("segment");
    }
//    qobject_cast<EagleCadSchematic*>(m_schematic)->addSegment(netName, segment);
    m_segmentsStore.insert(netName, segment);
}

void EagleCadSchematicReader::parsePinRef(QDomElement elem, Container *container) {
    ECPinRef *pinref = qobject_cast<EagleCadSchematic*>(m_schematic)->newPinRef();

    if (pinref != NULL) {
        QString attr = elem.attribute("part");
        if (!attr.isEmpty())
            pinref->m_part = attr;

        attr = elem.attribute("gate");
        if (!attr.isEmpty())
            pinref->m_gate = attr;

        attr = elem.attribute("pin");
        if (!attr.isEmpty())
            pinref->m_pin = attr;
    }
    container->addObject(pinref);
}

void EagleCadSchematicReader::parseLabel(QDomElement elem, Container *container) {
    ECLabel *label = qobject_cast<EagleCadSchematic*>(m_schematic)->newLabel();

    if (label != NULL) {
        QString attr = elem.attribute("x");
        if (!attr.isEmpty())
            label->m_x = attr.toDouble();

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            label->m_y = attr.toDouble();

        attr = elem.attribute("size");
        if (!attr.isEmpty())
            label->m_y = attr.toDouble();

        attr = elem.attribute("layer");
        if (!attr.isEmpty())
            label->m_layer = attr.toInt();

        container->addObject(label);
    }
}

void EagleCadSchematicReader::parseJunction(QDomElement elem, Container *container) {
    ECJunction *junction = qobject_cast<EagleCadSchematic*>(m_schematic)->newJunction();

    if (junction != NULL) {
        QString attr = elem.attribute("x");
        if (!attr.isEmpty())
            junction->m_x = attr.toDouble();

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            junction->m_y = attr.toDouble();
    }
    container->addObject(junction);
}

void EagleCadSchematicReader::parseNets(QDomElement elem) {
    QDomElement child = elem.firstChildElement("net");
    while (!child.isNull()) {
        QString attr = elem.attribute("name");
        parseSegment(attr, child);
        child = child.nextSiblingElement("net");
    }
}

void EagleCadSchematicReader::parseInstance(QDomElement elem) {
    ECInstance *instance = qobject_cast<EagleCadSchematic*>(m_schematic)->newInstance();

    if (instance != NULL) {
        QString attr = elem.attribute("part");
        if (!attr.isEmpty())
            instance->m_part = attr;

        attr = elem.attribute("gate");
        if (!attr.isEmpty())
            instance->m_part = attr;

        attr = elem.attribute("x");
        if (!attr.isEmpty())
            instance->m_x = attr.toDouble();

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            instance->m_y = attr.toDouble();

        attr = elem.attribute("rot");
        if (!attr.isEmpty()) {
            if (attr.startsWith("MR")) {
                instance->m_mirrored = true;
                instance->m_rotated = true;
                instance->m_angle = attr.mid(2).toInt();
                if (instance->m_angle == 0)
                    instance->m_rotated = false;
            } else if (attr.startsWith("R")) {
                instance->m_mirrored = false;
                instance->m_rotated = true;
                instance->m_angle = attr.mid(1).toInt();
                if (instance->m_angle == 0)
                    instance->m_rotated = false;
            }
        }

        attr = elem.attribute("smashed");
        if (!attr.isNull()) {
            if (attr.toLower() == "yes") instance->m_smashed = true;
            else instance->m_smashed = false;

            QDomElement child = elem.firstChildElement("attribute");
            while (!child.isNull()) {
                ECInstance::ECAttribute *attibute = instance->newAttribute();

                attr = child.attribute("name");
                if (!attr.isEmpty())
                    attibute->m_name = attr;

                attr = child.attribute("x");
                if (!attr.isEmpty())
                    attibute->m_x = attr.toDouble();

                attr = child.attribute("y");
                if (!attr.isEmpty())
                    attibute->m_y = attr.toDouble();

                attr = child.attribute("size");
                if (!attr.isEmpty())
                    attibute->m_size = attr.toDouble();

                attr = child.attribute("layer");
                if (!attr.isEmpty())
                    attibute->m_layer = attr.toInt();

                attr = elem.attribute("rot");
                if (!attr.isEmpty()) {
                    if (attr.startsWith("MR")) {
                        attibute->m_mirrored = true;
                        attibute->m_rotated = true;
                        attibute->m_angle = attr.mid(2).toInt();
                        if (attibute->m_angle == 0)
                            attibute->m_rotated = false;
                    } else if (attr.startsWith("R")) {
                        attibute->m_mirrored = false;
                        attibute->m_rotated = true;
                        attibute->m_angle = attr.mid(1).toInt();
                        if (attibute->m_angle == 0)
                            attibute->m_rotated = false;
                    }
                }

                instance->addAttribute(attibute);

                child = child.nextSiblingElement("attribute");
            }
        }

//        qobject_cast<EagleCadSchematic*>(m_schematic)->addInstance(instance);
        m_instances.append(instance);
    }
}

void EagleCadSchematicReader::parseSheet(QDomElement elem) {
    QDomElement child = elem.firstChildElement("instances");
    if (!child.isNull()) {
        parseInstances(child);
        child = child.nextSiblingElement("instances");
    }

    child = elem.firstChildElement("nets");
    if (!child.isNull()) {
        parseNets(child);
        child = child.nextSiblingElement("nets");
    }
}

void EagleCadSchematicReader::parseInstances(QDomElement elem) {
    QDomElement child = elem.firstChildElement("instance");
    while (!child.isNull()) {
        parseInstance(child);
        child = child.nextSiblingElement("instance");
    }
}

void EagleCadSchematicReader::parseSheets(QDomElement elem) {
    QDomElement child = elem.firstChildElement("sheet");
    while (!child.isNull()) {
        parseSheet(child);
        child = child.nextSiblingElement("sheet");
    }
}

void EagleCadSchematicReader::parseDeviceSet(QDomElement elem) {
    ECDeviceSet *deviceSet = qobject_cast<EagleCadSchematic*>(m_schematic)->newDeviceSet();

    if (deviceSet != NULL) {
        QString attr = elem.attribute("name");
        if (!attr.isEmpty())
            deviceSet->m_name = attr;

        QDomElement child1 = elem.firstChildElement("description"), child2;
        if (!child1.isNull())
            deviceSet->m_description = child1.text();

        child1 = elem.firstChildElement("gates");
        while (!child1.isNull()) {
            child2 = child1.firstChildElement("gate");
            while (!child2.isNull()) {
                parseGate(child2, deviceSet);
                child2 = child2.nextSiblingElement("gate");
            }
            child1 = elem.nextSiblingElement("gates");
        }

        child1 = elem.firstChildElement("devices");
        while (!child1.isNull()) {
            child2 = child1.firstChildElement("device");
            while (!child2.isNull()) {
                parseDevice(child2, deviceSet);
                child2 = child2.nextSiblingElement("device");
            }
            child1 = elem.nextSiblingElement("devices");
        }

//        qobject_cast<EagleCadSchematic*>(m_schematic)->addDeviceSet(m_libraryName, deviceSet);
        m_deviceSetsStore.insert(m_libraryName, deviceSet);
    }
}

void EagleCadSchematicReader::parseDevice(QDomElement elem, Container *container) {
    ECDevice *device = qobject_cast<EagleCadSchematic*>(m_schematic)->newDevice();

    QString attr = elem.attribute("name");
    if (!attr.isEmpty())
        device->m_name = attr;

    attr = elem.attribute("package");
    if (!attr.isEmpty())
        device->m_package = attr;

    QDomElement child1 = elem.firstChildElement("connects");
    if (!child1.isNull()) {
        parseConnects(child1, device);
    }

    child1 = elem.firstChildElement("technologies");
    if (!child1.isNull()) {
        parseTechnologies(child1, device);
    }

    container->addObject(device);
}

void EagleCadSchematicReader::parseConnects(QDomElement elem, ECDevice *container) {
    QDomElement child = elem.firstChildElement("connect");
    while (!child.isNull()) {
        parseConnect(child, container);
        child = child.nextSiblingElement("connect");
    }
}

void EagleCadSchematicReader::parseConnect(QDomElement elem, ECDevice *container) {
    ECConnect *connect = qobject_cast<EagleCadSchematic*>(m_schematic)->newConnect();

    QString attr = elem.attribute("gate");
    if (!attr.isEmpty())
        connect->m_gate = attr;

    attr = elem.attribute("pin");
    if (!attr.isEmpty())
        connect->m_pin = attr;

    attr = elem.attribute("pad");
    if (!attr.isEmpty())
        connect->m_pad = attr;

    container->addConnect(connect);
}

void EagleCadSchematicReader::parseTechnologies(QDomElement elem, ECDevice *container) {
    QDomElement child = elem.firstChildElement("technology");
    while (!child.isNull()) {
        parseTechnology(child, container);
        child = child.nextSiblingElement("technology");
    }
}

void EagleCadSchematicReader::parseTechnology(QDomElement elem, ECDevice *container) {
    ECTechnology *tech = qobject_cast<EagleCadSchematic*>(m_schematic)->newTechnology();

    QString attr = elem.attribute("name");
    if (!attr.isEmpty())
        tech->m_name = attr;

    container->addTechnology(tech);
}

void EagleCadSchematicReader::parseGate(QDomElement elem, Container *container) {
    ECGate *gate = qobject_cast<EagleCadSchematic*>(m_schematic)->newGate();

    QString attr = elem.attribute("name");
    if (!attr.isEmpty())
        gate->m_name = attr;

    attr = elem.attribute("symbol");
    if (!attr.isEmpty())
        gate->m_symbol = attr;

    attr = elem.attribute("x");
    if (!attr.isEmpty())
        gate->m_x = attr.toDouble();

    attr = elem.attribute("y");
    if (!attr.isEmpty())
        gate->m_y = attr.toDouble();

    container->addObject(gate);
}

void EagleCadSchematicReader::parseDeviceSets(QDomElement elem) {
    QDomElement child = elem.firstChildElement("deviceset");
    while (!child.isNull()) {
        parseDeviceSet(child);
        child = child.nextSiblingElement("deviceset");
    }
}

void EagleCadSchematicReader::parseSymbol(QDomElement elem) {
    ECSymbol *symbol = qobject_cast<EagleCadSchematic*>(m_schematic)->newSymbol();

    if (symbol != NULL) {
        QString attr = elem.attribute("name");
        if (!attr.isEmpty())
            symbol->m_name = attr;

        QDomElement child = elem.firstChildElement("description");
        if (!child.isNull())
            symbol->m_description = child.text();

        child = elem.firstChildElement("pin");
        while (!child.isNull()) {
            parsePin(child, symbol);
            child = child.nextSiblingElement("pin");
        }

        child = elem.firstChildElement("wire");
        while (!child.isNull()) {
            parseWire(child, symbol);
            child = child.nextSiblingElement("wire");
        }

        child = elem.firstChildElement("circle");
        while (!child.isNull()) {
            parseCircle(child, symbol);
            child = child.nextSiblingElement("circle");
        }

        child = elem.firstChildElement("rectangle");
        while (!child.isNull()) {
            parseRectangle(child, symbol);
            child = child.nextSiblingElement("rectangle");
        }

        child = elem.firstChildElement("text");
        while (!child.isNull()) {
            parseText(child, symbol);
            child = child.nextSiblingElement("text");
        }
    }

    m_symbolsStore.insert(m_libraryName, symbol);
}

void EagleCadSchematicReader::parseSymbols(QDomElement elem) {
    QDomElement child = elem.firstChildElement("symbol");
    while (!child.isNull()) {
        parseSymbol(child);
        child = child.nextSiblingElement("symbol");
    }
}


void EagleCadSchematicReader::parseRectangle(QDomElement elem, Container *container) {
    ECRectangle *rectangle = qobject_cast<EagleCadSchematic*>(m_schematic)->newRectangle();
    QString attr;
    float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;

    if (rectangle != NULL) {
        attr = elem.attribute("x1");
        if (!attr.isEmpty())
            x1 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("y1");
        if (!attr.isEmpty())
            y1 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("x2");
        if (!attr.isEmpty())
            x2 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("y2");
        if (!attr.isEmpty())
            y2 = attr.toFloat() * 1000.0f;

        rectangle->set(x1, y1, x2, y2);

        attr = elem.attribute("layer");
        if (!attr.isEmpty())
            rectangle->m_layer = attr.toInt();
    }

    container->addObject(rectangle);
}

void EagleCadSchematicReader::parsePin(QDomElement elem, Container *container) {
    ECPin *pin = qobject_cast<EagleCadSchematic*>(m_schematic)->newPin();
    QString attr;

    if (pin != NULL) {
        attr = elem.attribute("name");
        if (!attr.isEmpty())
            pin->m_name = attr;

        attr = elem.attribute("x");
        if (!attr.isEmpty())
            pin->m_x = attr.toFloat() * 1000.0f;

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            pin->m_y = attr.toFloat() * 1000.0f;

        attr = elem.attribute("visible");
        if (!attr.isEmpty())
            pin->m_visible = pin->visibleFromString(attr);

        attr = elem.attribute("length");
        if (!attr.isEmpty())
            pin->m_length = pin->lengthFromString(attr);

        attr = elem.attribute("direction");
        if (!attr.isEmpty())
            pin->m_direction = pin->directionFromString(attr);
    }

    container->addObject(pin);
}

void EagleCadSchematicReader::parseText(QDomElement elem, Container *container) {
    ECText *text = qobject_cast<EagleCadSchematic*>(m_schematic)->newText();
    QString attr;

    if (text != NULL) {
        text->setText(elem.text());

        attr = elem.attribute("x");
        if (!attr.isEmpty())
            text->setX(attr.toFloat() * 1000.0f);

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            text->setY(attr.toFloat() * 1000.0f);

        attr = elem.attribute("size");
        if (!attr.isEmpty())
            text->setSize(attr.toFloat() * 1000.0f);

        attr = elem.attribute("layer");
        if (!attr.isEmpty())
            text->setLayer(attr.toInt());
    }

    container->addObject(text);
}

void EagleCadSchematicReader::parseSMD(QDomElement elem, Container *container) {
    ECSmd *smd = qobject_cast<EagleCadSchematic*>(m_schematic)->newSMD();
    QString attr;

    if (smd != NULL) {
        attr = elem.attribute("name");
        if (!attr.isEmpty())
            smd->setName(attr);

        attr = elem.attribute("x");
        if (!attr.isEmpty())
            smd->setX(attr.toFloat() * 1000.0f);

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            smd->setY(attr.toFloat() * 1000.0f);

        attr = elem.attribute("dx");
        if (!attr.isEmpty())
            smd->setDX(attr.toFloat() * 1000.0f);

        attr = elem.attribute("dy");
        if (!attr.isEmpty())
            smd->setDY(attr.toFloat() * 1000.0f);

        attr = elem.attribute("layer");
        if (!attr.isEmpty())
            smd->setLayer(attr.toInt());
    }

    container->addObject(smd);
}

void EagleCadSchematicReader::parseCircle(QDomElement elem, Container *container) {
    ECCircle *circle = qobject_cast<EagleCadSchematic*>(m_schematic)->newCircle();
    QString attr;

    float x1 = 0.0f, y1 = 0.0f, radius = 0.0f;

    if (circle != NULL) {
        attr = elem.attribute("x");
        if (!attr.isEmpty())
            x1 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            y1 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("radius");
        if (!attr.isEmpty())
            radius = attr.toFloat() * 1000.0f;


        circle->set(x1, y1, radius);

        attr = elem.attribute("layer");
        if (!attr.isEmpty())
            circle->setLayer(attr.toInt());
    }

    container->addObject(circle);
}

void EagleCadSchematicReader::parseWire(QDomElement elem, Container *container) {
    ECWire *wire = qobject_cast<EagleCadSchematic*>(m_schematic)->newWire();
    QString attr;

    float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;

    if (wire != NULL) {
        attr = elem.attribute("x1");
        if (!attr.isEmpty())
            x1 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("y1");
        if (!attr.isEmpty())
            y1 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("x2");
        if (!attr.isEmpty())
            x2 = attr.toFloat() * 1000.0f;

        attr = elem.attribute("y2");
        if (!attr.isEmpty())
            y2 = attr.toFloat() * 1000.0f;

        wire->set(x1, y1, x2, y2);

        attr = elem.attribute("layer");
        if (!attr.isEmpty())
            wire->setLayer(attr.toInt());
    }

    container->addObject(wire);
}

void EagleCadSchematicReader::parsePad(QDomElement elem, Container *container) {
    ECPad *pad = qobject_cast<EagleCadSchematic*>(m_schematic)->newPad();
    QString attr;

    if (pad != NULL) {
        attr = elem.attribute("name");
        if (!attr.isEmpty())
            pad->setName(attr);

        attr = elem.attribute("x");
        if (!attr.isEmpty())
            pad->setX(attr.toFloat() * 1000.0f);

        attr = elem.attribute("y");
        if (!attr.isEmpty())
            pad->setY(attr.toFloat() * 1000.0f);

        attr = elem.attribute("drill");
        if (!attr.isEmpty())
            pad->setDrill(attr.toFloat() * 1000.0f);

        attr = elem.attribute("diameter");
        if (!attr.isEmpty())
            pad->setDiameter(attr.toFloat() * 1000.0f);

        attr = elem.attribute("shape");
        if (!attr.isEmpty())
            pad->setShape(pad->shapeFromString(attr));
    }

    container->addObject(pad);
}

void EagleCadSchematicReader::parsePackage(QString libraryName, QDomElement elem) {
    ECPackage *package = qobject_cast<EagleCadSchematic*>(m_schematic)->newPackage();

    if (package != NULL) {
        package->setLibraryName(libraryName);

        QString attr = elem.attribute("name");
        if (!attr.isEmpty())
            package->setName(attr);

        QDomElement child = elem.firstChildElement("description");
        if (!child.isNull())
            package->setDescription(child.text());

        child = elem.firstChildElement("pad");
        while (!child.isNull()) {
            parsePad(child, package);
            child = child.nextSiblingElement("pad");
        }

        child = elem.firstChildElement("wire");
        while (!child.isNull()) {
            parseWire(child, package);
            child = child.nextSiblingElement("wire");
        }

        child = elem.firstChildElement("smd");
        while (!child.isNull()) {
            parseSMD(child, package);
            child = child.nextSiblingElement("smd");
        }

        child = elem.firstChildElement("circle");
        while (!child.isNull()) {
            parseCircle(child, package);
            child = child.nextSiblingElement("circle");
        }

        child = elem.firstChildElement("rectangle");
        while (!child.isNull()) {
            parseRectangle(child, package);
            child = child.nextSiblingElement("rectangle");
        }

        child = elem.firstChildElement("text");
        while (!child.isNull()) {
            parseText(child, package);
            child = child.nextSiblingElement("text");
        }
    }

    p_db->addPackage(package->name(), package);
    m_packagesStore.insert(m_libraryName, package);
}



void EagleCadSchematicReader::parsePackages(QString libraryName, QDomElement elem) {
    QDomElement child = elem.firstChildElement("package");
    while (!child.isNull()) {
        parsePackage(libraryName, child);
        child = child.nextSiblingElement("package");
    }
}

void EagleCadSchematicReader::parsePart(QDomElement elem) {
    ECPart *part = qobject_cast<EagleCadSchematic*>(m_schematic)->newPart();
    QString attr;

    if (part != NULL) {
        attr = elem.attribute("name");
        if (!attr.isEmpty())
            part->mName = attr;

        attr = elem.attribute("library");
        if (!attr.isEmpty())
            part->mLibrary = attr;

        attr = elem.attribute("deviceset");
        if (!attr.isEmpty())
            part->mDeviceSet = attr;

        attr = elem.attribute("device");
        if (!attr.isEmpty())
            part->mDevice = attr;

        attr = elem.attribute("value");
        if (!attr.isEmpty())
            part->mValue = attr;
    }

    m_parts.append(part);
}

void EagleCadSchematicReader::parseParts(QDomElement elem) {
    QDomElement child = elem.firstChildElement("part");
    while (!child.isNull()) {
        parsePart(child);
        child = child.nextSiblingElement("part");
    }
}

void EagleCadSchematicReader::parseGrid(QDomElement elem) {
    QString attr;

    attr = elem.attribute("distance");
    if (!attr.isEmpty())
        grid->m_distance = attr.toDouble();

    attr = elem.attribute("unitdist");
    if (!attr.isEmpty())
        grid->m_unitDistance = grid->unitFromString(attr);

    attr = elem.attribute("unit");
    if (!attr.isEmpty())
        grid->m_unit = grid->unitFromString(attr);

    attr = elem.attribute("style");
    if (!attr.isEmpty())
        grid->m_style = grid->styleFromString(attr);

    attr = elem.attribute("multiple");
    if (!attr.isEmpty())
        grid->m_multiple = attr.toInt();

    attr = elem.attribute("display");
    if (!attr.isEmpty())
        grid->m_display = (attr.toLower() == "no" ? false : true);

    attr = elem.attribute("altdistance");
    if (!attr.isEmpty())
        grid->m_altDistance = attr.toDouble();

    attr = elem.attribute("altunitdist");
    if (!attr.isEmpty())
        grid->m_altUnitDistance = grid->unitFromString(attr);

    attr = elem.attribute("altunit");
    if (!attr.isEmpty())
        grid->m_altUnit = grid->unitFromString(attr);
}
