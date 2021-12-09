
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/CoCoDeLang_parser/grammar\CoCoDeLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "CoCoDeLangVisitor.h"


/**
 * This class provides an empty implementation of CoCoDeLangVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  CoCoDeLangBaseVisitor : public CoCoDeLangVisitor {
public:

  virtual antlrcpp::Any visitBasetype(CoCoDeLangParser::BasetypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDomain(CoCoDeLangParser::DomainContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDistribution_type(CoCoDeLangParser::Distribution_typeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDistribution_features(CoCoDeLangParser::Distribution_featuresContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDistribution_definition(CoCoDeLangParser::Distribution_definitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDistribution_declaration(CoCoDeLangParser::Distribution_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDistribution(CoCoDeLangParser::DistributionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDistribution_instance(CoCoDeLangParser::Distribution_instanceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitControllability(CoCoDeLangParser::ControllabilityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariable(CoCoDeLangParser::VariableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstant_definition(CoCoDeLangParser::Constant_definitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstant(CoCoDeLangParser::ConstantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBin_logic_op(CoCoDeLangParser::Bin_logic_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_logic_op(CoCoDeLangParser::Unary_logic_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_temp_op(CoCoDeLangParser::Unary_temp_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBin_temp_op(CoCoDeLangParser::Bin_temp_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelation_op(CoCoDeLangParser::Relation_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBin_math_op(CoCoDeLangParser::Bin_math_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumber(CoCoDeLangParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntegerKW(CoCoDeLangParser::IntegerKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBooleanKW(CoCoDeLangParser::BooleanKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRealKW(CoCoDeLangParser::RealKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComplexKW(CoCoDeLangParser::ComplexKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContractKW(CoCoDeLangParser::ContractKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariableKW(CoCoDeLangParser::VariableKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstantKW(CoCoDeLangParser::ConstantKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParametricKW(CoCoDeLangParser::ParametricKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitControlledKW(CoCoDeLangParser::ControlledKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUncontrolledKW(CoCoDeLangParser::UncontrolledKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIsKW(CoCoDeLangParser::IsKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStochasticKW(CoCoDeLangParser::StochasticKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDistributionKW(CoCoDeLangParser::DistributionKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComponentKW(CoCoDeLangParser::ComponentKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInputKW(CoCoDeLangParser::InputKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOutputKW(CoCoDeLangParser::OutputKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPropositionKW(CoCoDeLangParser::PropositionKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTrueKW(CoCoDeLangParser::TrueKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFalseKW(CoCoDeLangParser::FalseKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssumptionsKW(CoCoDeLangParser::AssumptionsKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGuaranteesKW(CoCoDeLangParser::GuaranteesKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSystemKW(CoCoDeLangParser::SystemKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGaussianKW(CoCoDeLangParser::GaussianKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMuKW(CoCoDeLangParser::MuKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSigmaKW(CoCoDeLangParser::SigmaKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHomogeneousKW(CoCoDeLangParser::HomogeneousKWContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimed_ID(CoCoDeLangParser::Primed_IDContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMinus_ID(CoCoDeLangParser::Minus_IDContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(CoCoDeLangParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPair(CoCoDeLangParser::PairContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_leftopen(CoCoDeLangParser::Interval_leftopenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_rightopen(CoCoDeLangParser::Interval_rightopenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_fullopen(CoCoDeLangParser::Interval_fullopenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval_closed(CoCoDeLangParser::Interval_closedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInterval(CoCoDeLangParser::IntervalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitList(CoCoDeLangParser::ListContext *ctx) override {
    return visitChildren(ctx);
  }


};

