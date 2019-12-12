# @author Michele Lora

###############################################################################
# CMake file for the CHASE Library
# @author Michele Lora
###############################################################################

set( SRC_CHASELIB_PATH ${PROJECT_SOURCE_DIR}/src/core/src/chase)

set(chase_library_representation
        ${SRC_CHASELIB_PATH}/representation/ChaseObject.cc
        ${SRC_CHASELIB_PATH}/representation/Specification.cc
        ${SRC_CHASELIB_PATH}/representation/Contract.cc

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
        ${SRC_CHASELIB_PATH}/utilities/Copy.cc
        ${SRC_CHASELIB_PATH}/utilities/UtilityFunctions.cc
        ${SRC_CHASELIB_PATH}/utilities/GraphUtilities.cc
)

set(chase_library
        ${chase_library_representation}
        ${chase_library_utilities}
        ${SRC_CHASELIB_PATH}/BaseVisitor.cc
        ${SRC_CHASELIB_PATH}/GuideVisitor.cc
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/src/core/include
        ${PROJECT_SOURCE_DIR}/src/core/include/chase
        )


add_library(chasecore ${chase_library})

install(TARGETS chasecore DESTINATION lib)


