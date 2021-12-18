
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/CoCoDeLang_parser/grammar\CoCoDeLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  CoCoDeLangParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    ID = 33, INTEGER = 34, DECIMAL = 35, WS = 36, LINE_COMMENT = 37, MULTILINE_COMMENT = 38, 
    AND = 39, OR = 40, NOT = 41, IMPLIES = 42, IFF = 43, COLON = 44, ALWAYS = 45, 
    EVENTUALLY = 46, NEXT = 47, UNTIL = 48, RELEASE = 49, EQ = 50, NEQ = 51, 
    LT = 52, LE = 53, GT = 54, GE = 55, PLUS = 56, MINUS = 57, TIMES = 58, 
    DIVIDE = 59, LROUND = 60, RROUND = 61, LSQUARE = 62, RSQUARE = 63, LCURLY = 64, 
    RCURLY = 65, COMMA = 66, DOT = 67, ENDST = 68
  };

  enum {
    RuleBasetype = 0, RuleDistribution_type = 1, RuleFrequency_entry = 2, 
    RuleFrequency_table = 3, RuleDistribution_features = 4, RuleDistribution_definition = 5, 
    RuleDistribution_declaration = 6, RuleDistribution = 7, RuleDistribution_instance = 8, 
    RuleControllability = 9, RuleVariable = 10, RuleConstant_definition = 11, 
    RuleConstant = 12, RuleDeclaration = 13, RuleDeclarations = 14, RuleBin_logic_op = 15, 
    RuleUnary_logic_op = 16, RuleUnary_temp_op = 17, RuleBin_temp_op = 18, 
    RuleRelation_op = 19, RuleBin_math_op = 20, RuleNumber = 21, RuleIntegerKW = 22, 
    RuleBooleanKW = 23, RuleRealKW = 24, RuleComplexKW = 25, RuleContractKW = 26, 
    RuleVariableKW = 27, RuleConstantKW = 28, RuleParametricKW = 29, RuleControlledKW = 30, 
    RuleUncontrolledKW = 31, RuleIsKW = 32, RuleInKW = 33, RuleStochasticKW = 34, 
    RuleDistributionKW = 35, RuleComponentKW = 36, RuleInputKW = 37, RuleOutputKW = 38, 
    RulePropositionKW = 39, RuleTrueKW = 40, RuleFalseKW = 41, RuleAssumptionsKW = 42, 
    RuleGuaranteesKW = 43, RuleSystemKW = 44, RuleDiscreteKW = 45, RuleContinuousKW = 46, 
    RuleGaussianKW = 47, RuleCustomKW = 48, RuleMuKW = 49, RuleSigmaKW = 50, 
    RuleHomogeneousKW = 51, RuleFrequenciesKW = 52, RulePrimed_ID = 53, 
    RuleMinus_ID = 54, RuleFunction = 55, RuleValue = 56, RulePair = 57, 
    RuleInterval_leftopen = 58, RuleInterval_rightopen = 59, RuleInterval_fullopen = 60, 
    RuleInterval_closed = 61, RuleInterval = 62, RuleList = 63, RuleMatrix_line = 64, 
    RuleMatrix = 65
  };

  explicit CoCoDeLangParser(antlr4::TokenStream *input);
  ~CoCoDeLangParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class BasetypeContext;
  class Distribution_typeContext;
  class Frequency_entryContext;
  class Frequency_tableContext;
  class Distribution_featuresContext;
  class Distribution_definitionContext;
  class Distribution_declarationContext;
  class DistributionContext;
  class Distribution_instanceContext;
  class ControllabilityContext;
  class VariableContext;
  class Constant_definitionContext;
  class ConstantContext;
  class DeclarationContext;
  class DeclarationsContext;
  class Bin_logic_opContext;
  class Unary_logic_opContext;
  class Unary_temp_opContext;
  class Bin_temp_opContext;
  class Relation_opContext;
  class Bin_math_opContext;
  class NumberContext;
  class IntegerKWContext;
  class BooleanKWContext;
  class RealKWContext;
  class ComplexKWContext;
  class ContractKWContext;
  class VariableKWContext;
  class ConstantKWContext;
  class ParametricKWContext;
  class ControlledKWContext;
  class UncontrolledKWContext;
  class IsKWContext;
  class InKWContext;
  class StochasticKWContext;
  class DistributionKWContext;
  class ComponentKWContext;
  class InputKWContext;
  class OutputKWContext;
  class PropositionKWContext;
  class TrueKWContext;
  class FalseKWContext;
  class AssumptionsKWContext;
  class GuaranteesKWContext;
  class SystemKWContext;
  class DiscreteKWContext;
  class ContinuousKWContext;
  class GaussianKWContext;
  class CustomKWContext;
  class MuKWContext;
  class SigmaKWContext;
  class HomogeneousKWContext;
  class FrequenciesKWContext;
  class Primed_IDContext;
  class Minus_IDContext;
  class FunctionContext;
  class ValueContext;
  class PairContext;
  class Interval_leftopenContext;
  class Interval_rightopenContext;
  class Interval_fullopenContext;
  class Interval_closedContext;
  class IntervalContext;
  class ListContext;
  class Matrix_lineContext;
  class MatrixContext; 

  class  BasetypeContext : public antlr4::ParserRuleContext {
  public:
    BasetypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerKWContext *integerKW();
    RealKWContext *realKW();
    BooleanKWContext *booleanKW();
    ComplexKWContext *complexKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BasetypeContext* basetype();

  class  Distribution_typeContext : public antlr4::ParserRuleContext {
  public:
    Distribution_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerKWContext *integerKW();
    IntervalContext *interval();
    RealKWContext *realKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_typeContext* distribution_type();

  class  Frequency_entryContext : public antlr4::ParserRuleContext {
  public:
    Frequency_entryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueContext *value();
    antlr4::tree::TerminalNode *COLON();
    NumberContext *number();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Frequency_entryContext* frequency_entry();

  class  Frequency_tableContext : public antlr4::ParserRuleContext {
  public:
    Frequency_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSQUARE();
    antlr4::tree::TerminalNode *RSQUARE();
    std::vector<Frequency_entryContext *> frequency_entry();
    Frequency_entryContext* frequency_entry(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Frequency_tableContext* frequency_table();

  class  Distribution_featuresContext : public antlr4::ParserRuleContext {
  public:
    Distribution_featuresContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GaussianKWContext *gaussianKW();
    antlr4::tree::TerminalNode *LROUND();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RROUND();
    MuKWContext *muKW();
    std::vector<antlr4::tree::TerminalNode *> EQ();
    antlr4::tree::TerminalNode* EQ(size_t i);
    SigmaKWContext *sigmaKW();
    HomogeneousKWContext *homogeneousKW();
    CustomKWContext *customKW();
    MatrixContext *matrix();
    FrequenciesKWContext *frequenciesKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_featuresContext* distribution_features();

  class  Distribution_definitionContext : public antlr4::ParserRuleContext {
  public:
    Distribution_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Distribution_typeContext *distribution_type();
    antlr4::tree::TerminalNode *COMMA();
    Distribution_featuresContext *distribution_features();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_definitionContext* distribution_definition();

  class  Distribution_declarationContext : public antlr4::ParserRuleContext {
  public:
    Distribution_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DistributionKWContext *distributionKW();
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_declarationContext* distribution_declaration();

  class  DistributionContext : public antlr4::ParserRuleContext {
  public:
    DistributionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Distribution_declarationContext *distribution_declaration();
    IsKWContext *isKW();
    Distribution_definitionContext *distribution_definition();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DistributionContext* distribution();

  class  Distribution_instanceContext : public antlr4::ParserRuleContext {
  public:
    Distribution_instanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    Distribution_definitionContext *distribution_definition();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_instanceContext* distribution_instance();

  class  ControllabilityContext : public antlr4::ParserRuleContext {
  public:
    ControllabilityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ControlledKWContext *controlledKW();
    UncontrolledKWContext *uncontrolledKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ControllabilityContext* controllability();

  class  VariableContext : public antlr4::ParserRuleContext {
  public:
    VariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ControllabilityContext *controllability();
    VariableKWContext *variableKW();
    antlr4::tree::TerminalNode *ID();
    IsKWContext *isKW();
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);
    BasetypeContext *basetype();
    antlr4::tree::TerminalNode *ENDST();
    InKWContext *inKW();
    std::vector<IntervalContext *> interval();
    IntervalContext* interval(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    StochasticKWContext *stochasticKW();
    Distribution_instanceContext *distribution_instance();
    ParametricKWContext *parametricKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableContext* variable();

  class  Constant_definitionContext : public antlr4::ParserRuleContext {
  public:
    Constant_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    TrueKWContext *trueKW();
    FalseKWContext *falseKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Constant_definitionContext* constant_definition();

  class  ConstantContext : public antlr4::ParserRuleContext {
  public:
    ConstantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstantKWContext *constantKW();
    BasetypeContext *basetype();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *COLON();
    Constant_definitionContext *constant_definition();
    antlr4::tree::TerminalNode *ENDST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstantContext* constant();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstantContext *constant();
    VariableContext *variable();
    DistributionContext *distribution();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationContext* declaration();

  class  DeclarationsContext : public antlr4::ParserRuleContext {
  public:
    DeclarationsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationsContext* declarations();

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
    antlr4::tree::TerminalNode *RELEASE();


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

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *DECIMAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumberContext* number();

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

  class  RealKWContext : public antlr4::ParserRuleContext {
  public:
    RealKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RealKWContext* realKW();

  class  ComplexKWContext : public antlr4::ParserRuleContext {
  public:
    ComplexKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComplexKWContext* complexKW();

  class  ContractKWContext : public antlr4::ParserRuleContext {
  public:
    ContractKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ContractKWContext* contractKW();

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

  class  ParametricKWContext : public antlr4::ParserRuleContext {
  public:
    ParametricKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParametricKWContext* parametricKW();

  class  ControlledKWContext : public antlr4::ParserRuleContext {
  public:
    ControlledKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ControlledKWContext* controlledKW();

  class  UncontrolledKWContext : public antlr4::ParserRuleContext {
  public:
    UncontrolledKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UncontrolledKWContext* uncontrolledKW();

  class  IsKWContext : public antlr4::ParserRuleContext {
  public:
    IsKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IsKWContext* isKW();

  class  InKWContext : public antlr4::ParserRuleContext {
  public:
    InKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InKWContext* inKW();

  class  StochasticKWContext : public antlr4::ParserRuleContext {
  public:
    StochasticKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StochasticKWContext* stochasticKW();

  class  DistributionKWContext : public antlr4::ParserRuleContext {
  public:
    DistributionKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DistributionKWContext* distributionKW();

  class  ComponentKWContext : public antlr4::ParserRuleContext {
  public:
    ComponentKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentKWContext* componentKW();

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

  class  PropositionKWContext : public antlr4::ParserRuleContext {
  public:
    PropositionKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropositionKWContext* propositionKW();

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

  class  SystemKWContext : public antlr4::ParserRuleContext {
  public:
    SystemKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SystemKWContext* systemKW();

  class  DiscreteKWContext : public antlr4::ParserRuleContext {
  public:
    DiscreteKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DiscreteKWContext* discreteKW();

  class  ContinuousKWContext : public antlr4::ParserRuleContext {
  public:
    ContinuousKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ContinuousKWContext* continuousKW();

  class  GaussianKWContext : public antlr4::ParserRuleContext {
  public:
    GaussianKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GaussianKWContext* gaussianKW();

  class  CustomKWContext : public antlr4::ParserRuleContext {
  public:
    CustomKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CustomKWContext* customKW();

  class  MuKWContext : public antlr4::ParserRuleContext {
  public:
    MuKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MuKWContext* muKW();

  class  SigmaKWContext : public antlr4::ParserRuleContext {
  public:
    SigmaKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SigmaKWContext* sigmaKW();

  class  HomogeneousKWContext : public antlr4::ParserRuleContext {
  public:
    HomogeneousKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HomogeneousKWContext* homogeneousKW();

  class  FrequenciesKWContext : public antlr4::ParserRuleContext {
  public:
    FrequenciesKWContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FrequenciesKWContext* frequenciesKW();

  class  Primed_IDContext : public antlr4::ParserRuleContext {
  public:
    Primed_IDContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Primed_IDContext* primed_ID();

  class  Minus_IDContext : public antlr4::ParserRuleContext {
  public:
    Minus_IDContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Minus_IDContext* minus_ID();

  class  FunctionContext : public antlr4::ParserRuleContext {
  public:
    FunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionContext* function();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Primed_IDContext *primed_ID();
    antlr4::tree::TerminalNode *ID();
    Minus_IDContext *minus_ID();
    NumberContext *number();
    antlr4::tree::TerminalNode *LROUND();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *RROUND();
    FunctionContext *function();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    TrueKWContext *trueKW();
    FalseKWContext *falseKW();
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
    antlr4::tree::TerminalNode *LROUND();
    PairContext *pair();
    antlr4::tree::TerminalNode *RSQUARE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Interval_leftopenContext* interval_leftopen();

  class  Interval_rightopenContext : public antlr4::ParserRuleContext {
  public:
    Interval_rightopenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSQUARE();
    PairContext *pair();
    antlr4::tree::TerminalNode *RROUND();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Interval_rightopenContext* interval_rightopen();

  class  Interval_fullopenContext : public antlr4::ParserRuleContext {
  public:
    Interval_fullopenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LROUND();
    PairContext *pair();
    antlr4::tree::TerminalNode *RROUND();


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

  class  ListContext : public antlr4::ParserRuleContext {
  public:
    ListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LCURLY();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *RCURLY();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ListContext* list();

  class  Matrix_lineContext : public antlr4::ParserRuleContext {
  public:
    Matrix_lineContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Matrix_lineContext* matrix_line();

  class  MatrixContext : public antlr4::ParserRuleContext {
  public:
    MatrixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSQUARE();
    std::vector<Matrix_lineContext *> matrix_line();
    Matrix_lineContext* matrix_line(size_t i);
    antlr4::tree::TerminalNode *RSQUARE();
    std::vector<antlr4::tree::TerminalNode *> ENDST();
    antlr4::tree::TerminalNode* ENDST(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatrixContext* matrix();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
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

