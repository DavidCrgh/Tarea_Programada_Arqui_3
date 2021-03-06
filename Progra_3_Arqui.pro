#-------------------------------------------------
#
# Project created by QtCreator 2017-06-05T12:18:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Progra_3_Arqui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_EXTRA_COMPILERS += nasm
NASMEXTRAFLAGS = -f elf64 -g -F dwarf
OTHER_FILES += $$NASM_SOURCES
nasm.output = ${QMAKE_FILE_BASE}.o
nasm.commands = nasm $$NASMEXTRAFLAGS -o ${QMAKE_FILE_BASE}.o ${QMAKE_FILE_NAME}
nasm.input = NASM_SOURCES

NASM_SOURCES += multiplicacion.asm

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        ventanaprincipal.cpp \
    ventanagenerar.cpp \
    datosmatriz.cpp \
    hilogenerar.cpp \
    hilosumar.cpp \
    hiloescalar.cpp \
    hilomultiplicar.cpp \
    hilotransponer.cpp \
    hilotipo.cpp

HEADERS += \
        ventanaprincipal.h \
    ventanagenerar.h \
    datosmatriz.h \
    hilogenerar.h \
    hilosumar.h \
    hiloescalar.h \
    hilomultiplicar.h \
    multiplicacion_asm.h \
    hilotransponer.h \
    hilotipo.h

FORMS += \
        ventanaprincipal.ui \
    ventanagenerar.ui

DISTFILES += \
    multiplicacion.asm
