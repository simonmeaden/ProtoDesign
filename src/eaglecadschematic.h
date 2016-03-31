#ifndef EAGLECADSCHEMATIC_H
#define EAGLECADSCHEMATIC_H

#include <QDebug>
#include <QFrame>
#include <QMap>
#include <QMultiMap>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QDomElement>
#include <QFile>

#include "schematic.h"


class EagleCadSchematic : public QObject, public Schematic {
    Q_OBJECT
public:
    explicit EagleCadSchematic(QObject *parent = 0);
    ~EagleCadSchematic();

    bool read(QString filename);

signals:

public slots:

protected:
    int mVersionMajor, mVersionMinor, mVersionBuild;

    void parseGrid(QDomElement);


};


#endif // EAGLECADSCHEMATIC_H
