#ifndef SCHEMATICREADER_H
#define SCHEMATICREADER_H

#include <QDomDocument>
#include <QFile>

#include "schematic.h"

class XmlSchematicReader : public QObject {
    Q_OBJECT
public:
    XmlSchematicReader(Schematic *schematic, QObject *parent = 0);
    ~XmlSchematicReader();

signals:
    void warning(QString title, QString message);
    void information(QString title, QString message);

public slots:
    virtual bool read(QFile &file)=0;

protected:
    Schematic *m_schematic;
    QDomDocument *m_document;

    virtual void buildSchematic()=0;

};

#endif // SCHEMATICREADER_H
