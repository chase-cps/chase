/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/27/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/utilities/GroupTemporalOperatorsVisitor.hh"
#include "representation.hh"

#include "utilities/Factory.hh"

using namespace chase;

GroupTemporalOperatorsVisitor::GroupTemporalOperatorsVisitor() :
    LogicSimplificationVisitor()
{
}

GroupTemporalOperatorsVisitor::~GroupTemporalOperatorsVisitor() = default;

LogicFormula *
GroupTemporalOperatorsVisitor::_analyzeFormula(LogicFormula *formula)
{
    if(formula->IsA() == binaryBooleanOperation_node)
    {
        auto outer_bin = reinterpret_cast<BinaryBooleanFormula*>(formula);
        if(outer_bin->getOp1()->IsA() == unaryTemporalOperation_node &&
            outer_bin->getOp2()->IsA() == unaryTemporalOperation_node)
        {
            auto op1 =
                    reinterpret_cast<UnaryTemporalFormula*>(
                            outer_bin->getOp1());
            auto op2 =
                    reinterpret_cast<UnaryTemporalFormula*>(
                            outer_bin->getOp2());

            if( op1->getOp() == op2->getOp() )
            {
                auto outer_op = outer_bin->getOp();
                auto inner_op = op1->getOp();

                if( outer_op == op_and &&
                    (inner_op == op_globally || inner_op == op_next))
                {
                    return new UnaryTemporalFormula(inner_op,
                            And(op1->getFormula(), op2->getFormula()));
                }
                if( outer_op == op_or &&
                    (inner_op == op_future || inner_op == op_next))
                {
                    return new UnaryTemporalFormula(inner_op,
                            Or( op1->getFormula(), op2->getFormula() ));
                }
            }
        }
    }
    return formula;
}






