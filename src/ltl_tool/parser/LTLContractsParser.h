
// Generated from //wsl$/Ubuntu-20.04/home/lora/software/chase/repo/src/ltl_tool/grammar\LTLContracts.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"




class  LTLContractsParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, WS = 16, LINE_COMMENT = 17, AND = 18, OR = 19, NOT = 20, 
    IMPLIES = 21, IFF = 22, COLON = 23, ALWAYS = 24, EVENTUALLY = 25, NEXT = 26, 
    UNTIL = 27, EQ = 28, NEQ = 29, LT = 30, LE = 31, GT = 32, GE = 33, PLUS = 34, 
    MINUS = 35, TIMES = 36, DIVIDE = 37, LBRACKET = 38, RBRACKET = 39, LSQUARE = 40, 
    RSQUARE = 41, COMMA = 42, DOT = 43, ID = 44, DECIMAL = 45, NUMBER = 46, 
    ENDST = 47
  };

  enum {
    RuleBin_logic_op = 0, RuleUnary_logic_op = 1, RuleUnary_temp_op = 2, 
    RuleBin_temp_op = 3, RuleRelation_op = 4, RuleBin_math_op = 5, RulePrimed_ID = 6, 
    RuleIntegerKW = 7, RuleBooleanKW = 8, RuleVariableKW = 9, RuleInputKW = 10, 
    RuleOutputKW = 11, RuleConstantKW = 12, RulePropositionKw = 13, RuleIsKw = 14, 
    RuleTrueKW = 15, RuleFalseKW = 16, RuleContractKW = 17, RuleAssumptionsKW = 18, 
    RuleGuaranteesKW = 19, RuleNameKw = 20, RuleCausality = 21, RuleLogic_constant = 22, 
    RuleRange = 23, RuleInteger = 24, RuleTypeKW = 25, RuleLvalue = 26, 
    RuleRvalue = 27, RuleRelation = 28, RuleValue = 29, RulePair = 30, RuleInterval_leftopen = 31, 
    RuleInterval_rightopen = 32, RuleInterval_fullopen = 33, RuleInterval_closed = 34, 
    RuleInterval = 35, RuleFormula = 36, RuleMinus_number = 37, RuleMinus_ID = 38, 
    RuleAtom = 39, RuleSingle_formula = 40, RuleAssumptions = 41, RuleGuarantees = 42, 
    RuleDeclaration = 43, RuleContract = 44, RuleName = 45, RuleSystemSpec = 46
  };

  explicit LTLContractsParser(antlr4::TokenStream *input);
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
  class Primed_IDContext;
  class IntegerKWContext;
  class BooleanKWContext;
  class VariableKWContext;
  class InputKWContext;
  class OutputKWContext;
  class ConstantKWContext;
  class PropositionKwContext;
  class IsKwContext;
  class TrueKWContext;
  class FalseKWContext;
  class ContractKWContext;
  class AssumptionsKWContext;
  class GuaranteesKWContext;
  class NameKwContext;
  class CausalityContext;
  class Logic_constantContext;
  class RangeContext;
  class IntegerContext;
  class TypeKWContext;
  class LvalueContext;
  class RvalueContext;
  class RelationContext;
  class ValueContext;
  class PairContext;
  class Interval_leftopenContext;
  class Interval_rightopenContext;
  class Interval_fullopenContext;
  class Interval_closedContext;
  class IntervalContext;
  class FormulaContext;
  class Minus_numberContext;
  class Minus_IDContext;
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

  class  Primed_IDContext : public antlr4::ParserRuleContext {
  public:
    Primed_IDContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Primed_IDContext* primed_ID();

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

  class  InputKWContext : public antlr4::ParserRuleContext {
  public:
    InputKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InputKWContext* inputKW();

  class  OutputKWContext : public antlr4::ParserRuleContext {
  public:
    OutputKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OutputKWContext* outputKW();

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

  class  CausalityContext : public antlr4::ParserRuleContext {
  public:
    CausalityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InputKWContext *inputKW();
    OutputKWContext *outputKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CausalityContext* causality();

  class  Logic_constantContext : public antlr4::ParserRuleContext {
  public:
    Logic_constantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TrueKWContext *trueKW();
    FalseKWContext *falseKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Logic_constantContext* logic_constant();

  class  RangeContext : public antlr4::ParserRuleContext {
  public:
    RangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET();
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *RBRACKET();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RangeContext* range();

  class  IntegerContext : public antlr4::ParserRuleContext {
  public:
    IntegerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerKWContext *integerKW();
    RangeContext *range();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerContext* integer();

  class  TypeKWContext : public antlr4::ParserRuleContext {
  public:
    TypeKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
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

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Primed_IDContext *primed_ID();
    antlr4::tree::TerminalNode *ID();
    Minus_IDContext *minus_ID();
    Minus_numberContext *minus_number();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *LBRACKET();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *RBRACKET();
    Bin_math_opContext *bin_math_op();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();
  ValueContext* value(int precedence);
  class  PairContext : public antlr4::ParserRuleContext {
  public:
    PairContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *COMMA();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PairContext* pair();

  class  Interval_leftopenContext : public antlr4::ParserRuleContext {
  public:
    Interval_leftopenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> RSQUARE();
    antlr4::tree::TerminalNode* RSQUARE(size_t i);
    PairContext *pair();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Interval_leftopenContext* interval_leftopen();

  class  Interval_rightopenContext : public antlr4::ParserRuleContext {
  public:
    Interval_rightopenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> LSQUARE();
    antlr4::tree::TerminalNode* LSQUARE(size_t i);
    PairContext *pair();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Interval_rightopenContext* interval_rightopen();

  class  Interval_fullopenContext : public antlr4::ParserRuleContext {
  public:
    Interval_fullopenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RSQUARE();
    PairContext *pair();
    antlr4::tree::TerminalNode *LSQUARE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Interval_fullopenContext* interval_fullopen();

  class  Interval_closedContext : public antlr4::ParserRuleContext {
  public:
    Interval_closedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSQUARE();
    PairContext *pair();
    antlr4::tree::TerminalNode *RSQUARE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Interval_closedContext* interval_closed();

  class  IntervalContext : public antlr4::ParserRuleContext {
  public:
    IntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Interval_closedContext *interval_closed();
    Interval_fullopenContext *interval_fullopen();
    Interval_leftopenContext *interval_leftopen();
    Interval_rightopenContext *interval_rightopen();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntervalContext* interval();

  class  FormulaContext : public antlr4::ParserRuleContext {
  public:
    FormulaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET();
    std::vector<FormulaContext *> formula();
    FormulaContext* formula(size_t i);
    antlr4::tree::TerminalNode *RBRACKET();
    Unary_logic_opContext *unary_logic_op();
    Unary_temp_opContext *unary_temp_op();
    IntervalContext *interval();
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

  class  Minus_IDContext : public antlr4::ParserRuleContext {
  public:
    Minus_IDContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Minus_IDContext* minus_ID();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Logic_constantContext *logic_constant();
    RelationContext *relation();
    antlr4::tree::TerminalNode *ID();
    Primed_IDContext *primed_ID();


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
    antlr4::tree::TerminalNode *COLON();
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
    antlr4::tree::TerminalNode *COLON();
    std::vector<Single_formulaContext *> single_formula();
    Single_formulaContext* single_formula(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GuaranteesContext* guarantees();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeKWContext *typeKW();
    VariableKWContext *variableKW();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *ENDST();
    CausalityContext *causality();
    ConstantKWContext *constantKW();
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
    antlr4::tree::TerminalNode *COLON();
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
    antlr4::tree::TerminalNode *COLON();
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
  bool valueSempred(ValueContext *_localctx, size_t predicateIndex);
  bool formulaSempred(FormulaContext *_localctx, size_t predicateIndex);

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

