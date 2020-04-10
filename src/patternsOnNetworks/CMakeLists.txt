# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing patterns over networks.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/patternsOnNetworks/include
        ${PROJECT_SOURCE_DIR}/src/patternsOnNetworks/include/DSLParser
)

# PON = Patterns On Network
set(PON_SRC ${PROJECT_SOURCE_DIR}/src/patternsOnNetworks/src)

set(pon_tool_srcs
        ${PON_SRC}/main.cc
        ${PON_SRC}/DSLParser/ChaseBaseListener.cpp
        ${PON_SRC}/DSLParser/ChaseLexer.cpp
        ${PON_SRC}/DSLParser/ChaseListener.cpp
        ${PON_SRC}/DSLParser/ChaseParser.cpp

        ${PON_SRC}/DSLParser/DesignProblem.cc

        ${PON_SRC}/DSLParser/SpecBuilder.cc
        ${PON_SRC}/DSLParser/SpecBuilder_support.cc
        ${PON_SRC}/DSLParser/ProblemDefinitions.cc
        ${PON_SRC}/DSLParser/DesignProblem_patterns.cc
        ${PON_SRC}/DSLParser/DesignProblem_support.cc
        )

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(patternsOnNetwork ${pon_tool_srcs})
add_dependencies(patternsOnNetwork chasecore)
add_dependencies(patternsOnNetwork antlr4_static)

target_link_libraries(patternsOnNetwork
        chasecore
        antlr4_static)
