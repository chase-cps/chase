
// Generated from Chase.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "ChaseListener.h"


/**
 * This class provides an empty implementation of ChaseListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ChaseBaseListener : public ChaseListener {
public:

  virtual void enterChaseSpec(ChaseParser::ChaseSpecContext * /*ctx*/) override { }
  virtual void exitChaseSpec(ChaseParser::ChaseSpecContext * /*ctx*/) override { }

  virtual void enterTypes(ChaseParser::TypesContext * /*ctx*/) override { }
  virtual void exitTypes(ChaseParser::TypesContext * /*ctx*/) override { }

  virtual void enterType(ChaseParser::TypeContext * /*ctx*/) override { }
  virtual void exitType(ChaseParser::TypeContext * /*ctx*/) override { }

  virtual void enterComponents(ChaseParser::ComponentsContext * /*ctx*/) override { }
  virtual void exitComponents(ChaseParser::ComponentsContext * /*ctx*/) override { }

  virtual void enterComponent(ChaseParser::ComponentContext * /*ctx*/) override { }
  virtual void exitComponent(ChaseParser::ComponentContext * /*ctx*/) override { }

  virtual void enterName(ChaseParser::NameContext * /*ctx*/) override { }
  virtual void exitName(ChaseParser::NameContext * /*ctx*/) override { }

  virtual void enterAbbreviations(ChaseParser::AbbreviationsContext * /*ctx*/) override { }
  virtual void exitAbbreviations(ChaseParser::AbbreviationsContext * /*ctx*/) override { }

  virtual void enterAbbrev(ChaseParser::AbbrevContext * /*ctx*/) override { }
  virtual void exitAbbrev(ChaseParser::AbbrevContext * /*ctx*/) override { }

  virtual void enterConnections(ChaseParser::ConnectionsContext * /*ctx*/) override { }
  virtual void exitConnections(ChaseParser::ConnectionsContext * /*ctx*/) override { }

  virtual void enterSwitched(ChaseParser::SwitchedContext * /*ctx*/) override { }
  virtual void exitSwitched(ChaseParser::SwitchedContext * /*ctx*/) override { }

  virtual void enterUnswitched(ChaseParser::UnswitchedContext * /*ctx*/) override { }
  virtual void exitUnswitched(ChaseParser::UnswitchedContext * /*ctx*/) override { }

  virtual void enterConn(ChaseParser::ConnContext * /*ctx*/) override { }
  virtual void exitConn(ChaseParser::ConnContext * /*ctx*/) override { }

  virtual void enterRequirements(ChaseParser::RequirementsContext * /*ctx*/) override { }
  virtual void exitRequirements(ChaseParser::RequirementsContext * /*ctx*/) override { }

  virtual void enterReq(ChaseParser::ReqContext * /*ctx*/) override { }
  virtual void exitReq(ChaseParser::ReqContext * /*ctx*/) override { }

  virtual void enterParam(ChaseParser::ParamContext * /*ctx*/) override { }
  virtual void exitParam(ChaseParser::ParamContext * /*ctx*/) override { }

  virtual void enterAssumptions(ChaseParser::AssumptionsContext * /*ctx*/) override { }
  virtual void exitAssumptions(ChaseParser::AssumptionsContext * /*ctx*/) override { }

  virtual void enterAssum(ChaseParser::AssumContext * /*ctx*/) override { }
  virtual void exitAssum(ChaseParser::AssumContext * /*ctx*/) override { }

  virtual void enterTypesKW(ChaseParser::TypesKWContext * /*ctx*/) override { }
  virtual void exitTypesKW(ChaseParser::TypesKWContext * /*ctx*/) override { }

  virtual void enterComponentsKW(ChaseParser::ComponentsKWContext * /*ctx*/) override { }
  virtual void exitComponentsKW(ChaseParser::ComponentsKWContext * /*ctx*/) override { }

  virtual void enterAbbreviationsKW(ChaseParser::AbbreviationsKWContext * /*ctx*/) override { }
  virtual void exitAbbreviationsKW(ChaseParser::AbbreviationsKWContext * /*ctx*/) override { }

  virtual void enterRequirementsKW(ChaseParser::RequirementsKWContext * /*ctx*/) override { }
  virtual void exitRequirementsKW(ChaseParser::RequirementsKWContext * /*ctx*/) override { }

  virtual void enterConnectionsKW(ChaseParser::ConnectionsKWContext * /*ctx*/) override { }
  virtual void exitConnectionsKW(ChaseParser::ConnectionsKWContext * /*ctx*/) override { }

  virtual void enterAssumptionsKW(ChaseParser::AssumptionsKWContext * /*ctx*/) override { }
  virtual void exitAssumptionsKW(ChaseParser::AssumptionsKWContext * /*ctx*/) override { }

  virtual void enterSwitchedKW(ChaseParser::SwitchedKWContext * /*ctx*/) override { }
  virtual void exitSwitchedKW(ChaseParser::SwitchedKWContext * /*ctx*/) override { }

  virtual void enterUnswitchedKW(ChaseParser::UnswitchedKWContext * /*ctx*/) override { }
  virtual void exitUnswitchedKW(ChaseParser::UnswitchedKWContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

