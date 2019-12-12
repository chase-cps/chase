
// Generated from Chase.g4 by ANTLR 4.5.3

#pragma once


#include "antlr4-runtime.h"


using namespace antlr4;



class ChaseParser : public Parser {
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

  ChaseParser(TokenStream *input);
  ~ChaseParser();

  virtual std::string getGrammarFileName() const override;
  virtual const atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual dfa::Vocabulary& getVocabulary() const override;


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

  class ChaseSpecContext : public ParserRuleContext {
  public:
    ChaseSpecContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<TypesContext> types();
    Ref<ComponentsContext> components();
    Ref<ConnectionsContext> connections();
    Ref<RequirementsContext> requirements();
    Ref<tree::TerminalNode> EOF();
    Ref<AbbreviationsContext> abbreviations();
    Ref<AssumptionsContext> assumptions();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ChaseSpecContext> chaseSpec();

  class TypesContext : public ParserRuleContext {
  public:
    TypesContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<TypesKWContext> typesKW();
    std::vector<Ref<TypeContext>> type();
    Ref<TypeContext> type(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<TypesContext> types();

  class TypeContext : public ParserRuleContext {
  public:
    Ref<ChaseParser::NameContext> nam;
    Token *typ = nullptr;
    Token *idToken = nullptr;
    std::vector<Token *> par;
    TypeContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<tree::TerminalNode> ENDST();
    Ref<NameContext> name();
    std::vector<Ref<tree::TerminalNode>> ID();
    Ref<tree::TerminalNode> ID(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<TypeContext> type();

  class ComponentsContext : public ParserRuleContext {
  public:
    ComponentsContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<ComponentsKWContext> componentsKW();
    std::vector<Ref<ComponentContext>> component();
    Ref<ComponentContext> component(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ComponentsContext> components();

  class ComponentContext : public ParserRuleContext {
  public:
    Ref<ChaseParser::NameContext> nam;
    Token *num = nullptr;
    ComponentContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<tree::TerminalNode> ENDST();
    Ref<NameContext> name();
    Ref<tree::TerminalNode> NUMBER();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ComponentContext> component();

  class NameContext : public ParserRuleContext {
  public:
    Token *idToken = nullptr;
    std::vector<Token *> comp;
    Token *num = nullptr;
    NameContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    std::vector<Ref<tree::TerminalNode>> ID();
    Ref<tree::TerminalNode> ID(int i);
    Ref<tree::TerminalNode> NUMBER();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<NameContext> name();

  class AbbreviationsContext : public ParserRuleContext {
  public:
    AbbreviationsContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<AbbreviationsKWContext> abbreviationsKW();
    std::vector<Ref<AbbrevContext>> abbrev();
    Ref<AbbrevContext> abbrev(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<AbbreviationsContext> abbreviations();

  class AbbrevContext : public ParserRuleContext {
  public:
    Ref<ChaseParser::NameContext> nam;
    Token *abbr = nullptr;
    AbbrevContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<tree::TerminalNode> ENDST();
    Ref<NameContext> name();
    Ref<tree::TerminalNode> ID();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<AbbrevContext> abbrev();

  class ConnectionsContext : public ParserRuleContext {
  public:
    ConnectionsContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<ConnectionsKWContext> connectionsKW();
    std::vector<Ref<ConnContext>> conn();
    Ref<ConnContext> conn(int i);
    std::vector<Ref<SwitchedContext>> switched();
    Ref<SwitchedContext> switched(int i);
    std::vector<Ref<UnswitchedContext>> unswitched();
    Ref<UnswitchedContext> unswitched(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ConnectionsContext> connections();

  class SwitchedContext : public ParserRuleContext {
  public:
    Ref<ChaseParser::NameContext> sw;
    SwitchedContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<SwitchedKWContext> switchedKW();
    Ref<NameContext> name();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<SwitchedContext> switched();

  class UnswitchedContext : public ParserRuleContext {
  public:
    UnswitchedContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<UnswitchedKWContext> unswitchedKW();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<UnswitchedContext> unswitched();

  class ConnContext : public ParserRuleContext {
  public:
    Ref<ChaseParser::NameContext> from;
    Ref<ChaseParser::NameContext> sw;
    Ref<ChaseParser::NameContext> to;
    ConnContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<tree::TerminalNode> ENDST();
    std::vector<Ref<NameContext>> name();
    Ref<NameContext> name(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ConnContext> conn();

  class RequirementsContext : public ParserRuleContext {
  public:
    RequirementsContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<RequirementsKWContext> requirementsKW();
    std::vector<Ref<ReqContext>> req();
    Ref<ReqContext> req(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<RequirementsContext> requirements();

  class ReqContext : public ParserRuleContext {
  public:
    Token *pred = nullptr;
    Ref<ChaseParser::ParamContext> paramContext;
    std::vector<Ref<ParamContext>> par;
    ReqContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<tree::TerminalNode> ENDST();
    Ref<tree::TerminalNode> ID();
    std::vector<Ref<ParamContext>> param();
    Ref<ParamContext> param(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ReqContext> req();

  class ParamContext : public ParserRuleContext {
  public:
    Ref<ChaseParser::NameContext> nam;
    Token *num = nullptr;
    ParamContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<NameContext> name();
    Ref<tree::TerminalNode> NUMBER();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ParamContext> param();

  class AssumptionsContext : public ParserRuleContext {
  public:
    AssumptionsContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<AssumptionsKWContext> assumptionsKW();
    std::vector<Ref<AssumContext>> assum();
    Ref<AssumContext> assum(int i);

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<AssumptionsContext> assumptions();

  class AssumContext : public ParserRuleContext {
  public:
    AssumContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;
    Ref<ReqContext> req();

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<AssumContext> assum();

  class TypesKWContext : public ParserRuleContext {
  public:
    TypesKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<TypesKWContext> typesKW();

  class ComponentsKWContext : public ParserRuleContext {
  public:
    ComponentsKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ComponentsKWContext> componentsKW();

  class AbbreviationsKWContext : public ParserRuleContext {
  public:
    AbbreviationsKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<AbbreviationsKWContext> abbreviationsKW();

  class RequirementsKWContext : public ParserRuleContext {
  public:
    RequirementsKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<RequirementsKWContext> requirementsKW();

  class ConnectionsKWContext : public ParserRuleContext {
  public:
    ConnectionsKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<ConnectionsKWContext> connectionsKW();

  class AssumptionsKWContext : public ParserRuleContext {
  public:
    AssumptionsKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<AssumptionsKWContext> assumptionsKW();

  class SwitchedKWContext : public ParserRuleContext {
  public:
    SwitchedKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<SwitchedKWContext> switchedKW();

  class UnswitchedKWContext : public ParserRuleContext {
  public:
    UnswitchedKWContext(std::weak_ptr<ParserRuleContext> parent, int invokingState);
    virtual ssize_t getRuleIndex() const override;

    virtual void enterRule(tree::ParseTreeListener *listener) override;
    virtual void exitRule(tree::ParseTreeListener *listener) override;
   
  };

  Ref<UnswitchedKWContext> unswitchedKW();


private:
  static std::vector<dfa::DFA> _decisionToDFA;
  static atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static dfa::Vocabulary _vocabulary;
  static atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

