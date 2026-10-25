###############################################################################
# Superbuild of CHASE.
#
# Configures, builds, and installs the CHASE modules (git submodules in the
# modules/ directory) in the required order:
#   third_party -> core-library -> chase-backends -> DSL_tool, logics_tool
#
# All the modules are installed in the same prefix, which is the value of
# CMAKE_INSTALL_PREFIX if given, or ${CMAKE_BINARY_DIR}/install otherwise.
# The installation is performed during the build, i.e., by:
#   cmake -S . -B build [-DCMAKE_INSTALL_PREFIX=<prefix>]
#   cmake --build build
###############################################################################
cmake_minimum_required(VERSION 3.18)
project(CHASE_SUPERBUILD NONE)

include(ExternalProject)
include(ProcessorCount)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

option(CHASE_BUILD_TESTS "Build the tests of the CHASE core library" OFF)

# Installation prefix of the modules. When building the Python wheel
# (scikit-build-core sets SKBUILD), the modules are staged in the build
# directory and only the Python modules go into the wheel (see the end).
if(SKBUILD)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage CACHE PATH
        "Installation prefix of the CHASE modules" FORCE)
elseif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH
        "Installation prefix of the CHASE modules" FORCE)
endif()
get_filename_component(CHASE_PREFIX "${CMAKE_INSTALL_PREFIX}" ABSOLUTE
    BASE_DIR "${CMAKE_BINARY_DIR}")
message(STATUS "CHASE installation prefix: ${CHASE_PREFIX}")

ProcessorCount(_nproc)
if(_nproc EQUAL 0)
    set(_nproc 1)
endif()
set(CHASE_BUILD_JOBS ${_nproc} CACHE STRING
    "Number of parallel jobs used to build each module")

# Every module reads its installation prefix from the environment variable
# CHASE_INSTALL_DIR (it ignores -DCMAKE_INSTALL_PREFIX). The variable is also
# set for the build and install steps, since a module may re-run its own
# configuration during the build.
set(_env ${CMAKE_COMMAND} -E env CHASE_INSTALL_DIR=${CHASE_PREFIX})

# Python interpreter: Python_EXECUTABLE is used by FindPython, PYTHON_EXECUTABLE
# by the older FindPythonInterp/FindPythonLibs. Either one can be given, and
# both are forwarded to the modules.
if(PYTHON_EXECUTABLE AND NOT Python_EXECUTABLE)
    set(Python_EXECUTABLE ${PYTHON_EXECUTABLE})
elseif(Python_EXECUTABLE AND NOT PYTHON_EXECUTABLE)
    set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
endif()

# pybind11 installed with pip (e.g., in the isolated build of the wheel).
if(NOT pybind11_DIR AND Python_EXECUTABLE)
    execute_process(COMMAND ${Python_EXECUTABLE} -m pybind11 --cmakedir
        OUTPUT_VARIABLE _pybind11_dir OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE _pybind11_found ERROR_QUIET)
    if(_pybind11_found EQUAL 0 AND EXISTS "${_pybind11_dir}")
        set(pybind11_DIR ${_pybind11_dir})
    endif()
endif()

# Options forwarded to every module. Lists are passed with '|' as separator.
set(_args -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
foreach(_var pybind11_DIR CMAKE_PREFIX_PATH Python_EXECUTABLE PYTHON_EXECUTABLE)
    if(${_var})
        string(REPLACE ";" "|" _value "${${_var}}")
        list(APPEND _args "-D${_var}=${_value}")
    endif()
endforeach()

set(_generator -G ${CMAKE_GENERATOR})
if(CMAKE_GENERATOR_PLATFORM)
    list(APPEND _generator -A ${CMAKE_GENERATOR_PLATFORM})
endif()
if(CMAKE_GENERATOR_TOOLSET)
    list(APPEND _generator -T ${CMAKE_GENERATOR_TOOLSET})
endif()

# chase_module(<name> [DEPENDS <targets>...] [ARGS <cmake args>...])
function(chase_module name)
    cmake_parse_arguments(M "" "" "DEPENDS;ARGS" ${ARGN})
    set(src ${PROJECT_SOURCE_DIR}/modules/${name})
    if(NOT EXISTS ${src}/CMakeLists.txt)
        message(FATAL_ERROR "Module ${name} not found in ${src}. "
            "Run: git submodule update --init --recursive")
    endif()
    ExternalProject_Add(${name}
        SOURCE_DIR ${src}
        BINARY_DIR ${CMAKE_BINARY_DIR}/${name}
        DEPENDS ${M_DEPENDS}
        LIST_SEPARATOR |
        CONFIGURE_COMMAND ${_env} ${CMAKE_COMMAND} ${_generator}
            ${_args} ${M_ARGS} -S <SOURCE_DIR> -B <BINARY_DIR>
        BUILD_COMMAND ${_env} ${CMAKE_COMMAND} --build <BINARY_DIR>
            --config ${CMAKE_BUILD_TYPE} --parallel ${CHASE_BUILD_JOBS}
        INSTALL_COMMAND ${_env} ${CMAKE_COMMAND} --install <BINARY_DIR>
            --config ${CMAKE_BUILD_TYPE}
        BUILD_ALWAYS ON
        USES_TERMINAL_CONFIGURE ON
        USES_TERMINAL_BUILD ON)
endfunction()

if(CHASE_BUILD_TESTS)
    set(_tests ON)
else()
    set(_tests OFF)
endif()

chase_module(third_party)
# INSTALL_GTEST=OFF keeps googletest (fetched by the tests) out of the prefix.
chase_module(core-library DEPENDS third_party
    ARGS -DENABLE_TESTS=${_tests} -DINSTALL_GTEST=OFF)
chase_module(chase-backends DEPENDS core-library)
chase_module(DSL_tool DEPENDS chase-backends)
chase_module(logics_tool DEPENDS chase-backends)

# 'ctest' in the build directory runs the tests of the core library.
if(CHASE_BUILD_TESTS)
    enable_testing()
    add_test(NAME core-library
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C ${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/core-library)
endif()

# Python wheel: the compiled modules and the ANTLR runtime go into
# chase_cps/_lib, next to the Python package (python/chase_cps).
if(SKBUILD)
    install(DIRECTORY ${CHASE_PREFIX}/chase/lib/ DESTINATION chase_cps/_lib
        FILES_MATCHING PATTERN "pychase*")
    install(DIRECTORY ${CHASE_PREFIX}/third_party/antlr4/lib/ DESTINATION chase_cps/_lib
        FILES_MATCHING PATTERN "libantlr4-runtime*" PATTERN "*.a" EXCLUDE)
endif()
