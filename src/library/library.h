#ifndef LIBRARY_H
#define LIBRARY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class Symbol;
class Footprint;

class Library : public QObject {
    Q_OBJECT
public:
    enum Type {SYMBOL = 1, PACKAGE = 2,};
    explicit Library(QObject *parent = 0);
    ~Library();

    Symbol *readSymbol();
    Footprint* readFootprint();

    Symbol* createSymbol();
    Footprint* createFootprint();

    QSqlQuery createQuery(QString);

signals:

public slots:
    void open();
    void close();
    void writeSymbol(Symbol*);
    void writeFootprint(Footprint*);

protected:
    QSqlDatabase mDb;

};

#endif // LIBRARY_H
