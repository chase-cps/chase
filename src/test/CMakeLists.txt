# @author Michele Lora

###############################################################################
# CMake file for the CHASE Test Tool
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        )

set(TEST_SRC ${PROJECT_SOURCE_DIR}/src/test/src)

set(test_tool_srcs
        ${TEST_SRC}/main.cc
   )

if(COMMAND cmake_policy)
        cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(testtool ${test_tool_srcs})
add_dependencies(testtool chasecore)
add_dependencies(testtool antlr4_static)

target_link_libraries(testtool
       chasecore
       antlr4_static)



