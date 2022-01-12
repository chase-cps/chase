
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
    ID = 33, INTEGER = 34, DECIMAL = 35, DesignKW = 36, WS = 37, LINE_COMMENT = 38, 
    MULTILINE_COMMENT = 39, AND = 40, OR = 41, NOT = 42, IMPLIES = 43, IFF = 44, 
    COLON = 45, ALWAYS = 46, EVENTUALLY = 47, NEXT = 48, UNTIL = 49, RELEASE = 50, 
    EQ = 51, NEQ = 52, LT = 53, LE = 54, GT = 55, GE = 56, PLUS = 57, MINUS = 58, 
    TIMES = 59, DIVIDE = 60, POWER = 61, MOD = 62, LROUND = 63, RROUND = 64, 
    LSQUARE = 65, RSQUARE = 66, LCURLY = 67, RCURLY = 68, COMMA = 69, DOT = 70, 
    ENDST = 71
  };

  enum {
    RuleDesign_header = 0, RuleDesign_body_element = 1, RuleDesign = 2, 
    RuleBasetype = 3, RuleDistribution_type = 4, RuleDistribution_param = 5, 
    RuleDistribution_params = 6, RuleVartype = 7, RuleDistribution_definition = 8, 
    RuleDistribution_declaration = 9, RuleDistribution = 10, RuleDistribution_instance = 11, 
    RuleControllability = 12, RuleVariable = 13, RuleConstant_definition = 14, 
    RuleConstant = 15, RuleDeclaration = 16, RuleDeclarations = 17, RuleBin_logic_op = 18, 
    RuleUnary_logic_op = 19, RuleUnary_temp_op = 20, RuleBin_temp_op = 21, 
    RuleRelation_op = 22, RuleBin_math_op = 23, RuleNumber = 24, RuleIntegerKW = 25, 
    RuleBooleanKW = 26, RuleRealKW = 27, RuleComplexKW = 28, RuleContractKW = 29, 
    RuleVariableKW = 30, RuleConstantKW = 31, RuleParametricKW = 32, RuleControlledKW = 33, 
    RuleUncontrolledKW = 34, RuleIsKW = 35, RuleInKW = 36, RuleStochasticKW = 37, 
    RuleDistributionKW = 38, RuleComponentKW = 39, RuleInputKW = 40, RuleOutputKW = 41, 
    RulePropositionKW = 42, RuleTrueKW = 43, RuleFalseKW = 44, RuleAssumptionsKW = 45, 
    RuleGuaranteesKW = 46, RuleSystemKW = 47, RuleDiscreteKW = 48, RuleContinuousKW = 49, 
    RuleGaussianKW = 50, RuleCustomKW = 51, RuleMuKW = 52, RuleSigmaKW = 53, 
    RuleHomogeneousKW = 54, RuleFrequenciesKW = 55, RulePrimed_ID = 56, 
    RuleMinus_ID = 57, RuleFunction = 58, RuleMatrix_line = 59, RuleMatrix = 60, 
    RuleValue = 61, RulePair = 62, RuleInterval_leftopen = 63, RuleInterval_rightopen = 64, 
    RuleInterval_fullopen = 65, RuleInterval_closed = 66, RuleInterval = 67, 
    RuleList = 68
  };

  explicit CoCoDeLangParser(antlr4::TokenStream *input);
  ~CoCoDeLangParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class Design_headerContext;
  class Design_body_elementContext;
  class DesignContext;
  class BasetypeContext;
  class Distribution_typeContext;
  class Distribution_paramContext;
  class Distribution_paramsContext;
  class VartypeContext;
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
  class Matrix_lineContext;
  class MatrixContext;
  class ValueContext;
  class PairContext;
  class Interval_leftopenContext;
  class Interval_rightopenContext;
  class Interval_fullopenContext;
  class Interval_closedContext;
  class IntervalContext;
  class ListContext; 

  class  Design_headerContext : public antlr4::ParserRuleContext {
  public:
    Design_headerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DesignKW();
    IsKWContext *isKW();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *COLON();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Design_headerContext* design_header();

  class  Design_body_elementContext : public antlr4::ParserRuleContext {
  public:
    Design_body_elementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationContext *declaration();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Design_body_elementContext* design_body_element();

  class  DesignContext : public antlr4::ParserRuleContext {
  public:
    DesignContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Design_headerContext *design_header();
    std::vector<Design_body_elementContext *> design_body_element();
    Design_body_elementContext* design_body_element(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DesignContext* design();

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
    CustomKWContext *customKW();
    GaussianKWContext *gaussianKW();
    HomogeneousKWContext *homogeneousKW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_typeContext* distribution_type();

  class  Distribution_paramContext : public antlr4::ParserRuleContext {
  public:
    Distribution_paramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *EQ();
    ValueContext *value();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_paramContext* distribution_param();

  class  Distribution_paramsContext : public antlr4::ParserRuleContext {
  public:
    Distribution_paramsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Distribution_paramContext *> distribution_param();
    Distribution_paramContext* distribution_param(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Distribution_paramsContext* distribution_params();

  class  VartypeContext : public antlr4::ParserRuleContext {
  public:
    VartypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BasetypeContext *basetype();
    InKWContext *inKW();
    IntervalContext *interval();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VartypeContext* vartype();

  class  Distribution_definitionContext : public antlr4::ParserRuleContext {
  public:
    Distribution_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Distribution_typeContext *distribution_type();
    antlr4::tree::TerminalNode *LROUND();
    Distribution_paramsContext *distribution_params();
    antlr4::tree::TerminalNode *RROUND();
    antlr4::tree::TerminalNode *COMMA();
    VartypeContext *vartype();


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
    antlr4::tree::TerminalNode *COLON();
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
    VartypeContext *vartype();
    antlr4::tree::TerminalNode *ENDST();
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
    antlr4::tree::TerminalNode *POWER();
    antlr4::tree::TerminalNode *MOD();


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
    MatrixContext *matrix();
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

