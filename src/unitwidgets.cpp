#include "unitwidgets.h"

/*!
 * \brief Constructor for the \c UnitComboBox class;
 *
 * Constructs a UnitComboBox containing the possible \c Unit values. The addItem() methods
 * from QComboBox are disabled so that no extra non-allowed values may be added.
 *
 * \param parent parent widget.
 */
UnitComboBox::UnitComboBox(QWidget *parent) : QComboBox(parent) {
    QComboBox::addItems(UnitConvert::strings());
    m_unit = INCH;
    setCurrentIndex(0);
//    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(indexChanged(QString)));
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

/*
 * Called internally whenever the selected unit is changed.
 *
 * param unit the new unit index.
 */
void UnitComboBox::indexChanged(int index) {
    emit unitHasChanged(UnitConvert::fromIndex(index));
}

/*
 * Called internally whenever the selected unit is changed.
 *
 * param unit the new unit string.
 */
void UnitComboBox::indexChanged(QString unit) {
    emit unitHasChanged(UnitConvert::fromString(unit));
}

void UnitComboBox::addItem(const QIcon &icon, const QString &text, const QVariant &userData) {
    Q_UNUSED(icon)
    Q_UNUSED(text)
    Q_UNUSED(userData)
}

void UnitComboBox::addItem(const QString &text, const QVariant &userData) {
    Q_UNUSED(text)
    Q_UNUSED(userData)
}

void UnitComboBox::addItems(const QStringList &texts) {
    Q_UNUSED(texts)
}

void UnitComboBox::setCurrentIndex(int index) {
    if (UnitConvert::isValid(index)) {
        QComboBox::setCurrentIndex(index);
        emit unitHasChanged(UnitConvert::fromIndex(index));
    }
}

/*!
 * \brief Constructor for the \c UnitSpinBox class.
 *
 * Allows the user to select various values with a specified \c Unit type.
 * The internal value is always in microns, not whatever unit is specified.
 * If the setUnit(Unit) method or the unitChanged(Unit) slots are called
 * the value will be changed to match the internally stored value as converted
 * to the new \c Unit type.
 *
 * The spin box can be linked to a \c UnitComboBox by connecting the
 * combobox unitHasChanged(Unit) signal with the spin box unitChanged(Unit)
 * slot.
 *
 * \code
 * connect(combobox, SIGNAL(unitHasChanged(Unit)), spinbox, SLOT(unitChanged(Unit)));
 * \endcode
 *
 * \param parent the parent widget.
 */
UnitSpinBox::UnitSpinBox(QWidget *parent) : QDoubleSpinBox(parent) {
    m_unit = INCH;
    m_value = 0.0;

    setBoxState(m_unit);
    setValue(m_value);
    setMaximum(999999.9);

    connect(this, SIGNAL(valueChanged(float)), this, SLOT(valueHasBeenChanged(float)));
}

/*!
 * \brief Sets the displayed and internal values appropriately, depending on the set \c Unit type.
 *
 * \param value the new display value.
 */
void UnitSpinBox::setValue(float value) {
    m_value = UnitConvert::toInternal(m_unit, value);
    QDoubleSpinBox::setValue(value);
    emit valueHasChanged(m_value);
}

/*!
 * \brief changes the \c Unit type.
 *
 * This will cause the displayed value to change to the correct value, depending on
 * the value of the new \c Unit and the internally stored value.
 *
 * \param unit the new Unit type.
 */
void UnitSpinBox::unitChanged(Unit unit) {
    m_unit = unit;
    float value = UnitConvert::fromInternal(m_unit, m_value);
    QDoubleSpinBox::setValue(value);
//    setBoxState(m_unit);
}

void UnitSpinBox::setBoxState(Unit unit) {
    switch (unit) {
    case INCH:
        setMinimum(0.0001);
        setSingleStep(1);
        setDecimals(4);
        break;
    case MM:
        setMinimum(0.0001);
        setSingleStep(1);
        setDecimals(4);
        break;
    case MIC:
        setMinimum(0.1);
        setSingleStep(1);
        setDecimals(0);
        break;
    case MIL:
        setMinimum(0.1);
        setSingleStep(1);
        setDecimals(0);
        break;
    default:
        break;
    }
}

/*
 * Internally used slot. Called by changes in the value.
 */
void UnitSpinBox::valueHasBeenChanged(float value) {
    m_value = UnitConvert::toInternal(m_unit, value);
    emit valueHasChanged(m_value);
}

