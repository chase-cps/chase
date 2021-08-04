# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing patterns over networks.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/src/logics_frontend/
        ${PROJECT_SOURCE_DIR}/src/logics_frontend/include
)

set(LTL_PARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/src/logics_frontend/parser)
set(LTL_SRC_DIR ${PROJECT_SOURCE_DIR}/src/logics_frontend)

set(LTL_PARSER_SRCS
        ${LTL_PARSER_SRC_DIR}/LTLContractsBaseVisitor.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsLexer.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsVisitor.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsParser.cpp
        )

set(LTL_TOOL_SRCS
        ${LTL_SRC_DIR}/src/main.cc
        ${LTL_SRC_DIR}/src/Console.cc
        ${LTL_SRC_DIR}/src/Console_commands.cc
)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(logics_frontend ${LTL_TOOL_SRCS})
add_dependencies(logics_frontend chase_logics_parser_library)
add_dependencies(logics_frontend chasecore)
add_dependencies(logics_frontend antlr4_static)


add_library(chase_logics_parser_library
        ${LTL_PARSER_SRCS}
        ${LTL_SRC_DIR}/src/LTLSpecsBuilder.cc
        )

############## SUPPRESS THIRD PARTIES WARNINGS #################################
target_compile_options(chase_logics_parser_library PRIVATE "-w")
################################################################################

target_link_libraries(logics_frontend
        chase_logics_parser_library
        chasecore
        antlr4_static
        )

install(TARGETS logics_frontend
        DESTINATION chase/bin
       )

install(TARGETS chase_logics_parser_library
        DESTINATION chase/lib
       )

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/logics_frontend/parser/
        DESTINATION chase/include/logics_parser/parser
        COMPONENT dev
        FILES_MATCHING PATTERN ".h"
       )

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/logics_frontend/include/
        DESTINATION chase/include/logics_parser
        COMPONENT dev
        FILES_MATCHING PATTERN "*LTLSpecsBuilder.hh"
       )

