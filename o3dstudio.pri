!isEmpty(O3DSTUDIO_PRI_INCLUDED):error("qtcreator.pri already included")
O3DSTUDIO_PRI_INCLUDED = 1

#
# Environment and compiler switch
#

O3S_BUILD_ENV = $$getenv(VIRTUAL_ENV)
isEmpty($$O3S_BUILD_ENV) {
    O3S_BUILD_ENV = "amd64"
}

BUILD_PREFIX = $$getenv(PREFIX)
isEmpty(BUILD_PREFIX) {
    linux: BUILD_PREFIX = /usr/local
    macx: BUILD_PREFIX = /usr/local
}

ENV_CC = $$getenv(CC)
!isEmpty($$ENV_CC) {
    QMAKE_CC = $$ENV_CC
}

ENV_CXX = $$getenv(CXX)
!isEmpty($$ENV_CXX) {
    QMAKE_CXX = $$ENV_CXX
}

ENV_AR = $$getenv(AR)
!isEmpty($$ENV_AR) {
    QMAKE_AR_CMD = $$ENV_AR
}

#ENV_LD = $$getenv(LD)
#!isEmpty($$ENV_LD) {
#    QMAKE_LINK = $$ENV_LD
#}

#
# Global
#

O3DSTUDIO_VERSION = 0.1
VERSION = $$O3DSTUDIO_VERSION
O3DSTUDIO_COPYRIGHT_YEAR = 2017
BINARY_ARTIFACTS_BRANCH = master

O3S_BIN_DIR = $$join($$list($$top_builddir, "bin"), "/")

CONFIG(debug, debug|release) {
#    O3S_BUILD_TARGET_MODE = "-dbg"
} else {
#    CONFIG(profile, profile|release) {
#        O3S_BUILD_TARGET_MODE = "-pro"
#    } else {
#        O3S_BUILD_TARGET_MODE = ""
#    }
}

# O3S_BIN_DIR = $$absolute_path($$join($$list($$PWD, "..", "build-o3dstudio-$$O3S_BUILD_ENV$$O3S_BUILD_TARGET_MODE", "bin"), "/"))

CONFIG += c++14  # declarative_debug

QMAKE_CXXFLAGS += -fno-rtti
QMAKE_CXXFLAGS_RELEASE += -O2 -DNDEBUG -ffunction-sections  # -fvisibility=hidden

# @todo msvc
*-clang++* {
    QMAKE_CXXFLAGS_DEBUG += -O0 -D_DEBUG -glldb
}
*-g++* {
    QMAKE_CXXFLAGS_DEBUG += -O0 -D_DEBUG -ggdb
}

darwin:!minQtVersion(5, 7, 0) {
    # Qt 5.6 still sets deployment target 10.7, which does not work
    # with all C++11/14 features (e.g. std::future)
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}

osx {

} else {
	
}

defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

macx: PLATFORM = "mac"
else:win32: PLATFORM = "windows"
else:linux-*: PLATFORM = "linux-$${ARCHITECTURE}"
else: PLATFORM = "unknown"

macx:INSTALLER_NAME = "o3d-studio-$${O3DSTUDIO_VERSION}"
else:INSTALLER_NAME = "$${BASENAME}"

TARGETPATH = $$shell_path($$_PRO_FILE_PWD_)
INCLUDEPATH += $$top_srcdir/common/include

!isEmpty(O3S_PLUGIN_DEPENDS) {
#    LIBS *= -L$$STUDIO_PLUGIN_PATH  # plugin path from output directory
#    LIBS *= -L$$LINK_PLUGIN_PATH    # when output path is different from build directory
}

LANGUAGES = en fr

TRANSLATIONS = $$files($$TARGETPATH/languages/*.ts)
TRANSLATION_TARGET_DIR = $$O3S_BIN_DIR/languages

TRANSLATIONS_FILES =

qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    qmfile = $$shadowed($$tsfile)
    qmfile ~= s,.ts$,.qm,
    qmdir = $$dirname(qmfile)

    !exists($$qmdir) {
        mkpath($$qmdir)|error("Aborting.")
    }

    command = $$LRELEASE -removeidentical $$tsfile -qm $$qmfile
    system($$command)|error("Failed to run: $$command")
    TRANSLATIONS_FILES += $$qmfile
}

# Copies the given files to the destination directory
defineTest(copyToDestdir) {
    files = $$1
    PATH_TO = $$2

    QMAKE_POST_LINK += $$QMAKE_MKDIR $$quote($$DESTDIR/$$PATH_TO/) $$escape_expand(\\n\\t)

    for(FILE, files) {
        DDIR = $$DESTDIR

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY -r $$quote($$FILE) $$quote($$DDIR/$$PATH_TO/) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}

# o3d in debug @todo for both
INCLUDEPATH += $$(PREFIX)/lib/objective3d-dbg/

DISTFILES += \
    $$PWD/cmake/FindObjective3DStudio.cmake
