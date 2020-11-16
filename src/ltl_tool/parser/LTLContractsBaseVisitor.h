
// Generated from //wsl$/Ubuntu-20.04/home/lora/software/chase/repo/src/ltl_tool/grammar\LTLContracts.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "LTLContractsVisitor.h"


/**
 * This class provides an empty implementation of LTLContractsVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LTLContractsBaseVisitor : public LTLContractsVisitor {
public:

  virtual antlrcpp::Any visitBin_logic_op(LTLContractsParser::Bin_logic_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_logic_op(LTLContractsParser::Unary_logic_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_temp_op(LTLContractsParser::Unary_temp_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBin_temp_op(LTLContractsParser::Bin_temp_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelation_op(LTLContractsParser::Relation_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBin_math_op(LTLContractsParser::Bin_math_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntegerKW(LTLContractsParser::IntegerKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBooleanKW(LTLContractsParser::BooleanKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariableKW(LTLContractsParser::VariableKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInputKW(LTLContractsParser::InputKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOutputKW(LTLContractsParser::OutputKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstantKW(LTLContractsParser::ConstantKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPropositionKw(LTLContractsParser::PropositionKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIsKw(LTLContractsParser::IsKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTrueKW(LTLContractsParser::TrueKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFalseKW(LTLContractsParser::FalseKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContractKW(LTLContractsParser::ContractKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssumptionsKW(LTLContractsParser::AssumptionsKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGuaranteesKW(LTLContractsParser::GuaranteesKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNameKw(LTLContractsParser::NameKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCausality(LTLContractsParser::CausalityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogic_constant(LTLContractsParser::Logic_constantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRange(LTLContractsParser::RangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeKW(LTLContractsParser::TypeKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLvalue(LTLContractsParser::LvalueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRvalue(LTLContractsParser::RvalueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelation(LTLContractsParser::RelationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFormula(LTLContractsParser::FormulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMinus_number(LTLContractsParser::Minus_numberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(LTLContractsParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtom(LTLContractsParser::AtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSingle_formula(LTLContractsParser::Single_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssumptions(LTLContractsParser::AssumptionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGuarantees(LTLContractsParser::GuaranteesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclaration(LTLContractsParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContract(LTLContractsParser::ContractContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitName(LTLContractsParser::NameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSystemSpec(LTLContractsParser::SystemSpecContext *ctx) override {
    return visitChildren(ctx);
  }


};

