
// Generated from Chase.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "ChaseParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ChaseParser.
 */
class  ChaseListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterChaseSpec(ChaseParser::ChaseSpecContext *ctx) = 0;
  virtual void exitChaseSpec(ChaseParser::ChaseSpecContext *ctx) = 0;

  virtual void enterTypes(ChaseParser::TypesContext *ctx) = 0;
  virtual void exitTypes(ChaseParser::TypesContext *ctx) = 0;

  virtual void enterType(ChaseParser::TypeContext *ctx) = 0;
  virtual void exitType(ChaseParser::TypeContext *ctx) = 0;

  virtual void enterComponents(ChaseParser::ComponentsContext *ctx) = 0;
  virtual void exitComponents(ChaseParser::ComponentsContext *ctx) = 0;

  virtual void enterComponent(ChaseParser::ComponentContext *ctx) = 0;
  virtual void exitComponent(ChaseParser::ComponentContext *ctx) = 0;

  virtual void enterName(ChaseParser::NameContext *ctx) = 0;
  virtual void exitName(ChaseParser::NameContext *ctx) = 0;

  virtual void enterAbbreviations(ChaseParser::AbbreviationsContext *ctx) = 0;
  virtual void exitAbbreviations(ChaseParser::AbbreviationsContext *ctx) = 0;

  virtual void enterAbbrev(ChaseParser::AbbrevContext *ctx) = 0;
  virtual void exitAbbrev(ChaseParser::AbbrevContext *ctx) = 0;

  virtual void enterConnections(ChaseParser::ConnectionsContext *ctx) = 0;
  virtual void exitConnections(ChaseParser::ConnectionsContext *ctx) = 0;

  virtual void enterSwitched(ChaseParser::SwitchedContext *ctx) = 0;
  virtual void exitSwitched(ChaseParser::SwitchedContext *ctx) = 0;

  virtual void enterUnswitched(ChaseParser::UnswitchedContext *ctx) = 0;
  virtual void exitUnswitched(ChaseParser::UnswitchedContext *ctx) = 0;

  virtual void enterConn(ChaseParser::ConnContext *ctx) = 0;
  virtual void exitConn(ChaseParser::ConnContext *ctx) = 0;

  virtual void enterRequirements(ChaseParser::RequirementsContext *ctx) = 0;
  virtual void exitRequirements(ChaseParser::RequirementsContext *ctx) = 0;

  virtual void enterReq(ChaseParser::ReqContext *ctx) = 0;
  virtual void exitReq(ChaseParser::ReqContext *ctx) = 0;

  virtual void enterParam(ChaseParser::ParamContext *ctx) = 0;
  virtual void exitParam(ChaseParser::ParamContext *ctx) = 0;

  virtual void enterAssumptions(ChaseParser::AssumptionsContext *ctx) = 0;
  virtual void exitAssumptions(ChaseParser::AssumptionsContext *ctx) = 0;

  virtual void enterAssum(ChaseParser::AssumContext *ctx) = 0;
  virtual void exitAssum(ChaseParser::AssumContext *ctx) = 0;

  virtual void enterTypesKW(ChaseParser::TypesKWContext *ctx) = 0;
  virtual void exitTypesKW(ChaseParser::TypesKWContext *ctx) = 0;

  virtual void enterComponentsKW(ChaseParser::ComponentsKWContext *ctx) = 0;
  virtual void exitComponentsKW(ChaseParser::ComponentsKWContext *ctx) = 0;

  virtual void enterAbbreviationsKW(ChaseParser::AbbreviationsKWContext *ctx) = 0;
  virtual void exitAbbreviationsKW(ChaseParser::AbbreviationsKWContext *ctx) = 0;

  virtual void enterRequirementsKW(ChaseParser::RequirementsKWContext *ctx) = 0;
  virtual void exitRequirementsKW(ChaseParser::RequirementsKWContext *ctx) = 0;

  virtual void enterConnectionsKW(ChaseParser::ConnectionsKWContext *ctx) = 0;
  virtual void exitConnectionsKW(ChaseParser::ConnectionsKWContext *ctx) = 0;

  virtual void enterAssumptionsKW(ChaseParser::AssumptionsKWContext *ctx) = 0;
  virtual void exitAssumptionsKW(ChaseParser::AssumptionsKWContext *ctx) = 0;

  virtual void enterSwitchedKW(ChaseParser::SwitchedKWContext *ctx) = 0;
  virtual void exitSwitchedKW(ChaseParser::SwitchedKWContext *ctx) = 0;

  virtual void enterUnswitchedKW(ChaseParser::UnswitchedKWContext *ctx) = 0;
  virtual void exitUnswitchedKW(ChaseParser::UnswitchedKWContext *ctx) = 0;


};

