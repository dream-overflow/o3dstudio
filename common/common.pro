#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T20:10:47
#
#-------------------------------------------------

include(../o3dstudio.pri)

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = o3scommon
TEMPLATE = lib
DESTDIR = $$O3S_BIN_DIR

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_SIGNALS_SLOTS_LOG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include

SOURCES += \ 
    src/module.cpp \
    src/modulemanager.cpp \
    src/workspace/workspace.cpp \
    src/workspace/workspacemanager.cpp \
    src/workspace/project.cpp \
    src/application.cpp \
    src/dynamicmodule.cpp \
    src/settings.cpp \
    src/moduleinfo.cpp \
    src/property/propertymodel.cpp \
    src/property/propertyitem.cpp \
    src/property/sectionnode.cpp


HEADERS += \ 
    include/common/module.h \
    include/common/modulemanager.h \
    include/common/workspace/workspace.h \
    include/common/workspace/workspacemanager.h \
    include/common/workspace/project.h \
    include/common/capacity.h \
    include/common/application.h \
    include/common/dynamicmodule.h \
    include/common/settings.h \
    include/common/capacity/capacitysettings.h \
    include/common/moduleinfo.h \
    include/common/property/propertymodel.h \
    include/common/property/propertyitem.h \
    include/common/property/propertysection.h \
    include/common/property/sectionnode.h \
    include/common/ui/toolbar.h \
    include/common/ui/content.h \
    include/common/ui/dock.h


RESOURCES +=


FORMS +=
