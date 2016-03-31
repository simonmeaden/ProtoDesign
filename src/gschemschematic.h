#ifndef GEDASCHEMATIC_H
#define GEDASCHEMATIC_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QRegularExpression>

#include "schematic.h"

class GSchemSchematicPrivate;

class GSchemSchematic : public QObject, public Schematic {
    Q_OBJECT
public:
    GSchemSchematic(QObject *parent);
    ~GSchemSchematic();

    virtual bool read(QString);

protected slots:

protected:
    //    GSchemSchematic(GSchemSchematicPrivate &d/*, QObject *parent*/);
    GSchemSchematicPrivate * d_ptr;

private:

};

#endif // GEDASCHEMATIC_H
