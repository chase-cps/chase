find_package(PythonLibs REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/runtime/src
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PROJECT_SOURCE_DIR}/src/logics_frontend/
        ${PROJECT_SOURCE_DIR}/src/logics_frontend/include
        ${PROJECT_SOURCE_DIR}/src/logics_frontend/parser
        ${PROJECT_SOURCE_DIR}/src/dsl_frontend/include
)

pybind11_add_module(
        pychase MODULE
        bindings.cc
)

target_link_libraries(pychase PRIVATE
        chase_logics_parser_library
        chase_dsl_parser_library
        antlr4_static
        chasecore
        )

install(
        TARGETS pychase
        EXPORT pychaseTargets
        DESTINATION ${LIB_INSTALL_DIR}/pylib
)