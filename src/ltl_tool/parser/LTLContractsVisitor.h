
// Generated from LTLContracts.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "LTLContractsParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by LTLContractsParser.
 */
class  LTLContractsVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LTLContractsParser.
   */
    virtual antlrcpp::Any visitBin_logic_op(LTLContractsParser::Bin_logic_opContext *context) = 0;

    virtual antlrcpp::Any visitUnary_logic_op(LTLContractsParser::Unary_logic_opContext *context) = 0;

    virtual antlrcpp::Any visitUnary_temp_op(LTLContractsParser::Unary_temp_opContext *context) = 0;

    virtual antlrcpp::Any visitBin_temp_op(LTLContractsParser::Bin_temp_opContext *context) = 0;

    virtual antlrcpp::Any visitRelation_op(LTLContractsParser::Relation_opContext *context) = 0;

    virtual antlrcpp::Any visitBin_math_op(LTLContractsParser::Bin_math_opContext *context) = 0;

    virtual antlrcpp::Any visitIntegerKW(LTLContractsParser::IntegerKWContext *context) = 0;

    virtual antlrcpp::Any visitBooleanKW(LTLContractsParser::BooleanKWContext *context) = 0;

    virtual antlrcpp::Any visitVariableKW(LTLContractsParser::VariableKWContext *context) = 0;

    virtual antlrcpp::Any visitConstantKW(LTLContractsParser::ConstantKWContext *context) = 0;

    virtual antlrcpp::Any visitPropositionKw(LTLContractsParser::PropositionKwContext *context) = 0;

    virtual antlrcpp::Any visitIsKw(LTLContractsParser::IsKwContext *context) = 0;

    virtual antlrcpp::Any visitTrueKW(LTLContractsParser::TrueKWContext *context) = 0;

    virtual antlrcpp::Any visitFalseKW(LTLContractsParser::FalseKWContext *context) = 0;

    virtual antlrcpp::Any visitContractKW(LTLContractsParser::ContractKWContext *context) = 0;

    virtual antlrcpp::Any visitAssumptionsKW(LTLContractsParser::AssumptionsKWContext *context) = 0;

    virtual antlrcpp::Any visitGuaranteesKW(LTLContractsParser::GuaranteesKWContext *context) = 0;

    virtual antlrcpp::Any visitNameKw(LTLContractsParser::NameKwContext *context) = 0;

    virtual antlrcpp::Any visitLogic_constant(LTLContractsParser::Logic_constantContext *context) = 0;

    virtual antlrcpp::Any visitTypeKW(LTLContractsParser::TypeKWContext *context) = 0;

    virtual antlrcpp::Any visitLvalue(LTLContractsParser::LvalueContext *context) = 0;

    virtual antlrcpp::Any visitRvalue(LTLContractsParser::RvalueContext *context) = 0;

    virtual antlrcpp::Any visitRelation(LTLContractsParser::RelationContext *context) = 0;

    virtual antlrcpp::Any visitFormula(LTLContractsParser::FormulaContext *context) = 0;

    virtual antlrcpp::Any visitMinus_number(LTLContractsParser::Minus_numberContext *context) = 0;

    virtual antlrcpp::Any visitValue(LTLContractsParser::ValueContext *context) = 0;

    virtual antlrcpp::Any visitAtom(LTLContractsParser::AtomContext *context) = 0;

    virtual antlrcpp::Any visitSingle_formula(LTLContractsParser::Single_formulaContext *context) = 0;

    virtual antlrcpp::Any visitAssumptions(LTLContractsParser::AssumptionsContext *context) = 0;

    virtual antlrcpp::Any visitGuarantees(LTLContractsParser::GuaranteesContext *context) = 0;

    virtual antlrcpp::Any visitDeclaration(LTLContractsParser::DeclarationContext *context) = 0;

    virtual antlrcpp::Any visitContract(LTLContractsParser::ContractContext *context) = 0;

    virtual antlrcpp::Any visitName(LTLContractsParser::NameContext *context) = 0;

    virtual antlrcpp::Any visitSystemSpec(LTLContractsParser::SystemSpecContext *context) = 0;


};

