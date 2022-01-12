
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logics_frontend/grammar\LogicsContracts.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "LogicsContractsParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by LogicsContractsParser.
 */
class  LogicsContractsVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LogicsContractsParser.
   */
    virtual antlrcpp::Any visitBin_logic_op(LogicsContractsParser::Bin_logic_opContext *context) = 0;

    virtual antlrcpp::Any visitUnary_logic_op(LogicsContractsParser::Unary_logic_opContext *context) = 0;

    virtual antlrcpp::Any visitUnary_temp_op(LogicsContractsParser::Unary_temp_opContext *context) = 0;

    virtual antlrcpp::Any visitBin_temp_op(LogicsContractsParser::Bin_temp_opContext *context) = 0;

    virtual antlrcpp::Any visitRelation_op(LogicsContractsParser::Relation_opContext *context) = 0;

    virtual antlrcpp::Any visitBin_math_op(LogicsContractsParser::Bin_math_opContext *context) = 0;

    virtual antlrcpp::Any visitIntegerKW(LogicsContractsParser::IntegerKWContext *context) = 0;

    virtual antlrcpp::Any visitBooleanKW(LogicsContractsParser::BooleanKWContext *context) = 0;

    virtual antlrcpp::Any visitRealKW(LogicsContractsParser::RealKWContext *context) = 0;

    virtual antlrcpp::Any visitVariableKW(LogicsContractsParser::VariableKWContext *context) = 0;

    virtual antlrcpp::Any visitInputKW(LogicsContractsParser::InputKWContext *context) = 0;

    virtual antlrcpp::Any visitOutputKW(LogicsContractsParser::OutputKWContext *context) = 0;

    virtual antlrcpp::Any visitConstantKW(LogicsContractsParser::ConstantKWContext *context) = 0;

    virtual antlrcpp::Any visitPropositionKw(LogicsContractsParser::PropositionKwContext *context) = 0;

    virtual antlrcpp::Any visitIsKw(LogicsContractsParser::IsKwContext *context) = 0;

    virtual antlrcpp::Any visitTrueKW(LogicsContractsParser::TrueKWContext *context) = 0;

    virtual antlrcpp::Any visitFalseKW(LogicsContractsParser::FalseKWContext *context) = 0;

    virtual antlrcpp::Any visitContractKW(LogicsContractsParser::ContractKWContext *context) = 0;

    virtual antlrcpp::Any visitAssumptionsKW(LogicsContractsParser::AssumptionsKWContext *context) = 0;

    virtual antlrcpp::Any visitGuaranteesKW(LogicsContractsParser::GuaranteesKWContext *context) = 0;

    virtual antlrcpp::Any visitNameKw(LogicsContractsParser::NameKwContext *context) = 0;

    virtual antlrcpp::Any visitCausality(LogicsContractsParser::CausalityContext *context) = 0;

    virtual antlrcpp::Any visitLogic_constant(LogicsContractsParser::Logic_constantContext *context) = 0;

    virtual antlrcpp::Any visitRange(LogicsContractsParser::RangeContext *context) = 0;

    virtual antlrcpp::Any visitInteger(LogicsContractsParser::IntegerContext *context) = 0;

    virtual antlrcpp::Any visitReal(LogicsContractsParser::RealContext *context) = 0;

    virtual antlrcpp::Any visitTypeKW(LogicsContractsParser::TypeKWContext *context) = 0;

    virtual antlrcpp::Any visitValue(LogicsContractsParser::ValueContext *context) = 0;

    virtual antlrcpp::Any visitLvalue(LogicsContractsParser::LvalueContext *context) = 0;

    virtual antlrcpp::Any visitRvalue(LogicsContractsParser::RvalueContext *context) = 0;

    virtual antlrcpp::Any visitPrimed_ID(LogicsContractsParser::Primed_IDContext *context) = 0;

    virtual antlrcpp::Any visitRelation(LogicsContractsParser::RelationContext *context) = 0;

    virtual antlrcpp::Any visitPair(LogicsContractsParser::PairContext *context) = 0;

    virtual antlrcpp::Any visitInterval_leftopen(LogicsContractsParser::Interval_leftopenContext *context) = 0;

    virtual antlrcpp::Any visitInterval_rightopen(LogicsContractsParser::Interval_rightopenContext *context) = 0;

    virtual antlrcpp::Any visitInterval_fullopen(LogicsContractsParser::Interval_fullopenContext *context) = 0;

    virtual antlrcpp::Any visitInterval_closed(LogicsContractsParser::Interval_closedContext *context) = 0;

    virtual antlrcpp::Any visitInterval(LogicsContractsParser::IntervalContext *context) = 0;

    virtual antlrcpp::Any visitFormula(LogicsContractsParser::FormulaContext *context) = 0;

    virtual antlrcpp::Any visitMinus_number(LogicsContractsParser::Minus_numberContext *context) = 0;

    virtual antlrcpp::Any visitMinus_ID(LogicsContractsParser::Minus_IDContext *context) = 0;

    virtual antlrcpp::Any visitAtom(LogicsContractsParser::AtomContext *context) = 0;

    virtual antlrcpp::Any visitSingle_formula(LogicsContractsParser::Single_formulaContext *context) = 0;

    virtual antlrcpp::Any visitAssumptions(LogicsContractsParser::AssumptionsContext *context) = 0;

    virtual antlrcpp::Any visitGuarantees(LogicsContractsParser::GuaranteesContext *context) = 0;

    virtual antlrcpp::Any visitDeclaration(LogicsContractsParser::DeclarationContext *context) = 0;

    virtual antlrcpp::Any visitContract(LogicsContractsParser::ContractContext *context) = 0;

    virtual antlrcpp::Any visitName(LogicsContractsParser::NameContext *context) = 0;

    virtual antlrcpp::Any visitSystemSpec(LogicsContractsParser::SystemSpecContext *context) = 0;


};

