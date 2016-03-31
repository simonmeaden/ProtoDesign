#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T12:57:52
#
#-------------------------------------------------
QT.protodesign.MAJOR_VERSION = 0
QT.protodesign.MINOR_VERSION = 1
QT.protodesign.PATCH_VERSION = 0

# The application version
VERSION = '$${QT.protodesign.MAJOR_VERSION}.$${QT.protodesign.MINOR_VERSION}.$${QT.protodesign.PATCH_VERSION}'
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProtoDesign
TEMPLATE = app

DEFINES *= QT_USE_QSTRINGBUILDER

CONFIG += debug_and_release
CONFIG += debug_and_release_target
CONFIG += build_all
#CONFIG += static # static library
#CONFIG += shared # shared dll library
CONFIG += c++11 # C++11 is the newest standard

DEFINES += OPEN_TEST_SCHEMATIC

CONFIG(debug, debug|release) {
    TARGET = protodesign
    DESTDIR = ../build/protodesign
    OBJECTS_DIR = $$DESTDIR/.objd
    MOC_DIR = $$DESTDIR/.mocd
    RCC_DIR = $$DESTDIR/.qrcd
    UI_DIR = $$DESTDIR/.uid
S
}

CONFIG(release, debug|release) {
    TARGET = Controller
    DESTDIR = ../build/protodesign
    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
    RCC_DIR = $$DESTDIR/.qrc
    UI_DIR = $$DESTDIR/.ui
}

SOURCES += \
    src/main.cpp\
    src/mainwindow.cpp \
    src/boards/stripboard.cpp \
    src/boards/prototypeboard.cpp \
    src/eaglecadschematic.cpp \
    src/boards/track.cpp \
    src/schematicframe.cpp \
    src/schematic.cpp \
    src/boards/stripboard_01pitch_60x35.cpp \
    src/optionswidgets.cpp \
    src/unitwidgets.cpp \
    src/librarymodel.cpp \
    src/display.cpp \
    src/componentlistmodel.cpp \
    src/units.cpp \
    src/object/shape.cpp \
    src/object/xyshape.cpp \
    src/object/minmaxshape.cpp \
    src/object/line.cpp \
    src/object/circle.cpp \
    src/object/arc.cpp \
    src/object/path.cpp \
    src/object/rectangle.cpp \
    src/object/text.cpp \
    src/object/picture.cpp \
    src/object/component.cpp \
    src/object/bus.cpp \
    src/object/pin.cpp \
    src/object/net.cpp \
    src/object/wire.cpp \
    src/detect.cpp \
    src/library/library.cpp \
    src/object/symbol.cpp \
    src/object/footprint.cpp \
    src/gschemsymbol.cpp \
    src/gschemschematic.cpp \
    src/object/grid.cpp \
    src/private/gschemschematic_p.cpp \
    src/private/gschemsymbol_p.cpp

HEADERS  += \
    src/mainwindow.h \
    src/boards/stripboard.h \
    src/boards/prototypeboard.h \
    src/eaglecadschematic.h \
    src/boards/track.h \
    src/schematicframe.h \
    src/schematic.h \
    src/boards/stripboard_01pitch_60x35.h \
    src/optionswidgets.h \
    src/unitwidgets.h \
    src/librarymodel.h \
    src/display.h \
    src/componentlistmodel.h \
    src/global.h \
    src/units.h \
    src/object/shape.h \
    src/object/xyshape.h \
    src/object/minmaxshape.h \
    src/object/line.h \
    src/object/circle.h \
    src/object/arc.h \
    src/object/path.h \
    src/object/rectangle.h \
    src/object/text.h \
    src/object/picture.h \
    src/object/component.h \
    src/object/bus.h \
    src/object/pin.h \
    src/object/net.h \
    src/object/wire.h \
    src/detect.h \
    src/library/library.h \
    src/object/symbol.h \
    src/object/footprint.h \
    src/gschemsymbol.h \
    src/gschemschematic.h \
    src/private/gschemsymbol_p.h \
    src/object/grid.h \
    src/private/gschemschematic_p.h

RESOURCES += \
    ProtoDesign.qrc

