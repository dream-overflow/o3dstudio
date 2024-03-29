#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T20:10:47
#
#-------------------------------------------------

include(../../o3dstudio.pri)

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = o3sdummy
TEMPLATE = lib
DESTDIR = $$join($$list($$O3S_BIN_DIR, "plugins"), "/")

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
        src/dummy.cpp

HEADERS += \
        src/dummy.h

RESOURCES +=

FORMS +=

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/release/ -lo3scommon
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/debug/ -lo3scommon
#else:unix: LIBS += -L$$OUT_PWD/../../common/ -lo3scommon

LIBS += -L$$O3S_BIN_DIR -lo3scommon

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common/include
