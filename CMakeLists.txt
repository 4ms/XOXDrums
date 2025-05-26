cmake_minimum_required(VERSION 3.22)

if(NOT "${METAMODULE_SDK_DIR}" STREQUAL "")
	message("METAMODULE_SDK_DIR set by CMake variable ${METAMODULE_SDK_DIR}")
elseif (DEFINED ENV{METAMODULE_SDK_DIR})
    set(METAMODULE_SDK_DIR "$ENV{METAMODULE_SDK_DIR}")
	message("METAMODULE_SDK_DIR set by environment variable ${METAMODULE_SDK_DIR}")
else()
    set(METAMODULE_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/metamodule-plugin-sdk")
	message("METAMODULE_SDK_DIR set to default: ${METAMODULE_SDK_DIR}")
endif()

include(${METAMODULE_SDK_DIR}/plugin.cmake)

project(XOXDrumsMetaModulePlugin 
    VERSION     0.1
    DESCRIPTION "XOX Drums Plugin for MetaModule"
    LANGUAGES   C CXX ASM
)

add_library(XOXDrums STATIC)


# Note: there aren't any files here presently
file(GLOB sources 
    src/modules/core/*.cc
    src/modules/core/*.cpp
)

target_sources(XOXDrums
    PRIVATE
    ${sources}
    src/plugin-mm.cc
)

target_include_directories(XOXDrums PRIVATE
    src/
    src/helpers/
    src/modules/
)

target_compile_options(XOXDrums
    PRIVATE
)

set_property(TARGET XOXDrums PROPERTY CXX_STANDARD 20)

if("${MM_INSTALL_DIR}" STREQUAL "")
    set(MM_INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR}/metamodule-plugins)
endif()

create_plugin(
    SOURCE_LIB      XOXDrums
    PLUGIN_NAME     4ms-XOXDrums
    PLUGIN_JSON     ${CMAKE_CURRENT_LIST_DIR}/plugin.json
    SOURCE_ASSETS   ${CMAKE_CURRENT_LIST_DIR}/assets
    DESTINATION     ${MM_INSTALL_DIR}
)

