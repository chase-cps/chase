
// Generated from Chase.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  ChaseParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, WS = 16, ENDST = 17, ID = 18, NUMBER = 19, LINE_COMMENT = 20
  };

  enum {
    RuleChaseSpec = 0, RuleTypes = 1, RuleType = 2, RuleComponents = 3, 
    RuleComponent = 4, RuleName = 5, RuleAbbreviations = 6, RuleAbbrev = 7, 
    RuleConnections = 8, RuleSwitched = 9, RuleUnswitched = 10, RuleConn = 11, 
    RuleRequirements = 12, RuleReq = 13, RuleParam = 14, RuleAssumptions = 15, 
    RuleAssum = 16, RuleTypesKW = 17, RuleComponentsKW = 18, RuleAbbreviationsKW = 19, 
    RuleRequirementsKW = 20, RuleConnectionsKW = 21, RuleAssumptionsKW = 22, 
    RuleSwitchedKW = 23, RuleUnswitchedKW = 24
  };

  ChaseParser(antlr4::TokenStream *input);
  ~ChaseParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ChaseSpecContext;
  class TypesContext;
  class TypeContext;
  class ComponentsContext;
  class ComponentContext;
  class NameContext;
  class AbbreviationsContext;
  class AbbrevContext;
  class ConnectionsContext;
  class SwitchedContext;
  class UnswitchedContext;
  class ConnContext;
  class RequirementsContext;
  class ReqContext;
  class ParamContext;
  class AssumptionsContext;
  class AssumContext;
  class TypesKWContext;
  class ComponentsKWContext;
  class AbbreviationsKWContext;
  class RequirementsKWContext;
  class ConnectionsKWContext;
  class AssumptionsKWContext;
  class SwitchedKWContext;
  class UnswitchedKWContext; 

  class  ChaseSpecContext : public antlr4::ParserRuleContext {
  public:
    ChaseSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypesContext *types();
    ComponentsContext *components();
    ConnectionsContext *connections();
    RequirementsContext *requirements();
    antlr4::tree::TerminalNode *EOF();
    AbbreviationsContext *abbreviations();
    AssumptionsContext *assumptions();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChaseSpecContext* chaseSpec();

  class  TypesContext : public antlr4::ParserRuleContext {
  public:
    TypesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypesKWContext *typesKW();
    std::vector<TypeContext *> type();
    TypeContext* type(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypesContext* types();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    ChaseParser::NameContext *nam = nullptr;
    antlr4::Token *typ = nullptr;
    antlr4::Token *idToken = nullptr;
    std::vector<antlr4::Token *> par;
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENDST();
    NameContext *name();
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeContext* type();

  class  ComponentsContext : public antlr4::ParserRuleContext {
  public:
    ComponentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentsKWContext *componentsKW();
    std::vector<ComponentContext *> component();
    ComponentContext* component(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ComponentsContext* components();

  class  ComponentContext : public antlr4::ParserRuleContext {
  public:
    ChaseParser::NameContext *nam = nullptr;
    antlr4::Token *num = nullptr;
    ComponentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENDST();
    NameContext *name();
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ComponentContext* component();

  class  NameContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *idToken = nullptr;
    std::vector<antlr4::Token *> comp;
    antlr4::Token *num = nullptr;
    NameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NameContext* name();

  class  AbbreviationsContext : public antlr4::ParserRuleContext {
  public:
    AbbreviationsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AbbreviationsKWContext *abbreviationsKW();
    std::vector<AbbrevContext *> abbrev();
    AbbrevContext* abbrev(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AbbreviationsContext* abbreviations();

  class  AbbrevContext : public antlr4::ParserRuleContext {
  public:
    ChaseParser::NameContext *nam = nullptr;
    antlr4::Token *abbr = nullptr;
    AbbrevContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENDST();
    NameContext *name();
    antlr4::tree::TerminalNode *ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AbbrevContext* abbrev();

  class  ConnectionsContext : public antlr4::ParserRuleContext {
  public:
    ConnectionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConnectionsKWContext *connectionsKW();
    std::vector<ConnContext *> conn();
    ConnContext* conn(size_t i);
    std::vector<SwitchedContext *> switched();
    SwitchedContext* switched(size_t i);
    std::vector<UnswitchedContext *> unswitched();
    UnswitchedContext* unswitched(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConnectionsContext* connections();

  class  SwitchedContext : public antlr4::ParserRuleContext {
  public:
    ChaseParser::NameContext *sw = nullptr;
    SwitchedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SwitchedKWContext *switchedKW();
    NameContext *name();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SwitchedContext* switched();

  class  UnswitchedContext : public antlr4::ParserRuleContext {
  public:
    UnswitchedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnswitchedKWContext *unswitchedKW();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UnswitchedContext* unswitched();

  class  ConnContext : public antlr4::ParserRuleContext {
  public:
    ChaseParser::NameContext *from = nullptr;
    ChaseParser::NameContext *sw = nullptr;
    ChaseParser::NameContext *to = nullptr;
    ConnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENDST();
    std::vector<NameContext *> name();
    NameContext* name(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConnContext* conn();

  class  RequirementsContext : public antlr4::ParserRuleContext {
  public:
    RequirementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RequirementsKWContext *requirementsKW();
    std::vector<ReqContext *> req();
    ReqContext* req(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RequirementsContext* requirements();

  class  ReqContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *pred = nullptr;
    ChaseParser::ParamContext *paramContext = nullptr;
    std::vector<ParamContext *> par;
    ReqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENDST();
    antlr4::tree::TerminalNode *ID();
    std::vector<ParamContext *> param();
    ParamContext* param(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ReqContext* req();

  class  ParamContext : public antlr4::ParserRuleContext {
  public:
    ChaseParser::NameContext *nam = nullptr;
    antlr4::Token *num = nullptr;
    ParamContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NameContext *name();
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ParamContext* param();

  class  AssumptionsContext : public antlr4::ParserRuleContext {
  public:
    AssumptionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssumptionsKWContext *assumptionsKW();
    std::vector<AssumContext *> assum();
    AssumContext* assum(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AssumptionsContext* assumptions();

  class  AssumContext : public antlr4::ParserRuleContext {
  public:
    AssumContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReqContext *req();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AssumContext* assum();

  class  TypesKWContext : public antlr4::ParserRuleContext {
  public:
    TypesKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypesKWContext* typesKW();

  class  ComponentsKWContext : public antlr4::ParserRuleContext {
  public:
    ComponentsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ComponentsKWContext* componentsKW();

  class  AbbreviationsKWContext : public antlr4::ParserRuleContext {
  public:
    AbbreviationsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AbbreviationsKWContext* abbreviationsKW();

  class  RequirementsKWContext : public antlr4::ParserRuleContext {
  public:
    RequirementsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RequirementsKWContext* requirementsKW();

  class  ConnectionsKWContext : public antlr4::ParserRuleContext {
  public:
    ConnectionsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConnectionsKWContext* connectionsKW();

  class  AssumptionsKWContext : public antlr4::ParserRuleContext {
  public:
    AssumptionsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AssumptionsKWContext* assumptionsKW();

  class  SwitchedKWContext : public antlr4::ParserRuleContext {
  public:
    SwitchedKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SwitchedKWContext* switchedKW();

  class  UnswitchedKWContext : public antlr4::ParserRuleContext {
  public:
    UnswitchedKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UnswitchedKWContext* unswitchedKW();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

