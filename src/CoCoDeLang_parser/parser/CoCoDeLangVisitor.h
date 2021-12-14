
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/CoCoDeLang_parser/grammar\CoCoDeLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "CoCoDeLangParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by CoCoDeLangParser.
 */
class  CoCoDeLangVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by CoCoDeLangParser.
   */
    virtual antlrcpp::Any visitBasetype(CoCoDeLangParser::BasetypeContext *context) = 0;

    virtual antlrcpp::Any visitDomain(CoCoDeLangParser::DomainContext *context) = 0;

    virtual antlrcpp::Any visitDistribution_type(CoCoDeLangParser::Distribution_typeContext *context) = 0;

    virtual antlrcpp::Any visitFrequency_entry(CoCoDeLangParser::Frequency_entryContext *context) = 0;

    virtual antlrcpp::Any visitFrequency_table(CoCoDeLangParser::Frequency_tableContext *context) = 0;

    virtual antlrcpp::Any visitDistribution_features(CoCoDeLangParser::Distribution_featuresContext *context) = 0;

    virtual antlrcpp::Any visitDistribution_definition(CoCoDeLangParser::Distribution_definitionContext *context) = 0;

    virtual antlrcpp::Any visitDistribution_declaration(CoCoDeLangParser::Distribution_declarationContext *context) = 0;

    virtual antlrcpp::Any visitDistribution(CoCoDeLangParser::DistributionContext *context) = 0;

    virtual antlrcpp::Any visitDistribution_instance(CoCoDeLangParser::Distribution_instanceContext *context) = 0;

    virtual antlrcpp::Any visitControllability(CoCoDeLangParser::ControllabilityContext *context) = 0;

    virtual antlrcpp::Any visitVariable(CoCoDeLangParser::VariableContext *context) = 0;

    virtual antlrcpp::Any visitConstant_definition(CoCoDeLangParser::Constant_definitionContext *context) = 0;

    virtual antlrcpp::Any visitConstant(CoCoDeLangParser::ConstantContext *context) = 0;

    virtual antlrcpp::Any visitDeclaration(CoCoDeLangParser::DeclarationContext *context) = 0;

    virtual antlrcpp::Any visitDeclarations(CoCoDeLangParser::DeclarationsContext *context) = 0;

    virtual antlrcpp::Any visitBin_logic_op(CoCoDeLangParser::Bin_logic_opContext *context) = 0;

    virtual antlrcpp::Any visitUnary_logic_op(CoCoDeLangParser::Unary_logic_opContext *context) = 0;

    virtual antlrcpp::Any visitUnary_temp_op(CoCoDeLangParser::Unary_temp_opContext *context) = 0;

    virtual antlrcpp::Any visitBin_temp_op(CoCoDeLangParser::Bin_temp_opContext *context) = 0;

    virtual antlrcpp::Any visitRelation_op(CoCoDeLangParser::Relation_opContext *context) = 0;

    virtual antlrcpp::Any visitBin_math_op(CoCoDeLangParser::Bin_math_opContext *context) = 0;

    virtual antlrcpp::Any visitNumber(CoCoDeLangParser::NumberContext *context) = 0;

    virtual antlrcpp::Any visitIntegerKW(CoCoDeLangParser::IntegerKWContext *context) = 0;

    virtual antlrcpp::Any visitBooleanKW(CoCoDeLangParser::BooleanKWContext *context) = 0;

    virtual antlrcpp::Any visitRealKW(CoCoDeLangParser::RealKWContext *context) = 0;

    virtual antlrcpp::Any visitComplexKW(CoCoDeLangParser::ComplexKWContext *context) = 0;

    virtual antlrcpp::Any visitContractKW(CoCoDeLangParser::ContractKWContext *context) = 0;

    virtual antlrcpp::Any visitVariableKW(CoCoDeLangParser::VariableKWContext *context) = 0;

    virtual antlrcpp::Any visitConstantKW(CoCoDeLangParser::ConstantKWContext *context) = 0;

    virtual antlrcpp::Any visitParametricKW(CoCoDeLangParser::ParametricKWContext *context) = 0;

    virtual antlrcpp::Any visitControlledKW(CoCoDeLangParser::ControlledKWContext *context) = 0;

    virtual antlrcpp::Any visitUncontrolledKW(CoCoDeLangParser::UncontrolledKWContext *context) = 0;

    virtual antlrcpp::Any visitIsKW(CoCoDeLangParser::IsKWContext *context) = 0;

    virtual antlrcpp::Any visitInKW(CoCoDeLangParser::InKWContext *context) = 0;

    virtual antlrcpp::Any visitStochasticKW(CoCoDeLangParser::StochasticKWContext *context) = 0;

    virtual antlrcpp::Any visitDistributionKW(CoCoDeLangParser::DistributionKWContext *context) = 0;

    virtual antlrcpp::Any visitComponentKW(CoCoDeLangParser::ComponentKWContext *context) = 0;

    virtual antlrcpp::Any visitInputKW(CoCoDeLangParser::InputKWContext *context) = 0;

    virtual antlrcpp::Any visitOutputKW(CoCoDeLangParser::OutputKWContext *context) = 0;

    virtual antlrcpp::Any visitPropositionKW(CoCoDeLangParser::PropositionKWContext *context) = 0;

    virtual antlrcpp::Any visitTrueKW(CoCoDeLangParser::TrueKWContext *context) = 0;

    virtual antlrcpp::Any visitFalseKW(CoCoDeLangParser::FalseKWContext *context) = 0;

    virtual antlrcpp::Any visitAssumptionsKW(CoCoDeLangParser::AssumptionsKWContext *context) = 0;

    virtual antlrcpp::Any visitGuaranteesKW(CoCoDeLangParser::GuaranteesKWContext *context) = 0;

    virtual antlrcpp::Any visitSystemKW(CoCoDeLangParser::SystemKWContext *context) = 0;

    virtual antlrcpp::Any visitDiscreteKW(CoCoDeLangParser::DiscreteKWContext *context) = 0;

    virtual antlrcpp::Any visitContinuousKW(CoCoDeLangParser::ContinuousKWContext *context) = 0;

    virtual antlrcpp::Any visitGaussianKW(CoCoDeLangParser::GaussianKWContext *context) = 0;

    virtual antlrcpp::Any visitMuKW(CoCoDeLangParser::MuKWContext *context) = 0;

    virtual antlrcpp::Any visitSigmaKW(CoCoDeLangParser::SigmaKWContext *context) = 0;

    virtual antlrcpp::Any visitHomogeneousKW(CoCoDeLangParser::HomogeneousKWContext *context) = 0;

    virtual antlrcpp::Any visitFrequenciesKW(CoCoDeLangParser::FrequenciesKWContext *context) = 0;

    virtual antlrcpp::Any visitPrimed_ID(CoCoDeLangParser::Primed_IDContext *context) = 0;

    virtual antlrcpp::Any visitMinus_ID(CoCoDeLangParser::Minus_IDContext *context) = 0;

    virtual antlrcpp::Any visitValue(CoCoDeLangParser::ValueContext *context) = 0;

    virtual antlrcpp::Any visitPair(CoCoDeLangParser::PairContext *context) = 0;

    virtual antlrcpp::Any visitInterval_leftopen(CoCoDeLangParser::Interval_leftopenContext *context) = 0;

    virtual antlrcpp::Any visitInterval_rightopen(CoCoDeLangParser::Interval_rightopenContext *context) = 0;

    virtual antlrcpp::Any visitInterval_fullopen(CoCoDeLangParser::Interval_fullopenContext *context) = 0;

    virtual antlrcpp::Any visitInterval_closed(CoCoDeLangParser::Interval_closedContext *context) = 0;

    virtual antlrcpp::Any visitInterval(CoCoDeLangParser::IntervalContext *context) = 0;

    virtual antlrcpp::Any visitList(CoCoDeLangParser::ListContext *context) = 0;


};

