#ifndef DETECT_H
#define DETECT_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class Detect {
public:
    Detect();

    static bool isEagleCadSchematic(QString);
    static bool isGedaSchematic(QString);

};

#endif // DETECT_H
