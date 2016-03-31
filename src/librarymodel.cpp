#include "librarymodel.h"

LibraryModel::LibraryModel(QObject *parent) : QSortFilterProxyModel(parent) {
    setDynamicSortFilter(true);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSourceModel(new QStringListModel(parent));
}
