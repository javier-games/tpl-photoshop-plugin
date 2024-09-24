cmake_minimum_required(VERSION 3.16)

# Variable

project(
        "PluginTemplate"
        VERSION 1.0.0 DESCRIPTION
        "Template for a Photoshop Plugin."
)

set(PLUGIN_NAME ${CMAKE_PROJECT_NAME})
set(PLUGIN_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(PLUGIN_CATEGORY "Template")
set(UUID_NAMESPACE "2799b176-669e-402b-b96e-5d13d2b5028b")
set(PS_SDK_VERSION "adobe_photoshop_sdk_2024_win_v1")

# Project

string(
        UUID PLUGIN_UUID
        NAMESPACE ${UUID_NAMESPACE}
        NAME ${CMAKE_PROJECT_NAME}
        TYPE SHA1
)

set(SOURCE_FILES "src/define.h" "src/main.cpp")
set(PIPL_FILE "src/pipl.r" )

# Output

string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_NAME_LOWER)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" SYSTEM_ARCH_LOWER)
set(BUILD_SUB_DIR "${SYSTEM_NAME_LOWER}/${SYSTEM_ARCH_LOWER}")
set(TEMP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_SUB_DIR}/temp")
set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_SUB_DIR}")

# Dependencies

if (WIN32)
    set(PS_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${PS_SDK_VERSION}/pluginsdk")
    set(CNVTPIPL_PATH "${PS_SDK_PATH}/samplecode/resources/Cnvtpipl.exe")
endif ()

set( PS_SDK_PATHS "${CMAKE_CURRENT_SOURCE_DIR}"
        "${PS_SDK_PATH}/samplecode/common/includes"
        "${PS_SDK_PATH}/samplecode/common/sources"
        "${PS_SDK_PATH}/samplecode/common/resources"
        "${PS_SDK_PATH}/photoshopapi/photoshop"
        "${PS_SDK_PATH}/photoshopapi/pica_sp"
)

include_directories(${PS_SDK_PATHS})

# Flag Push

set(CXX_FLAGS_BACKUP "${CMAKE_CXX_FLAGS}")
set(CXX_FLAGS_DEBUG_BACKUP "${CMAKE_CXX_FLAGS_DEBUG}")
set(CXX_FLAGS_RELEASE_BACKUP "${CMAKE_CXX_FLAGS_RELEASE}")
set(INCLUDE_DIRECTORIES_BACKUP ${INCLUDE_DIRECTORIES})
set(SHARED_LINKER_FLAGS_DEBUG_BACKUP "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
set(SHARED_LINKER_FLAGS_RELEASE_BACKUP "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")

# Fix Windows Paths

string(REPLACE "/" "\\" MS_TEMP_PATH "${TEMP_PATH}")
string(REPLACE "/" "\\" MS_PIPL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${PIPL_FILE}")

# Notice

message( NOTICE "Project Settings")
message( NOTICE "Name        | ${PLUGIN_NAME}")
message( NOTICE "Description | ${PLUGIN_DESCRIPTION}")
message( NOTICE "UUID        | ${PLUGIN_UUID}")
message( NOTICE )

set(CMAKE_CXX_STANDARD 11)

if(WIN32)

    set(PLUGIN_OUTPUT_SUFFIX ".8bf")

    set(MS_SDK_PATHS "/I \"${PS_SDK_PATH}/samplecode/common/resources\" \
        /I \"${PS_SDK_PATH}/samplecode/common/includes\" \
        /I \"${PS_SDK_PATH}/samplecode/common/sources\" \
        /I \"${PS_SDK_PATH}/photoshopapi/photoshop\" \
        /I \"${PS_SDK_PATH}/photoshopapi/pica_sp\"")

    add_compile_options("${CMAKE_CXX_FLAGS} /nologo /WX /EHsc /Zc:__cplusplus /arch:AVX2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DNDEBUG")

    set(EX_LIB_LINKER_FLAGS "/defaultlib:User32.lib /defaultlib:Kernel32.lib /defaultlib:Shell32.lib /nodefaultlib:LIBCMTD.lib /nodefaultlib:LIBCMT.lib")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /incremental:no ${EX_LIB_LINKER_FLAGS} \"${TEMP_PATH}/pipl.res\" /nologo")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /opt:noref")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /opt:ref")

    if(!MSVC OR MSVC_VERSION LESS 1910 OR MSVC_VERSION GREATER 1919)
        message( STATUS "MSVC v141 is required for building the project. Other compilers may not work properly." )
    endif()

    add_definitions(/D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /DISOLATION_AWARE_ENABLED=1 /DWIN32=1
            /D_WINDOWS /D_USRDLL /D_WINDLL /D_MBCS)

    #You may also need CMAKE_BUILD_TYPE.

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(PH_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ")
    else()
        set(PH_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ")
    endif()

    set(PiPL_DEFINE "/DPLUGIN_NAME=\\\"${PLUGIN_NAME}\\\" /DDESCRIPTION=\"\\\"${PLUGIN_DESCRIPTION}\\\"\" /DUUID=\\\"${PLUGIN_UUID}\\\" /DCATEGORY=\"\\\"${PLUGIN_CATEGORY}\\\"\"")

    add_custom_target(PiPL ALL
            BYPRODUCTS ${TEMP_PATH}/pipl.rr ${TEMP_PATH}/pipl.rc ${TEMP_PATH}/pipl.res
            COMMENT "Build PiPL..."
            SOURCES ${PIPL_FILE}
            COMMAND if not exist "${MS_TEMP_PATH}" mkdir "${MS_TEMP_PATH}"
            COMMAND ${CMAKE_CXX_COMPILER} ${PH_FLAGS} ${MS_SDK_PATHS} /EP /DMSWindows=1 ${PiPL_DEFINE} /Tc ${MS_PIPL_FILE} > "${TEMP_PATH}/pipl.rr"
            COMMAND echo .rr done (%errorlevel%)
            COMMAND ${CNVTPIPL_PATH} "${TEMP_PATH}/pipl.rr" "${TEMP_PATH}/pipl.rc"
            COMMAND echo .rc done (%errorlevel%)
            COMMAND rc /v /fo "${TEMP_PATH}/pipl.res" "${TEMP_PATH}/pipl.rc"
            COMMAND echo .res done (%errorlevel%)
    )

    # Build src
    # src -> .obj, .res -> PLUGIN_OUTPUT_SUFFIX( ex .8bp)

    add_library(${PLUGIN_NAME} SHARED ${SOURCE_FILES})
    add_dependencies(${PLUGIN_NAME} PiPL)

    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            "${OUTPUT_PATH}/${PLUGIN_NAME}.ilk"
            "${OUTPUT_PATH}/${PLUGIN_NAME}.exp")

    set_target_properties(${PLUGIN_NAME}
            PROPERTIES
            PREFIX ""
            OUTPUT_NAME ${PLUGIN_NAME}
            SUFFIX "${PLUGIN_OUTPUT_SUFFIX}"
            ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}
            LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}
            RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}
    )

endif()

# Flag Pop

set(CMAKE_CXX_FLAGS "${CXX_FLAGS_BACKUP}")
set(CMAKE_CXX_FLAGS_DEBUG "${CXX_FLAGS_BACKUP}")
set(CMAKE_CXX_FLAGS_RELEASE "${CXX_FLAGS_BACKUP}")
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES_BACKUP})
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${SHARED_LINKER_FLAGS_DEBUG_BACKUP}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${SHARED_LINKER_FLAGS_RELEASE_BACKUP}")
