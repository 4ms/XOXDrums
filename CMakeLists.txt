cmake_minimum_required(VERSION 3.22)

if(NOT "${METAMODULE_SDK_DIR}" STREQUAL "")
	message("METAMODULE_SDK_DIR set by CMake variable ${METAMODULE_SDK_DIR}")
elseif (DEFINED ENV{METAMODULE_SDK_DIR})
    set(METAMODULE_SDK_DIR "$ENV{METAMODULE_SDK_DIR}")
	message("METAMODULE_SDK_DIR set by environment variable ${METAMODULE_SDK_DIR}")
else()
    set(METAMODULE_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../metamodule-plugin-sdk")
	message("METAMODULE_SDK_DIR set to default: ${METAMODULE_SDK_DIR}")
endif()

include(${METAMODULE_SDK_DIR}/plugin.cmake)

project(CoreModule_MetaModulePlugin 
    VERSION     0.1
    DESCRIPTION "4ms Drums Plugin for MetaModule"
    LANGUAGES   C CXX ASM
)

add_library(4msDrums STATIC)


file(GLOB sources 
    src/modules/core/*.cc
    src/modules/core/*.cpp
)

target_sources(4msDrums
    PRIVATE
    ${sources}
    src/plugin-mm.cc
)

target_include_directories(4msDrums PRIVATE
    src/
    src/helpers/
    src/modules/
)

target_compile_options(4msDrums
    PRIVATE
)

set_property(TARGET 4msDrums PROPERTY CXX_STANDARD 20)

if("${INSTALL_DIR}" STREQUAL "")
    set(INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR}/metamodule-plugins)
endif()

create_plugin(
    SOURCE_LIB      4msDrums                                # This is the cmake target name (defined in add_target)
    PLUGIN_NAME     4msDrums                                # This must match the brand "slug" used in VCV Rack
    PLUGIN_JSON     ${CMAKE_CURRENT_LIST_DIR}/plugin.json   # Path to the plugin.json file used by VCV Rack
    SOURCE_ASSETS   ${CMAKE_CURRENT_LIST_DIR}/assets        # Path to the assets/ dir containing the PNGs
    DESTINATION     ${INSTALL_DIR}                          # Path to where you want the plugin file output
)

