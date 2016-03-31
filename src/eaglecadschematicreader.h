#ifndef EAGLECADSCHEMATICREADER_H
#define EAGLECADSCHEMATICREADER_H

#include <QObject>
#include <QFile>
#include <QMessageBox>
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>
#include <QMap>
#include <QMultiMap>

#include <protodesigndb.h>

#include "xmlschematicreader.h"
#include "eaglecadschematic.h"


class EagleCadSchematicReader :  public XmlSchematicReader {
    Q_OBJECT
public:

    explicit EagleCadSchematicReader(ProtoDesignDB *db, EagleCadSchematic *schematic, QObject *parent=0);
    ~EagleCadSchematicReader();

    virtual bool read(QFile &file);
    DeviceSet* getDeviceSet(QString library, QString name);
    Package* getPackage(QString library, QString name);
    Symbol* getSymbol(QString library, QString name);


signals:

protected slots:
    void parseGrid(QDomElement elem);
    void parsePart(QDomElement elem);
    void parseParts(QDomElement elem);
    void parsePackage(QString libraryName, QDomElement elem);
    void parsePackages(QString libraryName, QDomElement elem);
    void parseGate(QDomElement elem, Container *container);
    void parseDevice(QDomElement elem, Container *container);
    void parseConnect(QDomElement elem, ECDevice *container);
    void parseConnects(QDomElement elem, ECDevice *container);
    void parseTechnology(QDomElement elem, ECDevice *container);
    void parseTechnologies(QDomElement elem, ECDevice *container);
    void parseDeviceSet(QDomElement elem);
    void parseDeviceSets(QDomElement elem);
    void parseSymbol(QDomElement elem);
    void parseSymbols(QDomElement elem);
    void parseSheet(QDomElement elem);
    void parseNets(QDomElement elem);
    void parseJunction(QDomElement elem, Container *container);
    void parsePinRef(QDomElement elem, Container *container);
    void parseSegment(QString netName, QDomElement elem);
    void parseInstance(QDomElement elem);
    void parseInstances(QDomElement elem);
    void parseSheets(QDomElement elem);
    void parseLabel(QDomElement elem, Container *container);
    void parsePad(QDomElement elem, Container *container);
    void parsePin(QDomElement elem, Container *container);
    void parseWire(QDomElement elem, Container *container);
    void parseCircle(QDomElement elem, Container *container);
    void parseRectangle(QDomElement elem, Container *container);
    void parseText(QDomElement elem, Container *container);
    void parseSMD(QDomElement elem, Container *container);

protected:
    ProtoDesignDB *p_db;
    int m_versionMajor, m_versionMinor, m_versionBuild;
    QString m_libraryName;
    QList<Part*> m_parts;
    QMultiMap<QString, Package*> m_packagesStore;
    QMultiMap<QString, Symbol*> m_symbolsStore;
    QMultiMap<QString, DeviceSet*> m_deviceSetsStore;
    QMultiMap<QString, Segment*> m_segmentsStore;
    QList<ECInstance*> m_instances;
    QList<QString> nets;
    int netIndex;


    virtual void buildSchematic();
    QList<Segment *> getSegments();
    QList<Segment *> nextSegments();
    QString netName();

};



#endif // EAGLECADSCHEMATICREADER_H
