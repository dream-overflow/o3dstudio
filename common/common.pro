#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T20:10:47
#
#-------------------------------------------------

include(../o3dstudio.pri)

QT       += core gui widgets opengl

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
    src/plugin/pluginitem.cpp \
    src/plugin/pluginmodel.cpp \
    src/property/propertysectionnode.cpp \
    src/plugin/pluginsectionnode.cpp \
    src/ui/uicontroller.cpp \
    src/command/command.cpp \
    src/command/commandmanager.cpp \
    src/command/dummycommand.cpp \
    src/ui/canvas/canvascontent.cpp \
    src/ui/canvas/glcanvascontent.cpp \
    src/ui/qtrenderer.cpp \
    src/ui/canvas/o3dcanvascontent.cpp \
    src/workspace/masterscene.cpp \
    src/storage/store.cpp \
    src/storage/storeitem.cpp \
    src/workspace/projectfile.cpp \
    src/exception.cpp \
    src/workspace/projectitem.cpp \
    src/workspace/projectmodel.cpp \
    src/workspace/selection.cpp \
    src/workspace/selectionitem.cpp \
    src/workspace/hub.cpp \
    src/objectref.cpp \
    src/workspace/projectinfo.cpp \
    src/workspace/fragment.cpp \
    src/workspace/entity.cpp \
    src/component/component.cpp \
    src/component/componentregistry.cpp \
    src/typeregistry.cpp \
    src/command/fragment/addfragmentcommand.cpp \
    src/command/hub/addhubcommand.cpp \
    src/workspace/asset.cpp \
    src/ui/uiutils.cpp \
    src/command/hub/removehubcommand.cpp \
    src/command/fragment/removefragmentcommand.cpp \
    src/component/viewporthub.cpp \
    src/component/spacialnodehub.cpp \
    src/component/camerahub.cpp \
    src/component/meshhub.cpp \
    src/component/dummyhub.cpp \
    src/importer/importer.cpp \
    src/builder/builder.cpp \
    src/component/drawerhub.cpp \
    src/workspace/scenecommand.cpp \
    src/command/renameentitycommand.cpp \
    src/command/asset/addassetcommand.cpp \
    src/command/asset/removeassetcommand.cpp


HEADERS += \ 
    include/o3d/studio/common/module.h \
    include/o3d/studio/common/modulemanager.h \
    include/o3d/studio/common/workspace/workspace.h \
    include/o3d/studio/common/workspace/workspacemanager.h \
    include/o3d/studio/common/workspace/project.h \
    include/o3d/studio/common/capacity.h \
    include/o3d/studio/common/application.h \
    include/o3d/studio/common/dynamicmodule.h \
    include/o3d/studio/common/settings.h \
    include/o3d/studio/common/capacitysettings.h \
    include/o3d/studio/common/moduleinfo.h \
    include/o3d/studio/common/property/propertymodel.h \
    include/o3d/studio/common/property/propertyitem.h \
    include/o3d/studio/common/property/propertysection.h \
    include/o3d/studio/common/ui/toolbar.h \
    include/o3d/studio/common/ui/content.h \
    include/o3d/studio/common/ui/dock.h \
    include/o3d/studio/common/plugin/pluginitem.h \
    include/o3d/studio/common/plugin/pluginmodel.h \
    include/o3d/studio/common/plugin/pluginsection.h \
    include/o3d/studio/common/plugin/pluginsectionnode.h \
    include/o3d/studio/common/property/propertysectionnode.h \
    include/o3d/studio/common/ui/uicontroller.h \
    include/o3d/studio/common/element.h \
    include/o3d/studio/common/command/command.h \
    include/o3d/studio/common/command/commandmanager.h \
    include/o3d/studio/common/command/dummycommand.h \
    include/o3d/studio/common/ui/canvas/canvascontent.h \
    include/o3d/studio/common/ui/canvas/glcanvascontent.h \
    include/o3d/studio/common/ui/qtrenderer.h \
    include/o3d/studio/common/ui/canvas/o3dcanvascontent.h \
    include/o3d/studio/common/workspace/masterscene.h \
    include/o3d/studio/common/ui/canvas/o3ddrawer.h \
    include/o3d/studio/common/storage/store.h \
    include/o3d/studio/common/storage/storeitem.h \
    include/o3d/studio/common/workspace/projectfile.h \
    include/o3d/studio/common/exception.h \
    include/o3d/studio/common/workspace/projectitem.h \
    include/o3d/studio/common/workspace/projectmodel.h \
    include/o3d/studio/common/workspace/selection.h \
    include/o3d/studio/common/workspace/selectionitem.h \
    include/o3d/studio/common/workspace/hub.h \
    include/o3d/studio/common/objectref.h \
    include/o3d/studio/common/workspace/projectinfo.h \
    include/o3d/studio/common/global.h \
    include/o3d/studio/common/messenger.h \
    include/o3d/studio/common/workspace/fragment.h \
    include/o3d/studio/common/workspace/entity.h \
    include/o3d/studio/common/component/component.h \
    include/o3d/studio/common/component/componentregistry.h \
    include/o3d/studio/common/typeregistry.h \
    include/o3d/studio/common/command/fragment/addfragmentcommand.h \
    include/o3d/studio/common/command/hub/addhubcommand.h \
    include/o3d/studio/common/workspace/asset.h \
    include/o3d/studio/common/ui/uiutils.h \
    include/o3d/studio/common/command/hub/removehubcommand.h \
    include/o3d/studio/common/command/fragment/removefragmentcommand.h \
    include/o3d/studio/common/component/viewporthub.h \
    include/o3d/studio/common/component/spacialnodehub.h \
    include/o3d/studio/common/component/camerahub.h \
    include/o3d/studio/common/component/meshhub.h \
    include/o3d/studio/common/component/dummyhub.h \
    include/o3d/studio/common/importer/importer.h \
    include/o3d/studio/common/builder/builder.h \
    include/o3d/studio/common/component/drawerhub.h \
    include/o3d/studio/common/workspace/scenecommand.h \
    include/o3d/studio/common/ui/event.h \
    include/o3d/studio/common/ui/inputevent.h \
    include/o3d/studio/common/ui/keyevent.h \
    include/o3d/studio/common/ui/mouseevent.h \
    include/o3d/studio/common/ui/wheelevent.h \
    include/o3d/studio/common/command/renameentitycommand.h \
    include/o3d/studio/common/command/asset/addassetcommand.h \
    include/o3d/studio/common/command/asset/removeassetcommand.h

LIBS += -lobjective3d-dbg

# INCLUDEPATH += $$(PREFIX)/lib/objective3d-dbg/

copyToDestdir($$TRANSLATIONS_FILES, languages)

RESOURCES += \
    rc/common.qrc


FORMS +=


ts_target.files = $$TRANSLATIONS_FILES
ts_target.path = $$BUILD_PREFIX/share/o3dstudio/languages

# share_target.files = $$SHARE_FILES
# share_target.path = $$BUILD_PREFIX/share/o3dstudio/share

INSTALLS += ts_target  # share_target

DISTFILES += \
    languages/o3scommon_en.ts \
    languages/o3scommon_fr.ts
