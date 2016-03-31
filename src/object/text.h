#ifndef TEXT_H
#define TEXT_H

#include <QObject>

#include "object/xyshape.h"
#include "object/shape.h"

class Text : public XYShape, public AngledShape, public ColorShape {
    Q_OBJECT
public:
    explicit Text(QObject*, int, int, int, QColor);
    ~Text();

    bool isIn(int, int);
    virtual void paint(QPainter &painter, QPointF offset);
    int pointSize();
    bool visible();
    bool showName();
    bool showValue();
    Qt::Alignment alignment();
    int attributeCount();
    int stringCount();
    QString string(int);
    QString attribute(int);
    QString value(int);

public slots:
    void setPointSize(int);
    void setVisible(bool);
    void setShowName(bool);
    void setShowValue(bool);
    void setAlignment(Qt::Alignment);
    void addString(QString);
    void setAttribute(QString, QString);

protected:
    QStringList mText;
    QList<QPair<QString, QString> > mAttributes;
    int mPointSize, mAngle;
    bool bVisible, bShowName, bShowValue;
    Qt::Alignment mAlignment;
};
#endif // TEXT_H
