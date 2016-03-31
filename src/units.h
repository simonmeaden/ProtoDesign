#ifndef UNITS
#define UNITS

#ifndef UNITSPINBOX_H
#define UNITSPINBOX_H

#include <QMap>
#include <QPointF>
#include <QStringList>

/*!
    \enum Unit

    This enum type specifies a unit type:

    \value INCH
           measurement in inches.
    \value MM
           measurement in millimetres.
    \value MIL
           measurement in mil (1/1000 of an inch).
    \value MIC
           measurement in microns (1/1000 of a millimetre).
    \value NONE
           not a measureable value.
*/
enum Unit {
    INCH,// inch - 25.4mm
    MM,  // millimetre
    MIL, // 1/1000 of an inch
    MIC, // 1/1000 of a mm
    NONE,
  PIXEL,
};

class UnitConvert {
public:
    explicit UnitConvert() {}

    static float internalToScreen(float value);
    static QPointF internalToScreen(QPointF value);
    static float screenToInternal(float value);
    static QPointF screenToInternal(QPointF value);
    static float toInternal(Unit unit, float value);
    static float fromInternal(Unit unit, float value);
    static float betweenUnits(Unit unitIn, Unit unitOut, float valueIn);
    static QString toString(Unit unit);
    static QStringList strings();
    static Unit fromString(QString str);
    static Unit fromIndex(int index);
    static int indexOf(Unit unit);
    static int indexOf(QString str);
    static bool isValid(int index) {
        if (index < INCH || index > MIC) return false;
        return true;
    }

    static const double MICRONTOSCREEN;
};



#endif // UNITSPINBOX_H

#endif // UNITS

