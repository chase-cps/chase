
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logistics/grammar\LogisticsLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  LogisticsLangParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, WS = 13, LINE_COMMENT = 14, 
    COLON = 15, COMMA = 16, DOT = 17, EQ = 18, ENDST = 19, LBRACKET = 20, 
    RBRACKET = 21, ID = 22, DECIMAL = 23, NUMBER = 24, ARROW_PRE = 25, ARROW_POST = 26
  };

  enum {
    RuleWidgetsKw = 0, RuleComponentsKw = 1, RuleCrossroadsKw = 2, RuleArchitectureKw = 3, 
    RuleIsKw = 4, RuleRequirementsKw = 5, RuleProducesKw = 6, RuleRequiresKw = 7, 
    RuleBinKw = 8, RuleMachineKw = 9, RuleSinkKw = 10, RuleNothingKw = 11, 
    RuleWidgets = 12, RuleItem = 13, RuleBin = 14, RuleSink = 15, RuleRequire = 16, 
    RuleProduce = 17, RuleMachineSpec = 18, RuleMachine = 19, RuleComponent = 20, 
    RuleComponents = 21, RuleCrossroad = 22, RuleCrossroads = 23, RuleRoad = 24, 
    RuleConnection = 25, RuleArchitecture = 26, RuleRequirement = 27, RuleRequirements = 28, 
    RuleSpec = 29
  };

  explicit LogisticsLangParser(antlr4::TokenStream *input);
  ~LogisticsLangParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class WidgetsKwContext;
  class ComponentsKwContext;
  class CrossroadsKwContext;
  class ArchitectureKwContext;
  class IsKwContext;
  class RequirementsKwContext;
  class ProducesKwContext;
  class RequiresKwContext;
  class BinKwContext;
  class MachineKwContext;
  class SinkKwContext;
  class NothingKwContext;
  class WidgetsContext;
  class ItemContext;
  class BinContext;
  class SinkContext;
  class RequireContext;
  class ProduceContext;
  class MachineSpecContext;
  class MachineContext;
  class ComponentContext;
  class ComponentsContext;
  class CrossroadContext;
  class CrossroadsContext;
  class RoadContext;
  class ConnectionContext;
  class ArchitectureContext;
  class RequirementContext;
  class RequirementsContext;
  class SpecContext; 

  class  WidgetsKwContext : public antlr4::ParserRuleContext {
  public:
    WidgetsKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WidgetsKwContext* widgetsKw();

  class  ComponentsKwContext : public antlr4::ParserRuleContext {
  public:
    ComponentsKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentsKwContext* componentsKw();

  class  CrossroadsKwContext : public antlr4::ParserRuleContext {
  public:
    CrossroadsKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CrossroadsKwContext* crossroadsKw();

  class  ArchitectureKwContext : public antlr4::ParserRuleContext {
  public:
    ArchitectureKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArchitectureKwContext* architectureKw();

  class  IsKwContext : public antlr4::ParserRuleContext {
  public:
    IsKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IsKwContext* isKw();

  class  RequirementsKwContext : public antlr4::ParserRuleContext {
  public:
    RequirementsKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequirementsKwContext* requirementsKw();

  class  ProducesKwContext : public antlr4::ParserRuleContext {
  public:
    ProducesKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProducesKwContext* producesKw();

  class  RequiresKwContext : public antlr4::ParserRuleContext {
  public:
    RequiresKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequiresKwContext* requiresKw();

  class  BinKwContext : public antlr4::ParserRuleContext {
  public:
    BinKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BinKwContext* binKw();

  class  MachineKwContext : public antlr4::ParserRuleContext {
  public:
    MachineKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MachineKwContext* machineKw();

  class  SinkKwContext : public antlr4::ParserRuleContext {
  public:
    SinkKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SinkKwContext* sinkKw();

  class  NothingKwContext : public antlr4::ParserRuleContext {
  public:
    NothingKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NothingKwContext* nothingKw();

  class  WidgetsContext : public antlr4::ParserRuleContext {
  public:
    WidgetsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    WidgetsKwContext *widgetsKw();
    antlr4::tree::TerminalNode *COLON();
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    antlr4::tree::TerminalNode *ENDST();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WidgetsContext* widgets();

  class  ItemContext : public antlr4::ParserRuleContext {
  public:
    ItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ItemContext* item();

  class  BinContext : public antlr4::ParserRuleContext {
  public:
    BinContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    IsKwContext *isKw();
    BinKwContext *binKw();
    antlr4::tree::TerminalNode *LBRACKET();
    std::vector<ItemContext *> item();
    ItemContext* item(size_t i);
    antlr4::tree::TerminalNode *RBRACKET();
    antlr4::tree::TerminalNode *ENDST();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BinContext* bin();

  class  SinkContext : public antlr4::ParserRuleContext {
  public:
    SinkContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    IsKwContext *isKw();
    SinkKwContext *sinkKw();
    antlr4::tree::TerminalNode *LBRACKET();
    std::vector<ItemContext *> item();
    ItemContext* item(size_t i);
    antlr4::tree::TerminalNode *RBRACKET();
    antlr4::tree::TerminalNode *ENDST();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SinkContext* sink();

  class  RequireContext : public antlr4::ParserRuleContext {
  public:
    RequireContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RequiresKwContext *requiresKw();
    antlr4::tree::TerminalNode *COLON();
    std::vector<ItemContext *> item();
    ItemContext* item(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    NothingKwContext *nothingKw();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequireContext* require();

  class  ProduceContext : public antlr4::ParserRuleContext {
  public:
    ProduceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProducesKwContext *producesKw();
    antlr4::tree::TerminalNode *COLON();
    std::vector<ItemContext *> item();
    ItemContext* item(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    NothingKwContext *nothingKw();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProduceContext* produce();

  class  MachineSpecContext : public antlr4::ParserRuleContext {
  public:
    MachineSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProduceContext *produce();
    antlr4::tree::TerminalNode *ENDST();
    RequireContext *require();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MachineSpecContext* machineSpec();

  class  MachineContext : public antlr4::ParserRuleContext {
  public:
    MachineContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    IsKwContext *isKw();
    MachineKwContext *machineKw();
    antlr4::tree::TerminalNode *LBRACKET();
    MachineSpecContext *machineSpec();
    antlr4::tree::TerminalNode *RBRACKET();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MachineContext* machine();

  class  ComponentContext : public antlr4::ParserRuleContext {
  public:
    ComponentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MachineContext *machine();
    SinkContext *sink();
    BinContext *bin();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentContext* component();

  class  ComponentsContext : public antlr4::ParserRuleContext {
  public:
    ComponentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentsKwContext *componentsKw();
    antlr4::tree::TerminalNode *COLON();
    std::vector<ComponentContext *> component();
    ComponentContext* component(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentsContext* components();

  class  CrossroadContext : public antlr4::ParserRuleContext {
  public:
    CrossroadContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CrossroadContext* crossroad();

  class  CrossroadsContext : public antlr4::ParserRuleContext {
  public:
    CrossroadsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CrossroadsKwContext *crossroadsKw();
    antlr4::tree::TerminalNode *COLON();
    std::vector<CrossroadContext *> crossroad();
    CrossroadContext* crossroad(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CrossroadsContext* crossroads();

  class  RoadContext : public antlr4::ParserRuleContext {
  public:
    RoadContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ARROW_PRE();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *ARROW_POST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RoadContext* road();

  class  ConnectionContext : public antlr4::ParserRuleContext {
  public:
    ConnectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    RoadContext *road();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConnectionContext* connection();

  class  ArchitectureContext : public antlr4::ParserRuleContext {
  public:
    ArchitectureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ArchitectureKwContext *architectureKw();
    antlr4::tree::TerminalNode *COLON();
    std::vector<ConnectionContext *> connection();
    ConnectionContext* connection(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArchitectureContext* architecture();

  class  RequirementContext : public antlr4::ParserRuleContext {
  public:
    RequirementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequirementContext* requirement();

  class  RequirementsContext : public antlr4::ParserRuleContext {
  public:
    RequirementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RequirementsKwContext *requirementsKw();
    antlr4::tree::TerminalNode *COLON();
    std::vector<RequirementContext *> requirement();
    RequirementContext* requirement(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequirementsContext* requirements();

  class  SpecContext : public antlr4::ParserRuleContext {
  public:
    SpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    WidgetsContext *widgets();
    ComponentsContext *components();
    CrossroadsContext *crossroads();
    ArchitectureContext *architecture();
    RequirementsContext *requirements();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SpecContext* spec();


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

