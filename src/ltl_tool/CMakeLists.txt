# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing patterns over networks.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/src/ltl_tool/
        ${PROJECT_SOURCE_DIR}/src/ltl_tool/include
)

set(LTL_PARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/src/ltl_tool/parser)
set(LTL_SRC_DIR ${PROJECT_SOURCE_DIR}/src/ltl_tool)

set(LTL_PARSER_SRCS
        ${LTL_PARSER_SRC_DIR}/LTLContractsBaseListener.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsLexer.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsListener.cpp
        ${LTL_PARSER_SRC_DIR}/LTLContractsParser.cpp
        )

set(LTL_TOOL_SRCS
        ${LTL_PARSER_SRCS}
        ${LTL_SRC_DIR}/src/main.cc
        ${LTL_SRC_DIR}/src/LTLSpecsBuilder.cc
)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(ltl_tool ${LTL_TOOL_SRCS})
add_dependencies(ltl_tool chasecore)
add_dependencies(ltl_tool antlr4_static)

target_link_libraries(ltl_tool
        chasecore
        antlr4_static)