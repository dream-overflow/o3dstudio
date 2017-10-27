#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T20:10:47
#
#-------------------------------------------------

include(../o3dstudio.pri)

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = o3dstudio
TEMPLATE = app
DESTDIR = $$O3S_BIN_DIR  # $$IDE_APP_PATH

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_SIGNALS_SLOTS_LOG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/preferencesdialog.cpp \
    src/pluginsinformationsdialog.cpp \
    src/manageworkspacesdialog.cpp \
    src/newprojectdialog.cpp \
    src/maintoolbar.cpp \
    src/property/displaysection.cpp \
    src/quicktoolbar.cpp


HEADERS += \
    src/mainwindow.h \
    src/settings.h \
    src/preferencesdialog.h \
    src/pluginsinformationsdialog.h \
    src/manageworkspacesdialog.h \
    src/newprojectdialog.h \
    src/maintoolbar.h \
    src/property/displaysection.h \
    src/quicktoolbar.h


RESOURCES += \
    rc/mainresources.qrc

FORMS += \
    ui/mainwindow.ui \
    ui/preferencesdialog.ui \
    ui/displaysection.ui \
    ui/pluginsinformationsdialog.ui \
    ui/aboutdialog.ui \
    ui/systeminfodialog.ui \
    ui/manageworkspacesdialog.ui \
    ui/newprojectdialog.ui

LIBS += -L$$O3S_BIN_DIR -lo3scommon

#DISTFILES += \
#    languages/o3smain_fr.ts \
#    languages/o3smain_en.ts

SHARE_FILES = \
    share/*

ts_target.files = $$TRANSLATIONS_FILES
ts_target.path = $$O3S_BIN_DIR/languages

share_target.files = $$SHARE_FILES
share_target.path = $$O3S_BIN_DIR/share

INSTALLS += ts_target share_target
