###############################################################################
# CMake file for the CHASE Library
# @author Michele Lora
###############################################################################

set( SRC_CHASELIB_PATH ${PROJECT_SOURCE_DIR}/src/core/src/chase)

find_package(PythonLibs REQUIRED)

set(chase_library_representation
        ${SRC_CHASELIB_PATH}/representation/ChaseObject.cc
        ${SRC_CHASELIB_PATH}/representation/System.cc
        ${SRC_CHASELIB_PATH}/representation/Specification.cc
        ${SRC_CHASELIB_PATH}/representation/Contract.cc
        ${SRC_CHASELIB_PATH}/representation/Contract_algebra.cc

        ${SRC_CHASELIB_PATH}/representation/Type.cc
        ${SRC_CHASELIB_PATH}/representation/SimpleType.cc
        ${SRC_CHASELIB_PATH}/representation/Integer.cc
        ${SRC_CHASELIB_PATH}/representation/Real.cc
        ${SRC_CHASELIB_PATH}/representation/Boolean.cc
        ${SRC_CHASELIB_PATH}/representation/String.cc
        ${SRC_CHASELIB_PATH}/representation/CustomType.cc
        ${SRC_CHASELIB_PATH}/representation/Enumeration.cc

        ${SRC_CHASELIB_PATH}/representation/Value.cc
        ${SRC_CHASELIB_PATH}/representation/Range.cc
        ${SRC_CHASELIB_PATH}/representation/NumericValue.cc
        ${SRC_CHASELIB_PATH}/representation/IntegerValue.cc
        ${SRC_CHASELIB_PATH}/representation/RealValue.cc
        ${SRC_CHASELIB_PATH}/representation/BooleanValue.cc
        ${SRC_CHASELIB_PATH}/representation/StringValue.cc
        ${SRC_CHASELIB_PATH}/representation/Identifier.cc
        ${SRC_CHASELIB_PATH}/representation/Expression.cc

        ${SRC_CHASELIB_PATH}/representation/Name.cc
        ${SRC_CHASELIB_PATH}/representation/Declaration.cc
        ${SRC_CHASELIB_PATH}/representation/DataDeclaration.cc
        ${SRC_CHASELIB_PATH}/representation/Variable.cc
        ${SRC_CHASELIB_PATH}/representation/Constant.cc
        ${SRC_CHASELIB_PATH}/representation/Parameter.cc
        ${SRC_CHASELIB_PATH}/representation/ComponentDefinition.cc

        ${SRC_CHASELIB_PATH}/representation/Operators.cc

        ${SRC_CHASELIB_PATH}/representation/Proposition.cc
        ${SRC_CHASELIB_PATH}/representation/UnaryBooleanFormula.cc
        ${SRC_CHASELIB_PATH}/representation/BinaryBooleanFormula.cc
        ${SRC_CHASELIB_PATH}/representation/BooleanConstant.cc
        ${SRC_CHASELIB_PATH}/representation/ModalFormula.cc
        ${SRC_CHASELIB_PATH}/representation/UnaryTemporalFormula.cc
        ${SRC_CHASELIB_PATH}/representation/BinaryTemporalFormula.cc
        ${SRC_CHASELIB_PATH}/representation/LargeBooleanFormula.cc
        ${SRC_CHASELIB_PATH}/representation/Graph.cc
        ${SRC_CHASELIB_PATH}/representation/GraphVertex.cc
        ${SRC_CHASELIB_PATH}/representation/GraphEdge.cc

        ${SRC_CHASELIB_PATH}/utilities/Factory_baseFunctions.cc
        )

set(chase_library_utilities
        ${SRC_CHASELIB_PATH}/utilities/IOUtils.cc
        ${SRC_CHASELIB_PATH}/utilities/UtilityFunctions.cc
        ${SRC_CHASELIB_PATH}/utilities/GraphUtilities.cc
        ${SRC_CHASELIB_PATH}/utilities/ClonedDeclarationVisitor.cc
        ${SRC_CHASELIB_PATH}/utilities/LogicSimplificationVisitor.cc
        ${SRC_CHASELIB_PATH}/utilities/LogicNotNormalizationVisitor.cc
        ${SRC_CHASELIB_PATH}/utilities/GroupTemporalOperatorsVisitor.cc
        ${SRC_CHASELIB_PATH}/utilities/VarsCausalityVisitor.cc
        ${SRC_CHASELIB_PATH}/utilities/simplify.cc
)

set(chase_library_backends
        ${SRC_CHASELIB_PATH}/backends/GR1CPrinter.cc
        ${SRC_CHASELIB_PATH}/backends/GR1CPrinter_visit.cc
        ${SRC_CHASELIB_PATH}/backends/SlugsPrinter.cc
        ${SRC_CHASELIB_PATH}/backends/SlugsPrinter_visit.cc
        ${SRC_CHASELIB_PATH}/backends/NuSMVPrinter.cc
        ${SRC_CHASELIB_PATH}/backends/NuSMVPrinter_visit.cc
)

set(chase_library
        ${chase_library_representation}
        ${chase_library_utilities}
        ${chase_library_backends}
        ${SRC_CHASELIB_PATH}/BaseVisitor.cc
        ${SRC_CHASELIB_PATH}/GuideVisitor.cc
        src/chase/representation/Contract_algebra.cc
        src/chase/representation/Component.cc
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        ${PYTHON_INCLUDE_DIRS}
        )



add_library(chasecore ${chase_library})
set_target_properties(chasecore PROPERTIES POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)
set(LIB_INSTALL_DIR chase/lib  CACHE STRING ¨¨)
set(INCLUDE_INSTALL_DIR chase  CACHE STRING ¨¨)

pybind11_add_module(
        chasecorebnd MODULE 
        ${PROJECT_SOURCE_DIR}/src/core/bindings/bindings.cc
)
    
target_link_libraries(chasecorebnd PRIVATE chasecore)

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/core/include
        DESTINATION ${INCLUDE_INSTALL_DIR}
       )

install( 
        TARGETS chasecore
        EXPORT chaseTargets
        DESTINATION ${LIB_INSTALL_DIR}
)

install(
        TARGETS chasecorebnd
        EXPORT chaseBndTargets
        DESTINATION ${LIB_INSTALL_DIR}/pylib)

install(
        EXPORT chaseTargets
        DESTINATION ${LIB_INSTALL_DIR}/cmake/chase
        FILE chaseTargets.cmake # Not sure if this is still needed
)

install(
        EXPORT chaseBndTargets
        DESTINATION ${LIB_INSTALL_DIR}/cmake/chase
        FILE chaseBndTargets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_package_config_file( 
        ${PROJECT_SOURCE_DIR}/chaseConfig.cmake.in
        "chaseConfig.cmake"
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/cmake/chase
        PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR )

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/chaseConfigVersion.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
)

### Install Config and ConfigVersion files
install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/chaseConfig.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/chaseConfigVersion.cmake"
        DESTINATION "${LIB_INSTALL_DIR}/cmake/chase"
)
