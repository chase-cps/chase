/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/6/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/LargeBooleanFormula.hh"

using namespace chase;

LargeBooleanFormula::LargeBooleanFormula(BooleanOperator op ) :
    _op(op)
{
    _node_type = large_boolean_formula_node;
    bool valid = false;
    switch( _op )
    {
        case op_and:
        case op_or:
        case op_xor:
        case op_nand:
        case op_nor:
            valid = true;
            break;
        default:
            valid = false;
            break;
    }

    if(!valid)
    {
        std::string msg("Inconsistent big operator: ");
        msg += to_string(op);
        messageError(msg.c_str());
    }
}

LargeBooleanFormula::~LargeBooleanFormula() = default;

BooleanOperator LargeBooleanFormula::getOp() const {
    return _op;
}

void LargeBooleanFormula::setOp(BooleanOperator op) {
    _op = op;

    bool valid = false;
    switch( _op )
    {
        case op_and:
        case op_or:
        case op_xor:
        case op_nand:
        case op_nor:
            valid = true;
            break;
        default:
            valid = false;
            break;
    }

    if(!valid)
    {
        std::string msg("Inconsistent big operator: ");
        msg += to_string(op);
        messageError(msg.c_str());
    }
}

int LargeBooleanFormula::accept_visitor(chase::BaseVisitor &v)
{
    return v.visitLargeBooleanFormula(*this);
}

std::string LargeBooleanFormula::getString()
{
    std::string ret("AND(\n");
    for(size_t i = 0; i < operands.size(); ++i){
        ret += "\t";
        ret += operands[i]->getString();
        if( i == operands.size() -1 ) ret += "\n";
        else ret += ",\n";
    }
    ret += ")\n";
    return ret;
}

LargeBooleanFormula *LargeBooleanFormula::clone()
{
    auto ret = new LargeBooleanFormula( _op );
    for(auto & operand : operands) {
        ret->addOperand(operand->clone());
    }

    return ret;
}

void LargeBooleanFormula::addOperand(LogicFormula *f)
{
    operands.push_back(f);
    f->setParent(this);
}
