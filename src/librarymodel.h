#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringListModel>

class LibraryModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit LibraryModel(QObject *parent);
};

#endif // LIBRARYMODEL_H
