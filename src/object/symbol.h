#ifndef SYMBOL_H
#define SYMBOL_H

#include <QObject>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>


class Shape;
class Path;
class Library;

class Symbol : public QObject {
    Q_OBJECT
public:

    int count();


signals:

public slots:
    void addShape(Shape*);

    void write();

protected slots:
    void writeLine(int, int, int, int, int, int, int, int);
    void writeRectangle(int, int, int, int, int, int, int, int);
    void writeArc(int, int, int, int, int, int, int, int, int);
    void writeCircle(int, int, int, int, int, int, int);
    void writePath(int, Path*);

protected:
    explicit Symbol(QObject *parent = 0);

    Library *pLibrary;
    QMap<int, Shape*> mShapes;
    QString mName, mDescription;
    int mId;

    friend class Library;
};

#endif // SYMBOL_H
