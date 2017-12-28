ifeq ($(OS),Windows_NT)
PLATFORM= Windows
else
PLATFORM= $(shell uname -s)
ARCH=$(shell getconf LONG_BIT)
endif

ifndef CHASE_COMPILER
CXX=g++
else
CXX=$$CHASE_COMPILER
endif
LD=$(CXX)

ANTLR4=$(ANTLR4_HOME)

ifeq ($(ANTLR4),)
ANTLR4=third_party/runtime-source/run
endif

ANTLR4_INCLUDE=$(ANTLR4)/include
ANTLR4_LIB=$(ANTLR4)/lib/libantlr4-runtime.a


CHASE_LIB=lib/libchase.a
EXECUTABLE=bin/chase

######### TOOLS ###
#### FRONT-ENDs

### PATTERNS 2 CHASE


PATTERNS2CHASE_EXECUTABLE = bin/patterns2chase




EPSTOOL_EXECUTABLE = bin/epstool
EPSTOOL_OBJS = obj/epstool/EPSMain.o

	


INCDIRS = 	-Iinclude \
			-Isrc/chase/include \
		  	-Isrc/chase/include/Behaviors \
		  	-Isrc/chase/include/Behaviors/Logics \
		  	-Isrc/chase/include/Specification \
		  	-Isrc/chase/include/Manipulation \
		  	-Isrc/frontend/include/ \
			-Iinclude/Patterns2Chase \
			-Isrc/frontend/include/Patterns2Chase/TextualParsing \
			-Isrc/backend/include \
			-Isrc/epstool/include \
			-I$(ANTLR4_INCLUDE) \
			-I$(ANTLR4_INCLUDE)/antlr4-runtime \
			-I/usr/local/include

PARSER_OBJS = \
			  obj/frontend/ChaseBaseListener.o \
			  obj/frontend/ChaseLexer.o \
			  obj/frontend/ChaseListener.o \
			  obj/frontend/ChaseParser.o \
			  obj/frontend/SpecBuilder.o

OBJS =  obj/Behaviors/Logics/Globals.o \
		obj/Manipulation/BaseVisitor.o \
		obj/Manipulation/GuideVisitor.o \
		obj/Manipulation/SimplifyVisitor.o \
		obj/Manipulation/findComponents.o \
		obj/Behaviors/Variable.o \
		obj/Behaviors/Constant.o \
		obj/Behaviors/Function.o \
		obj/Behaviors/Logics/WellFormedFormula.o \
		obj/Behaviors/Logics/LogicAtom.o \
		obj/Behaviors/Logics/LogicPredicate.o \
		obj/Behaviors/Logics/LogicConstant.o \
		obj/Behaviors/Logics/LogicProposition.o \
		obj/Behaviors/Logics/BooleanFormula.o \
		obj/Behaviors/Logics/BinaryFormula.o \
		obj/Behaviors/Logics/UnaryFormula.o \
		obj/Behaviors/Logics/BinaryLogicCombination.o \
		obj/Behaviors/Logics/UnaryLogicCombination.o \
		obj/Behaviors/Logics/TemporalFormula.o \
		obj/Behaviors/Logics/UnaryTemporalFormula.o \
		obj/Behaviors/Logics/BinaryTemporalFormula.o \
		obj/Behaviors/Logics/GR1Formula.o \
		obj/Behaviors/Timing/Counter.o \
		obj/Behaviors/Timing/LivenessCounter.o \
		obj/Behaviors/Timing/SafetyCounter.o \
		obj/Contracts/Contract.o \
		obj/Contracts/AGContract.o \
		obj/epstool/EPS/EPSTopology.o \
		obj/epstool/EPS/EPSDefinitions.o \
		obj/Architecture/ComponentsGroup.o \
		obj/Architecture/DomainSpecificConverterType.o \
		obj/Architecture/DomainSpecificSinkType.o \
		obj/Architecture/DomainSpecificSwitchType.o \
		obj/Architecture/Network.o \
		obj/Architecture/NetworkConnection.o \
		obj/Architecture/DomainSpecificBusType.o \
		obj/Architecture/DomainSpecificGenericType.o \
		obj/Architecture/DomainSpecificSourceType.o \
		obj/Architecture/DomainSpecificType.o \
		obj/Architecture/NetworkComponent.o \
		obj/Architecture/NetworkDefinitions.o \
		obj/Specification/TextBasedPattern.o \
		obj/Specification/Requirement.o \
		obj/Specification/Problem.o \
		obj/Specification/RequirementFunction.o \
		obj/Specification/GraphBasedFunctions.o \
		obj/Specification/FunctionsLibrary.o \
		obj/Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.o \
		obj/Specification/BaseFunctionsLibrary/AlwaysActiveConnection.o \
		obj/Specification/BaseFunctionsLibrary/MaxFailures.o \
		obj/Specification/BaseFunctionsLibrary/MustDisconnectFailed.o \
		obj/Specification/BaseFunctionsLibrary/NeverConnect.o \
		obj/Specification/BaseFunctionsLibrary/NeverDisconnect.o \
		obj/Specification/BaseFunctionsLibrary/NoFailures.o \
		obj/Specification/BaseFunctionsLibrary/NoControl.o \
		obj/Specification/BaseFunctionsLibrary/NoRecovery.o \
		obj/Specification/BaseFunctionsLibrary/PreferActiveConnection.o \
		obj/Specification/BaseFunctionsLibrary/KeepConnectionStable.o \
		obj/Specification/BaseFunctionsLibrary/InitialState.o \
		obj/Specification/BaseFunctionsLibrary/InitialCommand.o \
		obj/Specification/BaseFunctionsLibrary/SetTimeResolution.o \
		obj/Specification/BaseFunctionsLibrary/SwitchOnTime.o \
		obj/Specification/BaseFunctionsLibrary/SwitchOffTime.o \
		obj/Utilities/Graphs/Graphs.o \
		obj/Utilities/Utilities.o \
		obj/Utilities/Timing/CounterUtilities.o \
		obj/Utilities/Portability/Portability.o \
		obj/GR1/ProgressFinder.o \
		obj/GR1/SafetyFinder.o \
		obj/GR1/InitialConditionsFinder.o \
		obj/GR1/GR1Builder.o \
		obj/Backend/Chase2TuLiP/SpinSyntaxWriter.o \
		obj/Backend/Chase2TuLiP/TuLiPPrinter.o \
		obj/Backend/Chase2GR1C/GR1CPrinter.o \
		obj/Backend/Chase2GR1C/GR1CSyntaxWriter.o \
		obj/Backend/manipulation/ResolveCounters.o \
		obj/Patterns2Chase/TextualParsing/TextualParsing.o \
		obj/Patterns2Chase/TextualParsing/PatternLibrary.o \
		obj/Patterns2Chase/TextualParsing/EPSPatternLibrary.o \
		obj/Patterns2Chase/EPSPatterns/EPSPatterns.o \
		obj/Patterns2Chase/TextualParsing/AorGVisitor.o \
		obj/Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.o \
		obj/Patterns2Chase/EPSPatterns/KeepBalanced.o \
		obj/Patterns2Chase/EPSPatterns/KeepConnected.o \
		obj/Patterns2Chase/EPSPatterns/KeepPowered.o \
		obj/Patterns2Chase/EPSPatterns/NoParallel.o \
		obj/Patterns2Chase/EPSPatterns/UnhealthySources.o \
		obj/Patterns2Chase/EPSPatterns/UnhealthySources_strong.o \
		obj/Patterns2Chase/EPSPatterns/AllowedFailures.o \
		obj/Patterns2Chase/EPSPatterns/PermanentFailures.o \
		obj/Patterns2Chase/EPSPatterns/TimedEPSPatterns.o \
		obj/Patterns2Chase/EPSPatterns/TimedUnhealthySources.o \
		obj/Patterns2Chase/EPSPatterns/TimedKeepPowered.o \
		obj/Patterns2Chase/EPSPatterns/TimedContactors.o \
		obj/Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.o \
		obj/Patterns2Chase/TextualParsing/EPSTimedPatternLibrary.o

DIRS = bin obj lib\
	   obj/Behaviors \
	   obj/Behaviors/Logics \
	   obj/Behaviors/Timing \
	   obj/Contracts \
	   obj/Manipulation \
	   obj/Specification \
	   obj/Specification/BaseFunctionsLibrary \
	   obj/Patterns2Chase \
	   obj/Patterns2Chase/EPSPatterns \
	   obj/Patterns2Chase/TextualParsing \
	   obj/Backend/ \
	   obj/Backend/manipulation \
	   obj/Backend/Chase2TuLiP \
	   obj/GR1 \
	   obj/Backend/Chase2TuLiP \
	   obj/Backend/Chase2GR1C \
	   obj/Utilities \
	   obj/Utilities/Graphs \
	   obj/Utilities/Timing \
	   obj/Utilities/Portability \
	   obj/Architecture \
	   obj/epstool/EPS \
	   obj/frontend


LIBDIRS = -Llib -L/usr/local/lib
LIBS = 

DEBUG=-ggdb
CXXFLAGS=-c -static $(DEBUG) $(INCDIRS) -std=c++11 -Wall -Wextra -pedantic
PARSERFLAGS=-c -static $(DEBUG) $(INCDIRS) -std=c++11 -w

LDFLAGS= -static $(DEBUG) $(LIBDIRS) $(LIBS) 

all: dirs $(CHASE_LIB) \
		$(EXECUTABLE) \
		$(EPSTOOL_EXECUTABLE)

PARSER=bin/parser

test: $(EXECUTBALE)
	@bin/chase -i Terraswarm/specs/trivial_failing.txt -o \
		badspec.py -V
	@python badspec.py
	@read -r dummy
	@bin/chase -i Terraswarm/specs/trivial_synthesizable.txt -o \
		workingspec.py -V
	@python workingspec.py

$(EXECUTABLE): obj/main.o $(PARSER_OBJS) $(CHASE_LIB) $(ANTLR4_LIB) 
	@echo Linking $@
	@$(LD) -o $@ $^ $(LDFLAGS) 

epstool: $(EPSTOOL_EXECUTABLE)

dirs:
	@echo Compiler: $(CXX)
	mkdir -p $(DIRS)

################### PARSER COMPILATION #################

obj/frontend/ChaseBaseListener.o: src/frontend/src/DSLParser/ChaseBaseListener.cpp
	@echo Compiling $@
	@$(CXX) $(PARSERFLAGS) $(INCDIRS) -o $@ $<

obj/frontend/ChaseLexer.o: src/frontend/src/DSLParser/ChaseLexer.cpp
	@echo Compiling $@
	@$(CXX) $(PARSERFLAGS) $(INCDIRS) -o $@ $<

obj/frontend/ChaseListener.o: src/frontend/src/DSLParser/ChaseListener.cpp
	@echo Compiling $@
	@$(CXX) $(PARSERFLAGS) $(INCDIRS) -o $@ $<

obj/frontend/ChaseParser.o: src/frontend/src/DSLParser/ChaseParser.cpp
	@echo Compiling $@
	@$(CXX) $(PARSERFLAGS) $(INCDIRS) -o $@ $<

obj/frontend/SpecBuilder.o: src/frontend/src/DSLParser/SpecBuilder.cc
	@echo Compiling $@
	@$(CXX) $(PARSERFLAGS) $(INCDIRS) -o $@ $<

obj/main.o: src/chase/src/main.cc
	@echo Compiling $@
	@$(CXX) $(PARSERFLAGS) $(INCDIRS) -o $@ $<


### COMPILATION FOR PATTERNS2CHASE FRONTEND TOOL

$(PATTERNS2CHASE_EXECUTABLE): obj/Patterns2Chase/patterns2chase.o \
								  $(PATTERNS2CHASE_OBJS) $(CHASE_LIB)
	@echo Linking $@
	@$(LD) $(LDFLAGS) -o $@ $^

obj/Patterns2Chase/patterns2chase.o: src/frontend/src/Patterns2Chase/patterns2chase.cc
	@echo Compiling Patterns2Chase
	@$(CXX) $(CXXFLAGS) $(INCDIR) -o $@ $<

obj/Patterns2Chase/TextualParsing/PatternLibrary.o: \
		src/frontend/src/Patterns2Chase/TextualParsing/PatternLibrary.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) $(INCDIR) -o $@ $<

obj/Patterns2Chase/TextualParsing/EPSPatternLibrary.o: \
		src/frontend/src/Patterns2Chase/TextualParsing/EPSPatternLibrary.cc
	@$(CXX) $(CXXFLAGS) $(INCDIR) -o $@ $<

obj/Patterns2Chase/EPSPatterns/EPSPatterns.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/EPSPatterns.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/TextualParsing/TextualParsing.o: \
		src/frontend/src/Patterns2Chase/TextualParsing/TextualParsing.cc
	@$(CXX) $(CXXFLAGS) $(INCDIR) -o $@ $<


obj/Patterns2Chase/TextualParsing/AorGVisitor.o: \
		src/frontend/src/Patterns2Chase/TextualParsing/AorGVisitor.cc
	@$(CXX) $(CXXFLAGS) $(INCDIR) -o $@ $<

### COMPILATION FOR THE CHASE2TULIP BACKEND
	
obj/GR1/ProgressFinder.o: src/backend/src/GR1/ProgressFinder.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/GR1/SafetyFinder.o: src/backend/src/GR1/SafetyFinder.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/GR1/InitialConditionsFinder.o: \
		src/backend/src/GR1/InitialConditionsFinder.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/GR1/GR1Builder.o: \
		src/backend/src/GR1/GR1Builder.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Backend/Chase2TuLiP/SpinSyntaxWriter.o: \
		src/backend/src/Chase2TuLiP/SpinSyntaxWriter.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Backend/Chase2TuLiP/TuLiPPrinter.o: \
		src/backend/src/Chase2TuLiP/TuLiPPrinter.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Backend/Chase2GR1C/GR1CPrinter.o: \
		src/backend/src/Chase2GR1C/GR1CPrinter.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Backend/Chase2GR1C/GR1CSyntaxWriter.o: \
		src/backend/src/Chase2GR1C/GR1CSyntaxWriter.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Backend/manipulation/ResolveCounters.o: \
		src/backend/src/manipulation/ResolveCounters.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

#COMPILATION FOR EPSTOOL 

$(EPSTOOL_EXECUTABLE): $(EPSTOOL_OBJS) $(PARSER_OBJS) $(ANTLR4_LIB) $(CHASE_LIB)
	@echo Linking $@
	@$(LD) $(LDFLAGS) -o $@ $^

obj/Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.o: \
		src/frontend/src/Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/TextualParsing/EPSTimedPatternLibrary.o: \
		src/frontend/src/Patterns2Chase/TextualParsing/EPSTimedPatternLibrary.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<


obj/Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/NoParallel.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/NoParallel.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/KeepBalanced.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/KeepBalanced.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/KeepConnected.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/KeepConnected.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/KeepPowered.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/KeepPowered.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/UnhealthySources.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/UnhealthySources.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/UnhealthySources_strong.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/UnhealthySources_strong.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/AllowedFailures.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/AllowedFailures.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/PermanentFailures.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/PermanentFailures.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/TimedEPSPatterns.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/TimedEPSPatterns.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/TimedContactors.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/TimedContactors.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/TimedKeepPowered.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/TimedKeepPowered.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Patterns2Chase/EPSPatterns/TimedUnhealthySources.o: \
		src/frontend/src/Patterns2Chase/EPSPatterns/TimedUnhealthySources.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<


obj/epstool/EPSMain.o: \
		src/epstool/src/EPSMain.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<
	
obj/epstool/EPS/EPSTopology.o: \
		src/epstool/src/EPS/EPSTopology.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/epstool/EPS/EPSDefinitions.o: \
		src/epstool/src/EPS/EPSDefinitions.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<




### COMPILATION FOR THE LIBRARY

library: $(CHASE_LIB)

$(CHASE_LIB): $(OBJS) 
	@echo Linking the library $(CHASE_LIB) 
	@ar cr $(CHASE_LIB) $^

obj/Manipulation/findComponents.o: \
		src/chase/src/Manipulation/findComponents.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<


obj/Manipulation/BaseVisitor.o: \
		src/chase/src/Manipulation/BaseVisitor.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Manipulation/GuideVisitor.o: \
		src/chase/src/Manipulation/GuideVisitor.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Manipulation/SimplifyVisitor.o: \
		src/chase/src/Manipulation/SimplifyVisitor.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Variable.o: \
		src/chase/src/Behaviors/Variable.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Constant.o: \
		src/chase/src/Behaviors/Constant.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Function.o: \
		src/chase/src/Behaviors/Function.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/Globals.o: \
		src/chase/src/Behaviors/Logics/Globals.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/WellFormedFormula.o: \
		src/chase/src/Behaviors/Logics/WellFormedFormula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/LogicAtom.o: \
		src/chase/src/Behaviors/Logics/LogicAtom.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/LogicPredicate.o: \
		src/chase/src/Behaviors/Logics/LogicPredicate.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/LogicConstant.o: \
		src/chase/src/Behaviors/Logics/LogicConstant.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/LogicProposition.o: \
		src/chase/src/Behaviors/Logics/LogicProposition.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/BinaryLogicCombination.o: \
		src/chase/src/Behaviors/Logics/BinaryLogicCombination.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/BinaryFormula.o: \
		src/chase/src/Behaviors/Logics/BinaryFormula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/UnaryFormula.o: \
		src/chase/src/Behaviors/Logics/UnaryFormula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/BooleanFormula.o: \
		src/chase/src/Behaviors/Logics/BooleanFormula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/UnaryLogicCombination.o: \
		src/chase/src/Behaviors/Logics/UnaryLogicCombination.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/TemporalFormula.o: \
		src/chase/src/Behaviors/Logics/TemporalFormula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/UnaryTemporalFormula.o: \
		src/chase/src/Behaviors/Logics/UnaryTemporalFormula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/BinaryTemporalFormula.o: \
		src/chase/src/Behaviors/Logics/BinaryTemporalFormula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Logics/GR1Formula.o: \
		src/chase/src/Behaviors/Logics/GR1Formula.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Timing/Counter.o: \
		src/chase/src/Behaviors/Timing/Counter.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Timing/LivenessCounter.o: \
		src/chase/src/Behaviors/Timing/LivenessCounter.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Behaviors/Timing/SafetyCounter.o: \
		src/chase/src/Behaviors/Timing/SafetyCounter.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Contracts/AGContract.o: \
		src/chase/src/Contracts/AGContract.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Contracts/Contract.o: \
		src/chase/src/Contracts/Contract.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/TextBasedPattern.o: \
		src/chase/src/Specification/TextBasedPattern.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/Requirement.o: \
		src/chase/src/Specification/Requirement.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/Problem.o: \
		src/chase/src/Specification/Problem.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/FunctionsLibrary.o: \
		src/chase/src/Specification/FunctionsLibrary.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/RequirementFunction.o: \
		src/chase/src/Specification/RequirementFunction.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/GraphBasedFunctions.o: \
		src/chase/src/Specification/GraphBasedFunctions.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<


obj/Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/AlwaysActiveConnection.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/AlwaysActiveConnection.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/MaxFailures.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/MaxFailures.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/MustDisconnectFailed.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/MustDisconnectFailed.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/NeverConnect.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/NeverConnect.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/NeverDisconnect.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/NeverDisconnect.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/NoFailures.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/NoFailures.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/NoControl.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/NoControl.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<


obj/Specification/BaseFunctionsLibrary/NoRecovery.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/NoRecovery.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/PreferActiveConnection.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/PreferActiveConnection.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/KeepConnectionStable.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/KeepConnectionStable.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/InitialState.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/InitialState.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/InitialCommand.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/InitialCommand.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/SetTimeResolution.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/SetTimeResolution.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/SwitchOnTime.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/SwitchOnTime.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Specification/BaseFunctionsLibrary/SwitchOffTime.o: \
	src/chase/src/Specification/BaseFunctionsLibrary/SwitchOffTime.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Utilities/Utilities.o: \
		src/chase/src/Utilities/Utilities.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Utilities/Graphs/Graphs.o: \
		src/chase/src/Utilities/Graphs/Graphs.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Utilities/Timing/CounterUtilities.o: \
		src/chase/src/Utilities/Timing/CounterUtilities.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Utilities/Portability/Portability.o: \
		src/chase/src/Utilities/Portability/Portability.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<


obj/Architecture/ComponentsGroup.o: \
		src/chase/src/Architecture/ComponentsGroup.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/DomainSpecificConverterType.o: \
		src/chase/src/Architecture/DomainSpecificConverterType.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/DomainSpecificSinkType.o: \
		src/chase/src/Architecture/DomainSpecificSinkType.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/DomainSpecificSwitchType.o: \
		src/chase/src/Architecture/DomainSpecificSwitchType.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/Network.o: \
		src/chase/src/Architecture/Network.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/NetworkConnection.o: \
		src/chase/src/Architecture/NetworkConnection.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/DomainSpecificBusType.o: \
		src/chase/src/Architecture/DomainSpecificBusType.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/DomainSpecificGenericType.o: \
		src/chase/src/Architecture/DomainSpecificGenericType.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/DomainSpecificSourceType.o: \
		src/chase/src/Architecture/DomainSpecificSourceType.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/DomainSpecificType.o: \
		src/chase/src/Architecture/DomainSpecificType.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/NetworkComponent.o: \
		src/chase/src/Architecture/NetworkComponent.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

obj/Architecture/NetworkDefinitions.o: \
		src/chase/src/Architecture/NetworkDefinitions.cc
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

dox:
	@doxygen doc/Doxyfile


clean:
	@echo Cleaning
	@rm -rf $(OBJS) $(CHASE2TULIP_OBJS) $(EPSTOOL_OBJS) $(PATTERNS2CHASE_OBJS)
	@rm -rf $(EXECUTABLE) $(PATTERNS2CHASE_EXECUTABLE) $(CHASE_LIB)
	@rm -rf $(EPSTOOL_EXECUTABLE) $(PARSER_OBJS) obj/chase.o \
	obj/Patterns2Chase/patterns2chase.o obj/frontend/main.o
	@rm -rf $(PARSER)
	@rm -rf doc/apidoc

help:
	@echo Defined targets:
	@echo all : compiles all the sources and build CHASE.
	@echo clean : clean all the builded files. It also cleans the documentations.
	@echo dox : generates the Doxygen-style documentation.

.PHONY: all clean dox help
