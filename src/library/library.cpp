#include "library.h"
#include "object/symbol.h"
#include "object/footprint.h"

Library::Library(QObject *parent) : QObject(parent) {

}

Library::~Library() {

}

void Library::open() {
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName("protodesign.db");
    bool success = mDb.open();
    if (!success) {
        qDebug() << "Error: connection with database fail";
    } else {
        qDebug() << "Database: connection ok";
    }
}

void Library::close() {
    mDb.close();
}

QSqlQuery Library::createQuery(QString query) {
    QSqlQuery q = QSqlQuery(query, mDb);
    return q;
}

Symbol* Library::createSymbol() {
    Symbol *symbol = new Symbol(this);
    return symbol;
}

Footprint* Library::createFootprint() {
    Footprint *footprint = new Footprint(this);
    return footprint;
}

Symbol* Library::readSymbol() {

}

void Library::writeSymbol(Symbol* symbol) {

}

Footprint* Library::readFootprint() {

}

void Library::writeFootprint(Footprint *footprint) {

}

