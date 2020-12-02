/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/26/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "../../../include/chase/utilities/LogicNotNormalizationVisitor.hh"
#include "representation.hh"
#include "utilities/Factory.hh"
#include "representation/Expression.hh"


#include "utilities/simplify.hh"

using namespace chase;

LogicNotNormalizationVisitor::LogicNotNormalizationVisitor() :
    LogicSimplificationVisitor()
{
}

LogicNotNormalizationVisitor::~LogicNotNormalizationVisitor() = default;



LogicFormula *
LogicNotNormalizationVisitor::_analyzeFormula(LogicFormula *formula)
{
    if(formula->IsA() == unaryBooleanOperation_node)
    {
        auto outer_not = static_cast<UnaryBooleanFormula*>(formula);
        if(outer_not->getOp() == op_not) {
            auto f = outer_not->getOp1();
            // Two annidated not.
            if (f->IsA() == unaryBooleanOperation_node) {
                auto inner_not = static_cast< UnaryBooleanFormula * >(f);
                if (inner_not->getOp() == op_not) {
                    return inner_not->getOp1();
                }
            }
            // Binary boolean operation.
            if(f->IsA() == binaryBooleanOperation_node)
            {
                auto inner_bin = static_cast< BinaryBooleanFormula * >(f);
                auto op = inner_bin->getOp();
                switch(op)
                {
                    case op_and:
                        return Or(
                                Not(inner_bin->getOp1()),
                                Not(inner_bin->getOp2()));
                    case op_or:
                        return And(
                                Not(inner_bin->getOp1()),
                                Not(inner_bin->getOp2()));
                    case op_implies:
                        return And(inner_bin->getOp1(),
                                Not(inner_bin->getOp2()));
                    case op_iff:
                        return Or(
                                And(inner_bin->getOp1(),
                                    Not(inner_bin->getOp2())),
                                And(inner_bin->getOp2()->clone(),
                                    Not(inner_bin->getOp1()->clone()))
                                );
                    case op_xor:
                        return Or(
                                And(inner_bin->getOp1(), inner_bin->getOp2()),
                                And(Not(inner_bin->getOp1()->clone()),
                                        Not(inner_bin->getOp2()->clone()))
                                );
                    case op_xnor:
                        return Or(
                                And(
                                        inner_bin->getOp1(),
                                        Not(inner_bin->getOp2())),
                                And(Not(
                                        inner_bin->getOp1()->clone()),
                                        inner_bin->getOp2()->clone()
                                        ));
                    default:
                        break;
                }
            }
            // Not with temporal operator within.
            if (f->IsA() == unaryTemporalOperation_node)
            {
                auto inner_temporal = static_cast< UnaryTemporalFormula * >(f);
                auto op = inner_temporal->getOp();
                switch(op) {
                    case op_globally :
                        return Eventually(Not(inner_temporal->getFormula()));
                    case op_future:
                        return Always(Not(inner_temporal->getFormula()));
                    case op_next:
                        return Next(Not(inner_temporal->getFormula()));
                    default:
                        break;
                }
            }
            if( f->IsA() == large_boolean_formula_node )
            {
                auto inner = static_cast< LargeBooleanFormula * >(f);

                for(size_t i = 0; i < inner->operands.size(); ++i)
                {
                    inner->operands[i] =
                            Not(inner->operands[i]);
                    simplify(inner->operands[i]);
                }
                auto op = inner->getOp();
                switch(op) {
                    case op_and:
                        inner->setOp(op_or);
                        break;
                    case op_or:
                        inner->setOp(op_and);
                        break;
                    default:
                        break;
                }
                return inner;
            }
            if (f->IsA() == proposition_node)
            {
                auto inner_proposition = static_cast< Proposition * >(f);
                if(inner_proposition->getValue()->IsA() == expression_node)
                {
                    auto exp = static_cast< Expression * >(
                            inner_proposition->getValue());
                    Operator op = exp->getOperator();
                    Operator nop = op;
                    switch(op)
                    {
                        case op_eq:
                            nop = op_neq;
                            break;
                        case op_neq:
                            nop = op_eq;
                            break;
                        case op_ge:
                            nop = op_lt;
                            break;
                        case op_gt:
                            nop = op_le;
                            break;
                        case op_lt:
                            nop = op_ge;
                            break;
                        case op_le:
                            nop = op_gt;
                            break;
                        default:
                            break;
                    }
                    if(op != nop)
                    {
                         return
                            new Proposition(
                                    new Expression(
                                            nop,
                                            exp->getOp1(),
                                            exp->getOp2()));
                    }
                }
            }
        }
    }
    return formula;
}
