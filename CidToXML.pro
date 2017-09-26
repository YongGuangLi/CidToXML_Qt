#-------------------------------------------------
#
# Project created by QtCreator 2017-07-14T09:35:58
#
#-------------------------------------------------

QT       += widgets xml

TARGET = CidToXML
TEMPLATE = lib

DEFINES += CIDTOXML_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = D:\QtProjects\CidToXML

SOURCES += \
        cidtoxml.cpp \
    cidtoxml.cpp \
    scd.cpp \
    scd_communication.cpp \
    scd_datatypetemplates.cpp \
    scd_ied.cpp \


HEADERS += \
        cidtoxml.h \
        cidtoxml_global.h \ 
    include/ICidToData.h \
    cidtoxml.h \
    cidtoxml_global.h \
    dtxy_scd.h \
    scd.h \ 
    scd_communication.h \
    scd_datatypetemplates.h \
    scd_ied.h \


unix {
    target.path = /usr/lib
    INSTALLS += target
}
