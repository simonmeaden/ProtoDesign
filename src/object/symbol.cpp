#include "symbol.h"
#include "object/shape.h"
#include "object/line.h"
#include "object/arc.h"
#include "object/circle.h"
#include "object/rectangle.h"
#include "object/path.h"
#include "library/library.h"

Symbol::Symbol(QObject *parent) :
    QObject(parent),
    pLibrary(qobject_cast<Library*>(parent)) {

}

void Symbol::addShape(Shape *shape) {
    mShapes.insert(mShapes.size(), shape);
}

void Symbol::write() {
    QSqlQuery query = pLibrary->createQuery("INSERT INTO symbol (name, decription)"
                                            " VALUES (:name, :description)");
    query.bindValue(":name", mName);
    query.bindValue(":description", mDescription);
    if(query.exec()) {
        query.prepare("SELECT last_insert_rowid()");
        query.exec();
        if (query.next()) {
            mId = query.value(0).toInt();
         }
    }

    QMapIterator<int, Shape*> it(mShapes);
    while (it.hasNext()) {
        it.next();
        int position = it.key();
        Shape *shape = it.value();
        Line *line = qobject_cast<Line*>(shape);
        if (line) {
            writeLine(position,
                      line->x1(),
                      line->y1(),
                      line->x2(),
                      line->y2(),
                      line->lineWidth(),
                      line->layer(),
                      ColorShape::fromColor(line->color()));
            continue;
        }
        Rectangle *rect = qobject_cast<Rectangle*>(shape);
        if (rect) {
            writeRectangle(position,
                           rect->x1(),
                           rect->y1(),
                           rect->x2(),
                           rect->y2(),
                           rect->lineWidth(),
                           rect->layer(),
                           ColorShape::fromColor(rect->color()));
            continue;
        }
        Circle *circle = qobject_cast<Circle*>(shape);
        if (circle) {
            writeCircle(position,
                        circle->x(),
                        circle->y(),
                        circle->radius(),
                        circle->lineWidth(),
                        circle->layer(),
                        ColorShape::fromColor(circle->color()));
            continue;
        }
        Arc *arc = qobject_cast<Arc*>(shape);
        if (arc) {
            writeArc(position,
                     arc->x(),
                     arc->y(),
                     arc->radius(),
                     arc->startAngle(),
                     arc->deltaAngle(),
                     arc->lineWidth(),
                     arc->layer(),
                     ColorShape::fromColor(arc->color()));
            continue;
        }
        Path *path = qobject_cast<Path*>(shape);
        if (path) {
            writePath(position,
                      path);
            continue;
        }

    }
}

int Symbol::count() {
    return mShapes.size();
}


void Symbol::writeLine(int position, int x1, int y1, int x2, int y2, int width, int layer, int color) {
    QSqlQuery query = pLibrary->createQuery("INSERT INTO line (symbol_id, position, x1, y1, x2, y2, width, layer, color)"
                                            " VALUES (:id, :position, :x1, :y1, :x2, :y2, :width, :layer, :color)");
    query.bindValue(":id", mId);
    query.bindValue(":position", position);
    query.bindValue(":x1", x1);
    query.bindValue(":y1", y1);
    query.bindValue(":x2", x2);
    query.bindValue(":y2", y2);
    query.bindValue(":width", width);
    query.bindValue(":layer", layer);
    query.bindValue(":color", color);
    query.exec();
}

void Symbol::writeRectangle(int position, int x1, int y1, int x2, int y2, int width, int layer, int color) {
    QSqlQuery query = pLibrary->createQuery("INSERT INTO rectangle (symbol_id, position, x1, y1, x2, y2, width, layer, color)"
                                            " VALUES (:id, :position, :x1, :y1, :x2, :y2, :width, :layer, :color)");
    query.bindValue(":id", mId);
    query.bindValue(":position", position);
    query.bindValue(":x1", x1);
    query.bindValue(":y1", y1);
    query.bindValue(":x2", x2);
    query.bindValue(":y2", y2);
    query.bindValue(":width", width);
    query.bindValue(":layer", layer);
    query.bindValue(":color", color);
    query.exec();
}

void Symbol::writeArc(int position, int x, int y, int radius, int start, int delta, int width, int layer, int color) {
    QSqlQuery query = pLibrary->createQuery("INSERT INTO arc (symbol_id, position, x, y, radius, startangle, deltaangle, width, layer, color)"
                                            " VALUES (:id, :position, :x, :y, :radius, :start, :delta, :width, :layer, :color)");
    query.bindValue(":id", mId);
    query.bindValue(":position", position);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":radius", radius);
    query.bindValue(":start", start);
    query.bindValue(":delta", delta);
    query.bindValue(":width", width);
    query.bindValue(":layer", layer);
    query.bindValue(":color", color);
    query.exec();
}

void Symbol::writeCircle(int position, int x, int y, int radius, int width, int layer, int color) {
    QSqlQuery query = pLibrary->createQuery("INSERT INTO circle (symbol_id, position, x, y, radius, width, layer, color)"
                                            " VALUES (:id, :position, :x1, :y1, :x2, :y2, :width, :layer, :color)");
    query.bindValue(":id", mId);
    query.bindValue(":position", position);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":radius", radius);
    query.bindValue(":width", width);
    query.bindValue(":layer", layer);
    query.bindValue(":color", color);
    query.exec();
}

void Symbol::writePath(int position, Path *path) {
    QSqlQuery query = pLibrary->createQuery("INSERT INTO path (symbol_id, position, layer, color)"
                                            " VALUES (:id, :position, :layer, :color)");
    query.bindValue(":id", mId);
    query.bindValue(":position", position);
    query.bindValue(":layer", path->layer());
    query.bindValue(":color", ColorShape::fromColor(path->color()));
    if(query.exec()) {
        query.prepare("SELECT last_insert_rowid()");
        query.exec();
        if (query.next()) {
            int pathId = query.value(0).toInt();

            foreach (PathType* pt, path->path()) {
                MoveTo *mt = qobject_cast<MoveTo*>(pt);
                if (mt) {
                    query = pLibrary->createQuery("INSERT INTO moveto (path_id, x, y, position)"
                                                  "VALUES (:path_id, :x, :y, :position)");
                    query.bindValue(":path_id", pathId);
                    query.bindValue(":position", position);
                    query.bindValue(":x", mt->x());
                    query.bindValue(":y", mt->y());
                    query.exec();
                    continue;
                }
                LineTo *lt = qobject_cast<LineTo*>(pt);
                if (lt) {
                    query = pLibrary->createQuery("INSERT INTO lineto (path_id, x, y, position)"
                                                  "VALUES (:path_id, :x, :y, :position)");
                    query.bindValue(":path_id", pathId);
                    query.bindValue(":position", position);
                    query.bindValue(":x", lt->x());
                    query.bindValue(":y", lt->y());
                    query.exec();
                    continue;
                }
                CurveTo *ct = qobject_cast<CurveTo*>(pt);
                if (ct) {
                    query = pLibrary->createQuery("INSERT INTO curveto (path_id, x1, y1, x2, y2, x3, y3, position)"
                                                  "VALUES (:path_id, :x1, :y1, :x2, :y2, :x3, :y3, :position)");
                    query.bindValue(":path_id", pathId);
                    query.bindValue(":position", position);
                    query.bindValue(":x1", ct->x1());
                    query.bindValue(":y1", ct->y1());
                    query.bindValue(":x2", ct->x2());
                    query.bindValue(":y2", ct->y2());
                    query.bindValue(":x3", ct->x3());
                    query.bindValue(":y3", ct->y3());
                    query.exec();
                    continue;
                }
            }
         }
    }
}

