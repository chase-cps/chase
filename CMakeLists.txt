###############################################################################
# Main CMake file for the Chase project
###############################################################################
# @author Michele Lora
#

cmake_minimum_required (VERSION 2.4)

project (Chase)


# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
# Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set( DEFAULT_INSTALL_LOCATION chase-${CMAKE_BUILD_TYPE} )

option(ENABLE_INSTALL_PREFIX "Install build targets to system (path given by '-DCMAKE_INSTALL_PREFIX' or '${CMAKE_INSTALL_PREFIX}' if not specified)." ON)
if ( ENABLE_INSTALL_PREFIX )
        set (CMAKE_INSTALL_PREFIX ${DEFAULT_INSTALL_LOCATION} CACHE PATH "Installation root")
else()
        set (CMAKE_INSTALL_PREFIX ${DEFAULT_INSTALL_LOCATION} CACHE PATH "Installation root" FORCE)
endif()

add_subdirectory(src)
