###############################################################################
# CMake file for the tool converting the DSL into a backend/src/GR1 specification.
# @author Michele Lora
###############################################################################
set( SRC_CHASELIB_PATH ${PROJECT_SOURCE_DIR}/src)

set( epstool_srcs
        ${SRC_CHASELIB_PATH}/epstool/src/EPSMain.cc
        ${SRC_CHASELIB_PATH}/epstool/src/EPS/EPSTopology.cc
        ${SRC_CHASELIB_PATH}/epstool/src/EPS/EPSDefinitions.cc
        ${SRC_CHASELIB_PATH}/backend/src/GR1/ProgressFinder.cc
		${SRC_CHASELIB_PATH}/backend/src/GR1/SafetyFinder.cc
		${SRC_CHASELIB_PATH}/backend/src/GR1/InitialConditionsFinder.cc
		${SRC_CHASELIB_PATH}/backend/src/GR1/GR1Builder.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2TuLiP/SpinSyntaxWriter.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2TuLiP/TuLiPPrinter.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2GR1C/GR1CPrinter.cc
		${SRC_CHASELIB_PATH}/backend/src/Chase2GR1C/GR1CSyntaxWriter.cc
		${SRC_CHASELIB_PATH}/backend/src/manipulation/ResolveCounters.cc
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

   )


## It should point to the a CHASE_INSTALL_DIR
include_directories(
        ${PROJECT_SOURCE_DIR}/src/chase/include
        ${PROJECT_SOURCE_DIR}/src/backend/include
        ${PROJECT_SOURCE_DIR}/src/frontend/include
        ${PROJECT_SOURCE_DIR}/third_party/antlr4_runtime/run/include
        ${PROJECT_SOURCE_DIR}/src/epstool/include
        )


add_executable(epstool ${epstool_srcs})
add_dependencies(epstool chase)

set_property(TARGET epstool PROPERTY CXX_STANDARD 11)

target_link_libraries(epstool chase)
install(TARGETS epstool DESTINATION bin)
