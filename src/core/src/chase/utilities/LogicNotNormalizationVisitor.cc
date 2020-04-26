/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/26/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "../../../include/chase/utilities/LogicNotNormalizationVisitor.hh"
#include "representation.hh"
#include "utilities/Factory.hh"

using namespace chase;

LogicNotNormalizationVisitor::LogicNotNormalizationVisitor() :
    GuideVisitor(0)
{
}

LogicNotNormalizationVisitor::~LogicNotNormalizationVisitor() = default;

int LogicNotNormalizationVisitor::visitBinaryBooleanOperation(
        BinaryBooleanFormula &formula)
{
    int rv = formula.getOp1()->accept_visitor(*this);
    rv |= formula.getOp2()->accept_visitor(*this);

    auto op1 = formula.getOp1();
    formula.setOp1(_analyzeFormula(op1));
    auto op2 = formula.getOp2();
    formula.setOp2(_analyzeFormula(op2));

    rv |= formula.getOp1()->accept_visitor(*this);
    rv |= formula.getOp2()->accept_visitor(*this);

    return rv;
}

int LogicNotNormalizationVisitor::visitUnaryBooleanOperation(
        UnaryBooleanFormula &formula)
{
    int rv = formula.getOp1()->accept_visitor(*this);
    auto op1 = formula.getOp1();

    formula.setOp1(_analyzeFormula(op1));

    rv |= formula.getOp1()->accept_visitor(*this);
    return rv;
}

int LogicNotNormalizationVisitor::visitLargeBooleanFormula(
        LargeBooleanFormula &formula)
{
    int rv = 0;
    for(size_t i = 0; i < formula.operands.size(); ++i)
    {
        rv |= formula.operands[i]->accept_visitor(*this);
        auto op = formula.operands[i];
        formula.operands[i] = _analyzeFormula(op);
        rv |= formula.operands[i]->accept_visitor(*this);
    }
    return rv;
}

int LogicNotNormalizationVisitor::visitUnaryTemporalOperation(
        UnaryTemporalFormula &formula)
{
    int rv =formula.getFormula()->accept_visitor(*this);
    auto op1 = formula.getFormula();
    formula.setFormula(_analyzeFormula(op1));
    rv |= formula.getFormula()->accept_visitor(*this);

    return rv;
}

int LogicNotNormalizationVisitor::visitBinaryTemporalOperation(
        BinaryTemporalFormula &formula)
{
    int rv = formula.getFormula1()->accept_visitor(*this);
    rv |= formula.getFormula2()->accept_visitor(*this);

    auto op1 = formula.getFormula1();
    formula.setFormula1(_analyzeFormula(op1));
    auto op2 = formula.getFormula2();
    formula.setFormula2(_analyzeFormula(op2));

    rv |= formula.getFormula1()->accept_visitor(*this);
    rv |= formula.getFormula2()->accept_visitor(*this);

    return rv;
}

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
        }
    }
    return formula;
}
