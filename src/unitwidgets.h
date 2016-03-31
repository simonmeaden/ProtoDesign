#ifndef UNITWIDGETS_H
#define UNITWIDGETS_H

#include <QDoubleSpinBox>
#include <QComboBox>

#include <units.h>


class UnitComboBox : public QComboBox {
    Q_OBJECT
public:
    UnitComboBox(QWidget *parent = 0);

signals:
    /*!
     * \brief Signal sent whenever the selected unit is changed.
     * \param unit - the new \c Unit type;
     */
    void unitHasChanged(Unit m_unit);

public slots:
    virtual void setCurrentIndex(int index);

private slots:
    void indexChanged(int index);
    void indexChanged(QString m_unit);
    void addItem(const QString &text, const QVariant &userData);
    void addItem(const QIcon &icon, const QString &text, const QVariant &userData);
    void addItems(const QStringList &texts);

protected:
    Unit m_unit;
    QMap<QString, Unit> m_strings;
};

class UnitSpinBox : public QDoubleSpinBox {
    Q_OBJECT
public:
    explicit UnitSpinBox(QWidget *parent = 0);

signals:
    void valueHasChanged(float value);

public slots:
    void setUnit(Unit unit) {
        unitChanged(unit);
    }
    void unitChanged(Unit unit);
    virtual void setValue(float value);

private slots:
    void setBoxState(Unit unit);
    void valueHasBeenChanged(float value);

protected:
    float m_value;
    QString m_displayValue;
    Unit m_unit;

};

#endif // UNITWIDGETS_H
