#include "units.h"

const double UnitConvert::MICRONTOSCREEN = 0.004;
/*!
 * \brief Convert external measurements to internal.
 *
 * All internal measurements are in \c microns. This will convert one
 * of the external measurements (\c inches, \c millimetres, \c mil and \c microns)
 * to \c microns.
 *
 * \param unit - the \c Unit type to convert.
 * \param value - the value to convert.
 * \return the value in microns.
 */
float UnitConvert::toInternal(Unit unit, float value) {
    switch (unit) {
    case INCH:
        return value * 25.4f * 1000.0f;
    case MM:
        return value * 1000.0f;
    case MIL:
        return (value * 25.4f);
    case MIC:
        return value;
    default:
        return 0.0f;
    }
}

QPointF UnitConvert::internalToScreen(QPointF value) {
    QPointF pt;
    pt.setX(value.x() * MICRONTOSCREEN);
    pt.setY(value.y() * MICRONTOSCREEN);
    return pt;
}

float UnitConvert::internalToScreen(float value) {
    return value * MICRONTOSCREEN;
}

float UnitConvert::screenToInternal(float value) {
  return value / MICRONTOSCREEN;
}

QPointF UnitConvert::screenToInternal(QPointF point) {
  QPointF converted;
  converted.setX(screenToInternal(point.x()));
  converted.setY(screenToInternal(point.y()));
  return converted;
}

/*!
 * \brief Convert an internal micron value to a \c Unit value.
 *
 * All internal measurements are in \c microns. This will convert an
 * internal measurement in \c microns to an external \c Unit value
 * (\c inches, \c millimetres, \c mil or \c microns).
 *
 * \param unit the \c Unit type to convert to
 * \param value to convert to unit value
 * \return the converted value
 */
float UnitConvert::fromInternal(Unit unit, float value) {
    switch (unit) {
    case INCH:
        return (value / 25.4f) / 1000.0f;
    case MM:
        return value / 1000.0f;
    case MIL:
        return (value / 25.4f);
    case MIC:
        return value;
    default:
        return 0.0f;
    }
    return value;
}

/*!
 * \brief Convert a value between two different \c Unit types.
 *
 * This will convert a \c Unit measurement in one type to the same value in
 * a different Unit type (\c inches, \c millimetres, \c mil or \c microns).
 *
 * \param unitIn the \c Unit type to convert from
 * \param unitOut the \c Unit type to convert to
 * \param valueIn the value to convert.
 * \return the converted value.
 */
float UnitConvert::betweenUnits(Unit unitIn, Unit unitOut, float valueIn) {
    float internalValue = toInternal(unitIn, valueIn);
    return fromInternal(unitOut, internalValue);
}

/*!
 * \brief Return a \c QStringList containing all valid values.
 *
 * \return the list of values.
 */
QStringList UnitConvert::strings() {
    QStringList list;
    list << "inch" << "mm" << "mil" << "mic";
    return list;
}

/*!
 * \brief Convert a \c Unit to a string representation.
 *
 * The \c Unit will be converted to a lower case string representation of the Unit.
 * If Unit::NONE is supplied an empty string is returned.
 *
 * \param unit the Unit to convert.
 * \return the converted \c QString value.
 */
QString UnitConvert::toString(Unit unit) {
    switch (unit) {
    case INCH:
        return "inch";
    case MM:
        return "mm";
    case MIL:
        return "mil";
    case MIC:
        return "mic";
    default:
        return QString();
    }
}

/*!
 * \brief Convert a string to it's \c Unit version.
 *
 * If the string is not one of the useable values it will return Unit::NONE.
 *
 * \param str the string to convert.
 * \return the resultant \c Unit.
 */
Unit UnitConvert::fromString(QString str) {
    if (str.toLower() == "inch") return INCH;
    else if (str.toLower() == "mm")return MM;
    else if (str.toLower() == "mil") return MIL;
    else if (str.toLower() == "mic") return MIC;
    return NONE;
}

/*!
 * \brief Convert an int index to its \c Unit version.
 *
 * If the int is not one of the useable values it will return Unit::NONE.
 *
 * \param str the string to convert.
 * \return the resultant \c Unit.
 */
Unit UnitConvert::fromIndex(int index) {
    if (UnitConvert::isValid(index))
        return static_cast<Unit>(index);
    return NONE;
}

/*!
 * \brief Convert a string to it's \c int index.
 *
 * If the string is not one of the useable values it will return -1.
 *
 * \param str the string to convert.
 * \return the resultant \c int value.
 */
int UnitConvert::indexOf(Unit unit) {
    return static_cast<int>(unit);
}

/*!
 * \brief Convert a string to it's \c int index.
 *
 * If the string is not one of the useable values it will return -1.
 *
 * \param str the string to convert.
 * \return the resultant \c int value.
 */
int UnitConvert::indexOf(QString str) {
    if (str.toLower() == "inch") return 0;
    else if (str.toLower() == "mm")return 1;
    else if (str.toLower() == "mil") return 2;
    else if (str.toLower() == "mic") return 3;
    return -1;
}

