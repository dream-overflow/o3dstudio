#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T20:10:47
#
#-------------------------------------------------

include(../o3dstudio.pri)

QT       += core gui widgets opengl

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
    src/quicktoolbar.cpp \
    src/plugin/pluginsection.cpp \
    src/content/browsercontent.cpp \
    src/dock/mainconsole.cpp \
    src/dock/workspacedock.cpp \
    src/property/workspacesection.cpp \
    src/property/projectsection.cpp \
    src/dock/propertydock.cpp \
    src/messenger.cpp \
    src/toolbar/hubcomponenttoolbar.cpp \
    src/importfiledialog.cpp \
    src/dock/selectiondock.cpp \
    src/toolbar/transformtoolbar.cpp


HEADERS += \
    src/mainwindow.h \
    src/settings.h \
    src/preferencesdialog.h \
    src/pluginsinformationsdialog.h \
    src/manageworkspacesdialog.h \
    src/newprojectdialog.h \
    src/maintoolbar.h \
    src/property/displaysection.h \
    src/quicktoolbar.h \
    src/plugin/pluginsection.h \
    src/content/browsercontent.h \
    src/dock/mainconsole.h \
    src/dock/workspacedock.h \
    src/property/workspacesection.h \
    src/property/projectsection.h \
    src/dock/propertydock.h \
    src/toolbar/hubcomponenttoolbar.h \
    src/importfiledialog.h \
    src/dock/selectiondock.h \
    src/toolbar/transformtoolbar.h


RESOURCES += \
    rc/mainresources.qrc \
    rc/darkorangetheme.qrc \
    rc/darktheme.qrc

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
LIBS += -lobjective3d-dbg

INCLUDEPATH += $$(PREFIX)/lib/objective3d-dbg/

SHARE_FILES = $$files($$TARGETPATH/share/*)
# SHARE_FILES = \
#    share/*

copyToDestdir($$TRANSLATIONS_FILES, languages)
copyToDestdir($$SHARE_FILES, share)

ts_target.files = $$TRANSLATIONS_FILES
ts_target.path = $$BUILD_PREFIX/share/o3dstudio/languages

share_target.files = $$SHARE_FILES
share_target.path = $$BUILD_PREFIX/share/o3dstudio/share

INSTALLS += ts_target share_target

DISTFILES += \
    languages/o3smain_en.ts \
    languages/o3smain_fr.ts
