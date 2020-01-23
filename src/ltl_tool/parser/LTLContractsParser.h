
// Generated from LTLContracts.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  LTLContractsParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, WS = 14, 
    LINE_COMMENT = 15, NUMBER = 16, AND = 17, OR = 18, NOT = 19, IMPLIES = 20, 
    IFF = 21, ALWAYS = 22, EVENTUALLY = 23, NEXT = 24, UNTIL = 25, EQ = 26, 
    NEQ = 27, LT = 28, LE = 29, GT = 30, GE = 31, PLUS = 32, MINUS = 33, 
    TIMES = 34, DIVIDE = 35, ID = 36, ENDST = 37
  };

  enum {
    RuleBin_logic_op = 0, RuleUnary_logic_op = 1, RuleUnary_temp_op = 2, 
    RuleBin_temp_op = 3, RuleRelation_op = 4, RuleBin_math_op = 5, RuleIntegerKW = 6, 
    RuleBooleanKW = 7, RuleVariableKW = 8, RuleConstantKW = 9, RuleTrueKW = 10, 
    RuleFalseKW = 11, RuleContractKW = 12, RuleAssumptionsKW = 13, RuleGuaranteesKW = 14, 
    RuleNameKw = 15, RuleLogic_constant = 16, RuleTypeKW = 17, RuleRelation = 18, 
    RuleFormula = 19, RuleValue = 20, RuleAtom = 21, RuleSingle_formula = 22, 
    RuleAssumptions = 23, RuleGuarantees = 24, RuleDeclaration = 25, RuleContract = 26, 
    RuleName = 27, RuleSystemSpec = 28
  };

  LTLContractsParser(antlr4::TokenStream *input);
  ~LTLContractsParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class Bin_logic_opContext;
  class Unary_logic_opContext;
  class Unary_temp_opContext;
  class Bin_temp_opContext;
  class Relation_opContext;
  class Bin_math_opContext;
  class IntegerKWContext;
  class BooleanKWContext;
  class VariableKWContext;
  class ConstantKWContext;
  class TrueKWContext;
  class FalseKWContext;
  class ContractKWContext;
  class AssumptionsKWContext;
  class GuaranteesKWContext;
  class NameKwContext;
  class Logic_constantContext;
  class TypeKWContext;
  class RelationContext;
  class FormulaContext;
  class ValueContext;
  class AtomContext;
  class Single_formulaContext;
  class AssumptionsContext;
  class GuaranteesContext;
  class DeclarationContext;
  class ContractContext;
  class NameContext;
  class SystemSpecContext; 

  class  Bin_logic_opContext : public antlr4::ParserRuleContext {
  public:
    Bin_logic_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *IFF();
    antlr4::tree::TerminalNode *IMPLIES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Bin_logic_opContext* bin_logic_op();

  class  Unary_logic_opContext : public antlr4::ParserRuleContext {
  public:
    Unary_logic_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Unary_logic_opContext* unary_logic_op();

  class  Unary_temp_opContext : public antlr4::ParserRuleContext {
  public:
    Unary_temp_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALWAYS();
    antlr4::tree::TerminalNode *EVENTUALLY();
    antlr4::tree::TerminalNode *NEXT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Unary_temp_opContext* unary_temp_op();

  class  Bin_temp_opContext : public antlr4::ParserRuleContext {
  public:
    Bin_temp_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNTIL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Bin_temp_opContext* bin_temp_op();

  class  Relation_opContext : public antlr4::ParserRuleContext {
  public:
    Relation_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *GE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Relation_opContext* relation_op();

  class  Bin_math_opContext : public antlr4::ParserRuleContext {
  public:
    Bin_math_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIMES();
    antlr4::tree::TerminalNode *DIVIDE();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Bin_math_opContext* bin_math_op();

  class  IntegerKWContext : public antlr4::ParserRuleContext {
  public:
    IntegerKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IntegerKWContext* integerKW();

  class  BooleanKWContext : public antlr4::ParserRuleContext {
  public:
    BooleanKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BooleanKWContext* booleanKW();

  class  VariableKWContext : public antlr4::ParserRuleContext {
  public:
    VariableKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  VariableKWContext* variableKW();

  class  ConstantKWContext : public antlr4::ParserRuleContext {
  public:
    ConstantKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstantKWContext* constantKW();

  class  TrueKWContext : public antlr4::ParserRuleContext {
  public:
    TrueKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TrueKWContext* trueKW();

  class  FalseKWContext : public antlr4::ParserRuleContext {
  public:
    FalseKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FalseKWContext* falseKW();

  class  ContractKWContext : public antlr4::ParserRuleContext {
  public:
    ContractKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ContractKWContext* contractKW();

  class  AssumptionsKWContext : public antlr4::ParserRuleContext {
  public:
    AssumptionsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AssumptionsKWContext* assumptionsKW();

  class  GuaranteesKWContext : public antlr4::ParserRuleContext {
  public:
    GuaranteesKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GuaranteesKWContext* guaranteesKW();

  class  NameKwContext : public antlr4::ParserRuleContext {
  public:
    NameKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NameKwContext* nameKw();

  class  Logic_constantContext : public antlr4::ParserRuleContext {
  public:
    Logic_constantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TrueKWContext *trueKW();
    FalseKWContext *falseKW();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Logic_constantContext* logic_constant();

  class  TypeKWContext : public antlr4::ParserRuleContext {
  public:
    TypeKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerKWContext *integerKW();
    BooleanKWContext *booleanKW();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeKWContext* typeKW();

  class  RelationContext : public antlr4::ParserRuleContext {
  public:
    RelationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    Relation_opContext *relation_op();
    RelationContext *relation();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RelationContext* relation();

  class  FormulaContext : public antlr4::ParserRuleContext {
  public:
    FormulaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Unary_logic_opContext *unary_logic_op();
    std::vector<FormulaContext *> formula();
    FormulaContext* formula(size_t i);
    Unary_temp_opContext *unary_temp_op();
    AtomContext *atom();
    Bin_logic_opContext *bin_logic_op();
    Bin_temp_opContext *bin_temp_op();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FormulaContext* formula();
  FormulaContext* formula(int precedence);
  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *NUMBER();
    Bin_math_opContext *bin_math_op();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValueContext* value();
  ValueContext* value(int precedence);
  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Logic_constantContext *logic_constant();
    RelationContext *relation();
    antlr4::tree::TerminalNode *ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AtomContext* atom();

  class  Single_formulaContext : public antlr4::ParserRuleContext {
  public:
    Single_formulaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FormulaContext *formula();
    antlr4::tree::TerminalNode *ENDST();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Single_formulaContext* single_formula();

  class  AssumptionsContext : public antlr4::ParserRuleContext {
  public:
    AssumptionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssumptionsKWContext *assumptionsKW();
    std::vector<Single_formulaContext *> single_formula();
    Single_formulaContext* single_formula(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AssumptionsContext* assumptions();

  class  GuaranteesContext : public antlr4::ParserRuleContext {
  public:
    GuaranteesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GuaranteesKWContext *guaranteesKW();
    std::vector<Single_formulaContext *> single_formula();
    Single_formulaContext* single_formula(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GuaranteesContext* guarantees();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VariableKWContext *variableKW();
    antlr4::tree::TerminalNode *ID();
    TypeKWContext *typeKW();
    antlr4::tree::TerminalNode *ENDST();
    ConstantKWContext *constantKW();
    IntegerKWContext *integerKW();
    antlr4::tree::TerminalNode *NUMBER();
    BooleanKWContext *booleanKW();
    Logic_constantContext *logic_constant();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DeclarationContext* declaration();

  class  ContractContext : public antlr4::ParserRuleContext {
  public:
    ContractContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ContractKWContext *contractKW();
    antlr4::tree::TerminalNode *ID();
    AssumptionsContext *assumptions();
    GuaranteesContext *guarantees();
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ContractContext* contract();

  class  NameContext : public antlr4::ParserRuleContext {
  public:
    NameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NameKwContext *nameKw();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *ENDST();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NameContext* name();

  class  SystemSpecContext : public antlr4::ParserRuleContext {
  public:
    SystemSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NameContext *name();
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);
    std::vector<ContractContext *> contract();
    ContractContext* contract(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SystemSpecContext* systemSpec();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool formulaSempred(FormulaContext *_localctx, size_t predicateIndex);
  bool valueSempred(ValueContext *_localctx, size_t predicateIndex);

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

