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

        ${SRC_CHASELIB_PATH}/representation/Value.cc
        ${SRC_CHASELIB_PATH}/representation/Range.cc
        ${SRC_CHASELIB_PATH}/representation/NumericValue.cc
        ${SRC_CHASELIB_PATH}/representation/IntegerValue.cc
        ${SRC_CHASELIB_PATH}/representation/RealValue.cc
        ${SRC_CHASELIB_PATH}/representation/BooleanValue.cc
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
)

set(chase_library_backends
        ${SRC_CHASELIB_PATH}/backends/GR1CPrinter.cc
        ${SRC_CHASELIB_PATH}/backends/GR1CPrinter_visit.cc
        ${SRC_CHASELIB_PATH}/backends/SlugsPrinter.cc
        ${SRC_CHASELIB_PATH}/backends/SlugsPrinter_visit.cc
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

install(TARGETS chasecore DESTINATION core/lib)


install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/core/include
        DESTINATION core
       )

pybind11_add_module(
    chasecorebnd MODULE 
    ${PROJECT_SOURCE_DIR}/src/core/bindings/bindings.cc
    )
    
target_link_libraries(chasecorebnd PRIVATE chasecore)
install(TARGETS chasecorebnd DESTINATION core/pylib)


