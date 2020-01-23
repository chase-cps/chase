
// Generated from ltl_tool.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "LTLContractsParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by LTLContractsParser.
 */
class  LTLContractsListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterIntegerKW(LTLContractsParser::IntegerKWContext *ctx) = 0;
  virtual void exitIntegerKW(LTLContractsParser::IntegerKWContext *ctx) = 0;

  virtual void enterBooleanKW(LTLContractsParser::BooleanKWContext *ctx) = 0;
  virtual void exitBooleanKW(LTLContractsParser::BooleanKWContext *ctx) = 0;

  virtual void enterVariableKW(LTLContractsParser::VariableKWContext *ctx) = 0;
  virtual void exitVariableKW(LTLContractsParser::VariableKWContext *ctx) = 0;

  virtual void enterConstantKW(LTLContractsParser::ConstantKWContext *ctx) = 0;
  virtual void exitConstantKW(LTLContractsParser::ConstantKWContext *ctx) = 0;

  virtual void enterTrueKW(LTLContractsParser::TrueKWContext *ctx) = 0;
  virtual void exitTrueKW(LTLContractsParser::TrueKWContext *ctx) = 0;

  virtual void enterFalseKW(LTLContractsParser::FalseKWContext *ctx) = 0;
  virtual void exitFalseKW(LTLContractsParser::FalseKWContext *ctx) = 0;

  virtual void enterContractKW(LTLContractsParser::ContractKWContext *ctx) = 0;
  virtual void exitContractKW(LTLContractsParser::ContractKWContext *ctx) = 0;

  virtual void enterAssumptionsKW(LTLContractsParser::AssumptionsKWContext *ctx) = 0;
  virtual void exitAssumptionsKW(LTLContractsParser::AssumptionsKWContext *ctx) = 0;

  virtual void enterGuaranteesKW(LTLContractsParser::GuaranteesKWContext *ctx) = 0;
  virtual void exitGuaranteesKW(LTLContractsParser::GuaranteesKWContext *ctx) = 0;

  virtual void enterLogic_constant(LTLContractsParser::Logic_constantContext *ctx) = 0;
  virtual void exitLogic_constant(LTLContractsParser::Logic_constantContext *ctx) = 0;

  virtual void enterTypeKW(LTLContractsParser::TypeKWContext *ctx) = 0;
  virtual void exitTypeKW(LTLContractsParser::TypeKWContext *ctx) = 0;

  virtual void enterBin_logic_op(LTLContractsParser::Bin_logic_opContext *ctx) = 0;
  virtual void exitBin_logic_op(LTLContractsParser::Bin_logic_opContext *ctx) = 0;

  virtual void enterUnary_logic_op(LTLContractsParser::Unary_logic_opContext *ctx) = 0;
  virtual void exitUnary_logic_op(LTLContractsParser::Unary_logic_opContext *ctx) = 0;

  virtual void enterUnary_temp_op(LTLContractsParser::Unary_temp_opContext *ctx) = 0;
  virtual void exitUnary_temp_op(LTLContractsParser::Unary_temp_opContext *ctx) = 0;

  virtual void enterBin_temp_op(LTLContractsParser::Bin_temp_opContext *ctx) = 0;
  virtual void exitBin_temp_op(LTLContractsParser::Bin_temp_opContext *ctx) = 0;

  virtual void enterRelation_op(LTLContractsParser::Relation_opContext *ctx) = 0;
  virtual void exitRelation_op(LTLContractsParser::Relation_opContext *ctx) = 0;

  virtual void enterBin_math_op(LTLContractsParser::Bin_math_opContext *ctx) = 0;
  virtual void exitBin_math_op(LTLContractsParser::Bin_math_opContext *ctx) = 0;

  virtual void enterValue(LTLContractsParser::ValueContext *ctx) = 0;
  virtual void exitValue(LTLContractsParser::ValueContext *ctx) = 0;

  virtual void enterRelation(LTLContractsParser::RelationContext *ctx) = 0;
  virtual void exitRelation(LTLContractsParser::RelationContext *ctx) = 0;

  virtual void enterAtom(LTLContractsParser::AtomContext *ctx) = 0;
  virtual void exitAtom(LTLContractsParser::AtomContext *ctx) = 0;

  virtual void enterProp_formula(LTLContractsParser::Prop_formulaContext *ctx) = 0;
  virtual void exitProp_formula(LTLContractsParser::Prop_formulaContext *ctx) = 0;

  virtual void enterFormula(LTLContractsParser::FormulaContext *ctx) = 0;
  virtual void exitFormula(LTLContractsParser::FormulaContext *ctx) = 0;

  virtual void enterSingle_formula(LTLContractsParser::Single_formulaContext *ctx) = 0;
  virtual void exitSingle_formula(LTLContractsParser::Single_formulaContext *ctx) = 0;

  virtual void enterAssumptions(LTLContractsParser::AssumptionsContext *ctx) = 0;
  virtual void exitAssumptions(LTLContractsParser::AssumptionsContext *ctx) = 0;

  virtual void enterGuarantees(LTLContractsParser::GuaranteesContext *ctx) = 0;
  virtual void exitGuarantees(LTLContractsParser::GuaranteesContext *ctx) = 0;

  virtual void enterDeclaration(LTLContractsParser::DeclarationContext *ctx) = 0;
  virtual void exitDeclaration(LTLContractsParser::DeclarationContext *ctx) = 0;

  virtual void enterContract(LTLContractsParser::ContractContext *ctx) = 0;
  virtual void exitContract(LTLContractsParser::ContractContext *ctx) = 0;

  virtual void enterProblem(LTLContractsParser::ProblemContext *ctx) = 0;
  virtual void exitProblem(LTLContractsParser::ProblemContext *ctx) = 0;


};

