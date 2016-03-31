#include "componentbox.h"

ComponentBox::ComponentBox(QWidget *parent) :
    QFrame(parent) {

}

void ComponentBox::paint(QPainter *painter) {

    QList<QString> parts = pSchematic->parts();
    QList<PositionedPackage*> packages;
    PositionedPackage *package;
    QString partName;
    for (int i = 0; i < parts.size(); i++) {
        partName = parts.at(i);

        packages = mPackages.values(partName);

        for (int j = 0; j < packages.size(); j++) {
            package = packages.at(j);
            package->package()->paint(painter, package->offset());
        }
    }

}
