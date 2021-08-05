# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing patterns over networks.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/dsl_frontend/include
        ${PROJECT_SOURCE_DIR}/src/dsl_frontend/include/DSLParser
)

# PON = Patterns On Network
set(PON_SRC ${PROJECT_SOURCE_DIR}/src/dsl_frontend/src)

set(pon_tool_srcs
        ${PON_SRC}/DSLParser/ChaseBaseListener.cpp
        ${PON_SRC}/DSLParser/ChaseLexer.cpp
        ${PON_SRC}/DSLParser/ChaseListener.cpp
        ${PON_SRC}/DSLParser/ChaseParser.cpp

        ${PON_SRC}/DesignProblem.cc

        ${PON_SRC}/DSLSpecsBuilder.cc
        ${PON_SRC}/SpecBuilder_support.cc
        ${PON_SRC}/ProblemDefinitions.cc
        ${PON_SRC}/DesignProblem_patterns.cc
        ${PON_SRC}/DesignProblem_support.cc
        )

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(dsl_parser
        ${PON_SRC}/main.cc
        ${pon_tool_srcs})
add_dependencies(dsl_parser chasecore)
add_dependencies(dsl_parser antlr4_static)

target_link_libraries(dsl_parser
        antlr4_static
        chasecore
        )

add_library(chase_dsl_parser_library
        ${pon_tool_srcs}
        )

install(TARGETS dsl_parser DESTINATION chase/bin)
