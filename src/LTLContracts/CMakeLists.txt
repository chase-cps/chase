# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing patterns over networks.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/src/LTLContracts/parser
        ${PROJECT_SOURCE_DIR}/src/LTLContracts/include
)

set(LTL_PARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/src/LTLContracts/parser)
set(LTL_SRC_DIR ${PROJECT_SOURCE_DIR}/src/LTLContracts)

set(LTL_PARSER_SRCS
        ${LTL_PARSER_SRC_DIR}/LTLContractsBaseListener.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsLexer.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsListener.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsParser.cpp
        )

set(LTL_TOOL_SRCS
        ${LTL_PARSER_SRCS}
        ${LTL_SRC_DIR}/src/main.cc
)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(LTLContracts ${LTL_TOOL_SRCS})
add_dependencies(LTLContracts chasecore)
add_dependencies(LTLContracts antlr4_static)

target_link_libraries(LTLContracts
        chasecore
        antlr4_static)