/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/27/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/utilities/GroupTemporalOperatorsVisitor.hh"
#include "representation.hh"

#include "utilities/Factory.hh"
#include "utilities/simplify.hh"

using namespace chase;

GroupTemporalOperatorsVisitor::GroupTemporalOperatorsVisitor() :
    LogicSimplificationVisitor()
{
}

GroupTemporalOperatorsVisitor::~GroupTemporalOperatorsVisitor() = default;

LogicFormula *
GroupTemporalOperatorsVisitor::_analyzeFormula(LogicFormula *formula)
{
    LogicFormula * ret = formula;
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
                    ret = new UnaryTemporalFormula(inner_op,
                            And(op1->getFormula(), op2->getFormula()));
                }
                if( outer_op == op_or &&
                    (inner_op == op_future || inner_op == op_next))
                {
                    ret = new UnaryTemporalFormula(inner_op,
                            Or( op1->getFormula(), op2->getFormula() ));
                }
            }
        }
    }
    if(formula->IsA() == large_boolean_formula_node)
    {
        auto lbf = reinterpret_cast< LargeBooleanFormula*>(formula);

        std::vector< LogicFormula * > _always;
        std::vector< LogicFormula * > _eventually;
        std::vector< LogicFormula * > _untimed;
        for(size_t i = 0; i < lbf->operands.size(); ++i)
        {
            if( lbf->operands[i]->IsA() == unaryTemporalOperation_node )
            {
                auto f = reinterpret_cast< UnaryTemporalFormula* >(
                                lbf->operands[i]);
                if(f->getOp() == op_globally)
                    _always.push_back(lbf->operands[i]);
                if(f->getOp() == op_future)
                    _eventually.push_back(lbf->operands[i]);
            }
            else
            {
                _untimed.push_back(lbf->operands[i]);
            }
        }

        //auto lop = lbf->getOp();

    }
    return ret;
}






