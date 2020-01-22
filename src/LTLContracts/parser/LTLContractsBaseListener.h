
// Generated from LTLContracts.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "LTLContractsListener.h"


/**
 * This class provides an empty implementation of LTLContractsListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  LTLContractsBaseListener : public LTLContractsListener {
public:

  virtual void enterIntegerKW(LTLContractsParser::IntegerKWContext * /*ctx*/) override { }
  virtual void exitIntegerKW(LTLContractsParser::IntegerKWContext * /*ctx*/) override { }

  virtual void enterBooleanKW(LTLContractsParser::BooleanKWContext * /*ctx*/) override { }
  virtual void exitBooleanKW(LTLContractsParser::BooleanKWContext * /*ctx*/) override { }

  virtual void enterVariableKW(LTLContractsParser::VariableKWContext * /*ctx*/) override { }
  virtual void exitVariableKW(LTLContractsParser::VariableKWContext * /*ctx*/) override { }

  virtual void enterConstantKW(LTLContractsParser::ConstantKWContext * /*ctx*/) override { }
  virtual void exitConstantKW(LTLContractsParser::ConstantKWContext * /*ctx*/) override { }

  virtual void enterTrueKW(LTLContractsParser::TrueKWContext * /*ctx*/) override { }
  virtual void exitTrueKW(LTLContractsParser::TrueKWContext * /*ctx*/) override { }

  virtual void enterFalseKW(LTLContractsParser::FalseKWContext * /*ctx*/) override { }
  virtual void exitFalseKW(LTLContractsParser::FalseKWContext * /*ctx*/) override { }

  virtual void enterContractKW(LTLContractsParser::ContractKWContext * /*ctx*/) override { }
  virtual void exitContractKW(LTLContractsParser::ContractKWContext * /*ctx*/) override { }

  virtual void enterAssumptionsKW(LTLContractsParser::AssumptionsKWContext * /*ctx*/) override { }
  virtual void exitAssumptionsKW(LTLContractsParser::AssumptionsKWContext * /*ctx*/) override { }

  virtual void enterGuaranteesKW(LTLContractsParser::GuaranteesKWContext * /*ctx*/) override { }
  virtual void exitGuaranteesKW(LTLContractsParser::GuaranteesKWContext * /*ctx*/) override { }

  virtual void enterLogic_constant(LTLContractsParser::Logic_constantContext * /*ctx*/) override { }
  virtual void exitLogic_constant(LTLContractsParser::Logic_constantContext * /*ctx*/) override { }

  virtual void enterTypeKW(LTLContractsParser::TypeKWContext * /*ctx*/) override { }
  virtual void exitTypeKW(LTLContractsParser::TypeKWContext * /*ctx*/) override { }

  virtual void enterBin_logic_op(LTLContractsParser::Bin_logic_opContext * /*ctx*/) override { }
  virtual void exitBin_logic_op(LTLContractsParser::Bin_logic_opContext * /*ctx*/) override { }

  virtual void enterUnary_logic_op(LTLContractsParser::Unary_logic_opContext * /*ctx*/) override { }
  virtual void exitUnary_logic_op(LTLContractsParser::Unary_logic_opContext * /*ctx*/) override { }

  virtual void enterUnary_temp_op(LTLContractsParser::Unary_temp_opContext * /*ctx*/) override { }
  virtual void exitUnary_temp_op(LTLContractsParser::Unary_temp_opContext * /*ctx*/) override { }

  virtual void enterBin_temp_op(LTLContractsParser::Bin_temp_opContext * /*ctx*/) override { }
  virtual void exitBin_temp_op(LTLContractsParser::Bin_temp_opContext * /*ctx*/) override { }

  virtual void enterRelation_op(LTLContractsParser::Relation_opContext * /*ctx*/) override { }
  virtual void exitRelation_op(LTLContractsParser::Relation_opContext * /*ctx*/) override { }

  virtual void enterBin_math_op(LTLContractsParser::Bin_math_opContext * /*ctx*/) override { }
  virtual void exitBin_math_op(LTLContractsParser::Bin_math_opContext * /*ctx*/) override { }

  virtual void enterValue(LTLContractsParser::ValueContext * /*ctx*/) override { }
  virtual void exitValue(LTLContractsParser::ValueContext * /*ctx*/) override { }

  virtual void enterRelation(LTLContractsParser::RelationContext * /*ctx*/) override { }
  virtual void exitRelation(LTLContractsParser::RelationContext * /*ctx*/) override { }

  virtual void enterAtom(LTLContractsParser::AtomContext * /*ctx*/) override { }
  virtual void exitAtom(LTLContractsParser::AtomContext * /*ctx*/) override { }

  virtual void enterProp_formula(LTLContractsParser::Prop_formulaContext * /*ctx*/) override { }
  virtual void exitProp_formula(LTLContractsParser::Prop_formulaContext * /*ctx*/) override { }

  virtual void enterFormula(LTLContractsParser::FormulaContext * /*ctx*/) override { }
  virtual void exitFormula(LTLContractsParser::FormulaContext * /*ctx*/) override { }

  virtual void enterSingle_formula(LTLContractsParser::Single_formulaContext * /*ctx*/) override { }
  virtual void exitSingle_formula(LTLContractsParser::Single_formulaContext * /*ctx*/) override { }

  virtual void enterAssumptions(LTLContractsParser::AssumptionsContext * /*ctx*/) override { }
  virtual void exitAssumptions(LTLContractsParser::AssumptionsContext * /*ctx*/) override { }

  virtual void enterGuarantees(LTLContractsParser::GuaranteesContext * /*ctx*/) override { }
  virtual void exitGuarantees(LTLContractsParser::GuaranteesContext * /*ctx*/) override { }

  virtual void enterDeclaration(LTLContractsParser::DeclarationContext * /*ctx*/) override { }
  virtual void exitDeclaration(LTLContractsParser::DeclarationContext * /*ctx*/) override { }

  virtual void enterContract(LTLContractsParser::ContractContext * /*ctx*/) override { }
  virtual void exitContract(LTLContractsParser::ContractContext * /*ctx*/) override { }

  virtual void enterProblem(LTLContractsParser::ProblemContext * /*ctx*/) override { }
  virtual void exitProblem(LTLContractsParser::ProblemContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

