TEMPLATE      = lib
CONFIG       += designer plugin release
HEADERS       = ../PWidget/iconeditor.h \
                iconeditorplugin.h
SOURCES       = ../PWidget/iconeditor.cpp \
                iconeditorplugin.cpp
RESOURCES     = iconeditorplugin.qrc
DESTDIR       = $$[QT_INSTALL_PLUGINS]/designer
