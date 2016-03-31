#include "xmlschematicreader.h"

XmlSchematicReader::XmlSchematicReader(Schematic *schematic, QObject *parent) : QObject(parent) {
    m_schematic = schematic;
}

XmlSchematicReader::~XmlSchematicReader() {

}


