include(o3dstudio.pri)

#version check qt
!minQtVersion(5, 6, 2) {
    message("Cannot build Qt Creator with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.6.2.")
}

include(doc/doc.pri)

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = common main plugins
unix:!macx:!isEmpty(copydata):SUBDIRS += bin
!isEmpty(BUILD_TESTS):SUBDIRS += tests

DISTFILES +=
