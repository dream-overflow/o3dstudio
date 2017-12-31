# - Try to find the Objective3D Studio libraries
# Once done this will define
#
#  OBJECTIVE3D_STUDIO_FOUND - System has Objective3D Studio
#  OBJECTIVE3D_STUDIO_INCLUDE_DIR - The Objective3D include directory
#  OBJECTIVE3D_STUDIO_LIBRARY_DIR - The Objective3D library directory
#  OBJECTIVE3D_STUDIO_LIBRARY - The Objective3D Studio common library
#  OBJECTIVE3D_STUDIO_PLUGIN_DIR - The Objective3D Studio plugin directory

# Copyright (c) 2017, DreamOverflow, <frederic.scherma@dreamoverflow.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (UNIX OR MINGW)
    include (CheckLibraryExists)

    set(O3D_STUDIO_DIR $ENV{O3D_STUDIO})

    if (O3D_STUDIO_DIR)
        set(VIRTUAL_ENV $ENV{VIRTUAL_ENV})
        set(OBJECTIVE3D_STUDIO_INCLUDE_DIR ${O3D_STUDIO_DIR}/common/include)

        if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(OBJECTIVE3D_STUDIO_LIBRARY_DIR ${O3D_STUDIO_DIR}/build-${VIRTUAL_ENV}-dbg/bin)
        elseif (${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
            set(OBJECTIVE3D_STUDIO_LIBRARY_DIR O3D_STUDIO_DIR/build-${VIRTUAL_ENV}-odbg/bin)
        elseif (${CMAKE_BUILD_TYPE} MATCHES "Release")
            set(OBJECTIVE3D_STUDIO_LIBRARY_DIR O3D_STUDIO_DIR/build-${VIRTUAL_ENV}/bin)
        endif ()

        set(OBJECTIVE3D_STUDIO_PLUGIN_DIR ${OBJECTIVE3D_STUDIO_LIBRARY_DIR}/plugins)
    else ()
        find_path(OBJECTIVE3D_STUDIO_INCLUDE_DIR o3d/studio)
        find_library(OBJECTIVE3D_LIBRARY NAMES o3scommon)
        find_path(OBJECTIVE3D_STUDIO_PLUGIN_DIR share/o3d/studio/plugins)
    endif ()

    get_filename_component(OBJECTIVE3D_LIBRARY_DIR ${OBJECTIVE3D_STUDIO_LIBRARY} DIRECTORY)

    mark_as_advanced(OBJECTIVE3D_STUDIO_INCLUDE_DIR OBJECTIVE3D_STUDIO_LIBRARY)

    if (OBJECTIVE3D_STUDIO_INCLUDE_DIR AND OBJECTIVE3D_STUDIO_LIBRARY)
        set(OBJECTIVE3D_STUDIO_FOUND TRUE)
    else ()
        set(OBJECTIVE3D_STUDIO_FOUND FALSE)
    endif ()

    if (OBJECTIVE3D_STUDIO_FOUND)
        if (NOT objective3d_FIND_QUIETLY)
            message(STATUS "Found Objective3D Studio: ${OBJECTIVE3D_STUDIO_LIBRARY}")
        endif (NOT objective3dstudio_FIND_QUIETLY)
    else (OBJECTIVE3D_STUDIO_FOUND)
        if (objective3dstudio_FIND_REQUIRED)
            message("*****************************************************")
            message_color(${TextError} "Error : Objective3D Studio library not found")
            message("*****************************************************")
            message(FATAL_ERROR "Could NOT find Objective3D Studio library")
        endif (objective3dstudio_FIND_REQUIRED)
        if (NOT objective3dstudio_FIND_QUITELY)
            message_color(${TextWarning} "Could NOT find Objective3D Studio library")
        endif (NOT objective3d_FIND_QUITELY)
    endif (OBJECTIVE3D_STUDIO_FOUND)
endif (UNIX OR MINGW)
