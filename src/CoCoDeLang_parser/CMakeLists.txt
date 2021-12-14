# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing the formal language of CHASE.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/src/CoCoDeLang_parser
        ${PROJECT_SOURCE_DIR}/src/CoCoDeLang_parser/include/
)

set(COCODE_PARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/src/CoCoDeLang_parser/parser)
set(COCODE_SRC_DIR ${PROJECT_SOURCE_DIR}/src/CoCoDeLang_parser)

set(COCODE_PARSER_SRCS
        ${COCODE_PARSER_SRC_DIR}/CoCoDeLangBaseVisitor.cpp
        ${COCODE_PARSER_SRC_DIR}/CoCoDeLangLexer.cpp
        ${COCODE_PARSER_SRC_DIR}/CoCoDeLangVisitor.cpp
        ${COCODE_PARSER_SRC_DIR}/CoCoDeLangParser.cpp)

set(COCODE_TOOL_SRCS
        ${COCODE_SRC_DIR}/src/main.cc
        )

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(cocode_frontend ${COCODE_TOOL_SRCS})
add_dependencies(cocode_frontend chase_logics_parser_library)
add_dependencies(cocode_frontend chasecore)
add_dependencies(cocode_frontend antlr4_static)

add_library(chase_cocode_parser_library
        ${COCODE_PARSER_SRCS}
        ${COCODE_SRC_DIR}/src/CoCoDeSpecsBuilder.cc
        )

############## SUPPRESS THIRD PARTIES WARNINGS #################################
target_compile_options(chase_cocode_parser_library PRIVATE "-w")
################################################################################

target_link_libraries(cocode_frontend
        chase_cocode_parser_library
        chasecore
        antlr4_static
        )

install(TARGETS cocode_frontend
        DESTINATION chase/bin
        )

install(TARGETS chase_cocode_parser_library
        DESTINATION chase/lib
        )

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/cocode_frontend/parser/
        DESTINATION chase/include/cocode_parser/parser
        COMPONENT dev
        FILES_MATCHING PATTERN ".h"
)

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/cocode_frontend/include/
        DESTINATION chase/include/cocode_parser
        COMPONENT dev
        FILES_MATCHING PATTERN "*CoCoDeSpecsBuilder.hh"
)