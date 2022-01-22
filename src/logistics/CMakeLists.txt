# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing LTL specifications.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/src/logistics/
        ${PROJECT_SOURCE_DIR}/src/logistics/include
)

set(logistics_PARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/src/logistics/parser)
set(logistics_SRC_DIR ${PROJECT_SOURCE_DIR}/src/logistics)

set(logistics_PARSER_SRCS
        ${logistics_PARSER_SRC_DIR}/LogisticsLangBaseVisitor.cpp
        ${logistics_PARSER_SRC_DIR}/LogisticsLangLexer.cpp
        ${logistics_PARSER_SRC_DIR}/LogisticsLangVisitor.cpp
        ${logistics_PARSER_SRC_DIR}/LogisticsLangParser.cpp)

set(logistics_TOOL_SRCS
        ${logistics_SRC_DIR}/src/main.cc
        )

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(logistics_frontend ${logistics_TOOL_SRCS})
add_dependencies(logistics_frontend chase_logistics_parser_library)
add_dependencies(logistics_frontend chasecore)
add_dependencies(logistics_frontend antlr4_static)

add_library(chase_logistics_parser_library
        ${logistics_PARSER_SRCS}
        ${logistics_SRC_DIR}/src/LogisticsSpecsBuilder.cc
        ${logistics_SRC_DIR}/src/LogisticsSpecsBuilder_topology.cc
        ${logistics_SRC_DIR}/src/LogisticsProblem.cc)

############## SUPPRESS THIRD PARTIES WARNINGS #################################
target_compile_options(chase_logistics_parser_library PRIVATE "-w")
################################################################################

target_link_libraries(logistics_frontend
        chase_logistics_parser_library
        chasecore
        antlr4_static
        )

install(TARGETS logistics_frontend
        DESTINATION chase/bin
        )

install(TARGETS chase_logistics_parser_library
        DESTINATION chase/lib
        )

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/logistics_frontend/parser/
        DESTINATION chase/include/logistics_parser/parser
        COMPONENT dev
        FILES_MATCHING PATTERN ".h"
)

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/logistics_frontend/include/
        DESTINATION chase/include/logistics_parser
        COMPONENT dev
        FILES_MATCHING PATTERN "*logisticsSpecsBuilder.hh"
)
