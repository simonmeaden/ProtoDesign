#include "detect.h"

Detect::Detect() {

}

bool Detect::isEagleCadSchematic(QString filename) {
    QFile file(filename);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QTextStream *is = new QTextStream(&file);
        QString line = is->readLine();
        line = is->readLine();
        line = is->readLine();
        if (line.startsWith("<eagle")) { // third line. first are xml shit
            return true;
        } else {
            return false;
        }
    }
}

bool Detect::isGedaSchematic(QString filename) {
    QFile file(filename);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QTextStream *is = new QTextStream(&file);
        QString line = is->readLine();
        if (line.startsWith("v")) { // first character is version.
            return true;
        } else {
            return false;
        }
    }
}
