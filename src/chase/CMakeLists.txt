###############################################################################
# CMake file for the Abstract Syntax Tree Library
# @author Michele Lora
###############################################################################

set( chase_ast_srcs
    ${PROJECT_SOURCE_DIR}/src/chase/src/AST/BaseObject.cc
    ${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/Globals.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Manipulation/BaseVisitor.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Manipulation/GuideVisitor.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Manipulation/SimplifyVisitor.cc
    ${PROJECT_SOURCE_DIR}/src/chase/src/Manipulation/FixParentsVisitor.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Manipulation/findComponents.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Variable.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Constant.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Function.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/WellFormedFormula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/LogicAtom.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/LogicPredicate.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/LogicConstant.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/LogicProposition.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/BooleanFormula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/BinaryFormula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/UnaryFormula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/BinaryLogicCombination.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/UnaryLogicCombination.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/TemporalFormula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/UnaryTemporalFormula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/BinaryTemporalFormula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Logics/GR1Formula.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Timing/Counter.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Timing/LivenessCounter.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Behaviors/Timing/SafetyCounter.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Contracts/Contract.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Contracts/AGContract.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/ComponentsGroup.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/DomainSpecificConverterType.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/DomainSpecificSinkType.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/DomainSpecificSwitchType.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/Network.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/NetworkConnection.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/DomainSpecificBusType.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/DomainSpecificGenericType.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/DomainSpecificSourceType.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/DomainSpecificType.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/NetworkComponent.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Architecture/NetworkDefinitions.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/TextBasedPattern.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/Requirement.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/Problem.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/RequirementFunction.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/GraphBasedFunctions.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/FunctionsLibrary.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/AlwaysActiveConnection.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/MaxFailures.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/MaxFailuresPerStep.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/MustDisconnectFailed.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/NeverConnect.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/NeverDisconnect.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/NoFailures.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/NoControl.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/NoRecovery.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/PreferActiveConnection.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/KeepConnectionStable.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/InitialState.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/InitialCommand.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/SetTimeResolution.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/SwitchOnTime.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Specification/BaseFunctionsLibrary/SwitchOffTime.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Utilities/Graphs/Graphs.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Utilities/Utilities.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Utilities/Timing/CounterUtilities.cc 
	${PROJECT_SOURCE_DIR}/src/chase/src/Utilities/Portability/Portability.cc 
    )

set( SRC_CHASELIB_PATH ${PROJECT_SOURCE_DIR}/src)
set( chase_tool_srcs
        ${SRC_CHASELIB_PATH}/chase/src/main.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2TuLiP/SpinSyntaxWriter.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2TuLiP/TuLiPPrinter.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2GR1C/GR1CPrinter.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2GR1C/GR1CSyntaxWriter.cc
		${SRC_CHASELIB_PATH}/backend/src/manipulation/ResolveCounters.cc
        ${SRC_CHASELIB_PATH}/backend/src/GR1/ProgressFinder.cc
		${SRC_CHASELIB_PATH}/backend/src/GR1/SafetyFinder.cc
		${SRC_CHASELIB_PATH}/backend/src/GR1/InitialConditionsFinder.cc
		${SRC_CHASELIB_PATH}/backend/src/GR1/GR1Builder.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/TextualParsing/TextualParsing.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/TextualParsing/PatternLibrary.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/TextualParsing/EPSPatternLibrary.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/EPSPatterns.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/TextualParsing/AorGVisitor.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/KeepBalanced.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/KeepConnected.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/KeepPowered.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/NoParallel.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/UnhealthySources.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/UnhealthySources_strong.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/AllowedFailures.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/PermanentFailures.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/TimedEPSPatterns.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/TimedUnhealthySources.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/TimedKeepPowered.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/EPSPatterns/TimedContactors.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.cc
		${SRC_CHASELIB_PATH}/frontend/src/Patterns2Chase/TextualParsing/EPSTimedPatternLibrary.cc
		${SRC_CHASELIB_PATH}/frontend/src/DSLParser/ChaseBaseListener.cpp
		${SRC_CHASELIB_PATH}/frontend/src/DSLParser/ChaseLexer.cpp
		${SRC_CHASELIB_PATH}/frontend/src/DSLParser/ChaseListener.cpp
		${SRC_CHASELIB_PATH}/frontend/src/DSLParser/ChaseParser.cpp
		${SRC_CHASELIB_PATH}/frontend/src/DSLParser/SpecBuilder.cc
        ${SRC_CHASELIB_PATH}/epstool/src/EPS/EPSTopology.cc
        ${SRC_CHASELIB_PATH}/epstool/src/EPS/EPSDefinitions.cc
   )

include_directories(
        ${PROJECT_SOURCE_DIR}/src/chase/include
        ${PROJECT_SOURCE_DIR}/src/backend/include
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/run/include
        ${PROJECT_SOURCE_DIR}/src/epstool/include
        )

include_directories( SYSTEM
        ${PROJECT_SOURCE_DIR}/src/frontend/include
        )



add_library(chase ${chase_ast_srcs})

set_property(TARGET chase PROPERTY CXX_STANDARD 11)

install(TARGETS chase DESTINATION lib)


link_directories(${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/run/lib/)


add_executable(chasetool ${chase_tool_srcs})
add_dependencies(chasetool chase)
target_link_libraries(chasetool 
        chase
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/run/lib/libantlr4-runtime.a)

set_property(TARGET chasetool PROPERTY CXX_STANDARD 11)
set_property(TARGET chasetool PROPERTY OUTPUT_NAME chase)


install(TARGETS chasetool DESTINATION bin)

