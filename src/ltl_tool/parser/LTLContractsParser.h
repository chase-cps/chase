
// Generated from LTLContracts.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  LTLContractsParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, WS = 16, LINE_COMMENT = 17, AND = 18, OR = 19, NOT = 20, 
    IMPLIES = 21, IFF = 22, ALWAYS = 23, EVENTUALLY = 24, NEXT = 25, UNTIL = 26, 
    EQ = 27, NEQ = 28, LT = 29, LE = 30, GT = 31, GE = 32, PLUS = 33, MINUS = 34, 
    TIMES = 35, DIVIDE = 36, ID = 37, NUMBER = 38, ENDST = 39
  };

  enum {
    RuleBin_logic_op = 0, RuleUnary_logic_op = 1, RuleUnary_temp_op = 2, 
    RuleBin_temp_op = 3, RuleRelation_op = 4, RuleBin_math_op = 5, RuleIntegerKW = 6, 
    RuleBooleanKW = 7, RuleVariableKW = 8, RuleConstantKW = 9, RulePropositionKw = 10, 
    RuleIsKw = 11, RuleTrueKW = 12, RuleFalseKW = 13, RuleContractKW = 14, 
    RuleAssumptionsKW = 15, RuleGuaranteesKW = 16, RuleNameKw = 17, RuleLogic_constant = 18, 
    RuleTypeKW = 19, RuleLvalue = 20, RuleRvalue = 21, RuleRelation = 22, 
    RuleFormula = 23, RuleMinus_number = 24, RuleValue = 25, RuleAtom = 26, 
    RuleSingle_formula = 27, RuleAssumptions = 28, RuleGuarantees = 29, 
    RuleDeclaration = 30, RuleContract = 31, RuleName = 32, RuleSystemSpec = 33
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
  class PropositionKwContext;
  class IsKwContext;
  class TrueKWContext;
  class FalseKWContext;
  class ContractKWContext;
  class AssumptionsKWContext;
  class GuaranteesKWContext;
  class NameKwContext;
  class Logic_constantContext;
  class TypeKWContext;
  class LvalueContext;
  class RvalueContext;
  class RelationContext;
  class FormulaContext;
  class Minus_numberContext;
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


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Bin_logic_opContext* bin_logic_op();

  class  Unary_logic_opContext : public antlr4::ParserRuleContext {
  public:
    Unary_logic_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NOT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Unary_logic_opContext* unary_logic_op();

  class  Unary_temp_opContext : public antlr4::ParserRuleContext {
  public:
    Unary_temp_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALWAYS();
    antlr4::tree::TerminalNode *EVENTUALLY();
    antlr4::tree::TerminalNode *NEXT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Unary_temp_opContext* unary_temp_op();

  class  Bin_temp_opContext : public antlr4::ParserRuleContext {
  public:
    Bin_temp_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNTIL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Bin_math_opContext* bin_math_op();

  class  IntegerKWContext : public antlr4::ParserRuleContext {
  public:
    IntegerKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerKWContext* integerKW();

  class  BooleanKWContext : public antlr4::ParserRuleContext {
  public:
    BooleanKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanKWContext* booleanKW();

  class  VariableKWContext : public antlr4::ParserRuleContext {
  public:
    VariableKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableKWContext* variableKW();

  class  ConstantKWContext : public antlr4::ParserRuleContext {
  public:
    ConstantKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstantKWContext* constantKW();

  class  PropositionKwContext : public antlr4::ParserRuleContext {
  public:
    PropositionKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropositionKwContext* propositionKw();

  class  IsKwContext : public antlr4::ParserRuleContext {
  public:
    IsKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IsKwContext* isKw();

  class  TrueKWContext : public antlr4::ParserRuleContext {
  public:
    TrueKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TrueKWContext* trueKW();

  class  FalseKWContext : public antlr4::ParserRuleContext {
  public:
    FalseKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FalseKWContext* falseKW();

  class  ContractKWContext : public antlr4::ParserRuleContext {
  public:
    ContractKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ContractKWContext* contractKW();

  class  AssumptionsKWContext : public antlr4::ParserRuleContext {
  public:
    AssumptionsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssumptionsKWContext* assumptionsKW();

  class  GuaranteesKWContext : public antlr4::ParserRuleContext {
  public:
    GuaranteesKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GuaranteesKWContext* guaranteesKW();

  class  NameKwContext : public antlr4::ParserRuleContext {
  public:
    NameKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NameKwContext* nameKw();

  class  Logic_constantContext : public antlr4::ParserRuleContext {
  public:
    Logic_constantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TrueKWContext *trueKW();
    FalseKWContext *falseKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Logic_constantContext* logic_constant();

  class  TypeKWContext : public antlr4::ParserRuleContext {
  public:
    TypeKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerKWContext *integerKW();
    BooleanKWContext *booleanKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeKWContext* typeKW();

  class  LvalueContext : public antlr4::ParserRuleContext {
  public:
    LvalueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueContext *value();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LvalueContext* lvalue();

  class  RvalueContext : public antlr4::ParserRuleContext {
  public:
    RvalueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueContext *value();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RvalueContext* rvalue();

  class  RelationContext : public antlr4::ParserRuleContext {
  public:
    RelationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LvalueContext *lvalue();
    Relation_opContext *relation_op();
    RvalueContext *rvalue();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FormulaContext* formula();
  FormulaContext* formula(int precedence);
  class  Minus_numberContext : public antlr4::ParserRuleContext {
  public:
    Minus_numberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *NUMBER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Minus_numberContext* minus_number();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    Bin_math_opContext *bin_math_op();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    Minus_numberContext *minus_number();
    antlr4::tree::TerminalNode *NUMBER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();
  ValueContext* value(int precedence);
  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Logic_constantContext *logic_constant();
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtomContext* atom();

  class  Single_formulaContext : public antlr4::ParserRuleContext {
  public:
    Single_formulaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FormulaContext *formula();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Single_formulaContext* single_formula();

  class  AssumptionsContext : public antlr4::ParserRuleContext {
  public:
    AssumptionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssumptionsKWContext *assumptionsKW();
    std::vector<Single_formulaContext *> single_formula();
    Single_formulaContext* single_formula(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssumptionsContext* assumptions();

  class  GuaranteesContext : public antlr4::ParserRuleContext {
  public:
    GuaranteesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GuaranteesKWContext *guaranteesKW();
    std::vector<Single_formulaContext *> single_formula();
    Single_formulaContext* single_formula(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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
    PropositionKwContext *propositionKw();
    IsKwContext *isKw();
    RelationContext *relation();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationContext* declaration();

  class  ContractContext : public antlr4::ParserRuleContext {
  public:
    ContractContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ContractKWContext *contractKW();
    antlr4::tree::TerminalNode *ID();
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);
    AssumptionsContext *assumptions();
    GuaranteesContext *guarantees();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ContractContext* contract();

  class  NameContext : public antlr4::ParserRuleContext {
  public:
    NameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NameKwContext *nameKw();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

