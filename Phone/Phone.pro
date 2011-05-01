#-------------------------------------------------
#
# Project created by QtCreator 2011-04-20T10:53:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = Phone
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    udp.cpp \
    videodevice.cpp \
    utils.c \
    color.c

HEADERS  += dialog.h \
    udp.h \
    videodevice.h \
    utils.h \
    my-types.h \
    huffman.h \
    color.h

FORMS    += dialog.ui

OTHER_FILES +=
