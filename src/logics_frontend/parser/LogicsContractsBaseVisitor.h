
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logics_frontend/grammar\LogicsContracts.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "LogicsContractsVisitor.h"


/**
 * This class provides an empty implementation of LogicsContractsVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LogicsContractsBaseVisitor : public LogicsContractsVisitor {
public:

  virtual antlrcpp::Any visitBin_logic_op(LogicsContractsParser::Bin_logic_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_logic_op(LogicsContractsParser::Unary_logic_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_temp_op(LogicsContractsParser::Unary_temp_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBin_temp_op(LogicsContractsParser::Bin_temp_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelation_op(LogicsContractsParser::Relation_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBin_math_op(LogicsContractsParser::Bin_math_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntegerKW(LogicsContractsParser::IntegerKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBooleanKW(LogicsContractsParser::BooleanKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRealKW(LogicsContractsParser::RealKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariableKW(LogicsContractsParser::VariableKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInputKW(LogicsContractsParser::InputKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOutputKW(LogicsContractsParser::OutputKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstantKW(LogicsContractsParser::ConstantKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPropositionKw(LogicsContractsParser::PropositionKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIsKw(LogicsContractsParser::IsKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTrueKW(LogicsContractsParser::TrueKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFalseKW(LogicsContractsParser::FalseKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContractKW(LogicsContractsParser::ContractKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssumptionsKW(LogicsContractsParser::AssumptionsKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGuaranteesKW(LogicsContractsParser::GuaranteesKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNameKw(LogicsContractsParser::NameKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCausality(LogicsContractsParser::CausalityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogic_constant(LogicsContractsParser::Logic_constantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRange(LogicsContractsParser::RangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInteger(LogicsContractsParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReal(LogicsContractsParser::RealContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeKW(LogicsContractsParser::TypeKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(LogicsContractsParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLvalue(LogicsContractsParser::LvalueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRvalue(LogicsContractsParser::RvalueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimed_ID(LogicsContractsParser::Primed_IDContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelation(LogicsContractsParser::RelationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPair(LogicsContractsParser::PairContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_leftopen(LogicsContractsParser::Interval_leftopenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_rightopen(LogicsContractsParser::Interval_rightopenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_fullopen(LogicsContractsParser::Interval_fullopenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_closed(LogicsContractsParser::Interval_closedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval(LogicsContractsParser::IntervalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFormula(LogicsContractsParser::FormulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMinus_number(LogicsContractsParser::Minus_numberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMinus_ID(LogicsContractsParser::Minus_IDContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtom(LogicsContractsParser::AtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSingle_formula(LogicsContractsParser::Single_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssumptions(LogicsContractsParser::AssumptionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGuarantees(LogicsContractsParser::GuaranteesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclaration(LogicsContractsParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContract(LogicsContractsParser::ContractContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitName(LogicsContractsParser::NameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSystemSpec(LogicsContractsParser::SystemSpecContext *ctx) override {
    return visitChildren(ctx);
  }


};

