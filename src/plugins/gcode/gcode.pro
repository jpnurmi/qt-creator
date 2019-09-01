DEFINES += GCODE_LIBRARY
QT += 3dcore 3drender

include(../../qtcreatorplugin.pri)

HEADERS += \
    gcode.h \
    gcodeautocompleter.h \
    gcodecompletionassist.h \
    gcodeconstants.h \
    gcodedocument.h \
    gcodeeditor.h \
    gcodemesh.h \
    gcodeoutline.h \
    gcodeparser.h \
    gcodeplugin.h \
    gcodereader.h \
    gcodethread.h

SOURCES += \
    gcode.cpp \
    gcodeautocompleter.cpp \
    gcodecompletionassist.cpp \
    gcodedocument.cpp \
    gcodeeditor.cpp \
    gcodemesh.cpp \
    gcodeoutline.cpp \
    gcodeparser.cpp \
    gcodeplugin.cpp \
    gcodereader.cpp \
    gcodethread.cpp
