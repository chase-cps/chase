/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/16/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "backends/NuSMVPrinter.hh"

using namespace chase;

int NuSMVPrinter::visitBooleanValue(BooleanValue &value) {
    if(value.getValue() == true)
        _fout << "TRUE";
    else
        _fout << "FALSE";
    return GuideVisitor::visitBooleanValue(value);
}

int NuSMVPrinter::visitIdentifier(Identifier &identifier) {
    std::string name = identifier.getDeclaration()->getName()->getString();
    if (!identifier.isPrimed()) _fout << identifier.getString();
    else {
        _fout << "X(" << name << ")";
    }
    return GuideVisitor::visitIdentifier(identifier);
}

int NuSMVPrinter::visitVariable(Variable &variable) {
    auto type = variable.getType();
    auto name = variable.getName()->getString();
    /// \todo Add support for integers.
    if (!(type->IsA() == boolean_node || type->IsA() == integer_node))
        messageError(
                "Trying to use a non supported type variable in NuSMV. " + name);

    if (type->IsA() == boolean_node) {
        std::string stype("boolean");
        _fout << "\t" << name << " : " << stype << ";" << std::endl;
    }
    if (type->IsA() == integer_node)
    {
        _fout << "\t" << name << " : ";
        auto i = reinterpret_cast< Integer * >(type);
        _fout << i->getRange()->getLeftValue() << ".."
            << i->getRange()->getRightValue() << ";" << std::endl;
    }

    return GuideVisitor::visitVariable(variable);
}

int NuSMVPrinter::visitProposition(Proposition &proposition)
{
    return GuideVisitor::visitProposition(proposition);
}

int NuSMVPrinter::visitBinaryBooleanOperation(BinaryBooleanFormula &formula)
{
    std::string op = _printBooleanOperator(formula.getOp());
    _fout << "(";
    int rv = formula.getOp1()->accept_visitor(*this);
    _fout << " " << op << " ";
    rv |= formula.getOp2()->accept_visitor(*this);
    _fout << ")";
    return rv;
}

int NuSMVPrinter::visitUnaryBooleanOperation(UnaryBooleanFormula &formula) {
    std::string op = _printBooleanOperator(formula.getOp());
    _fout << "(";
    _fout << " " << op << " ";
    int rv = formula.getOp1()->accept_visitor(*this);
    _fout << ")";
    return rv;
}

int NuSMVPrinter::visitLargeBooleanFormula(LargeBooleanFormula &formula) {
    std::string op( _printBooleanOperator(formula.getOp()));
    auto i = formula.operands.begin();
    if( i == formula.operands.end() )
        messageError("Empty large boolean formula. Invalid formula.");

    int rv = (*i)->accept_visitor(*this);
    ++i;
    for(; i != formula.operands.end(); ++i)
    {
        _fout << " " << op << " ";
        rv |= (*i)->accept_visitor(*this);
    }
    return rv;
}

int NuSMVPrinter::visitUnaryTemporalOperation(UnaryTemporalFormula &formula) {
    std::string op = _printTemporalOperator(formula.getOp());
    if( formula.getOp() == op_globally)
        _fout << "\n\t\t";
    _fout << "(";
    _fout << " " << op << " ";
    int rv = formula.getFormula()->accept_visitor(*this);
    _fout << ")";
    return rv;
}

int NuSMVPrinter::visitBinaryTemporalOperation(BinaryTemporalFormula &formula) {
    std::string op = _printTemporalOperator(formula.getOp());
    _fout << "(";
    int rv = formula.getFormula1()->accept_visitor(*this);
    _fout << " " << op << " ";
    rv |= formula.getFormula2()->accept_visitor(*this);
    _fout << ")";
    return rv;
}

int NuSMVPrinter::visitExpression(Expression &expression) {
    auto op = expression.getOperator();
    std::string sop;
    switch(op)
    {
        case op_divide:
            sop = "/";
            break;
        case op_plus:
            sop = "+";
            break;
        case op_minus:
            sop = "-";
            break;
        case op_multiply:
            sop = "*";
            break;
        case op_mod:
            sop = "%";
            break;
        case op_eq:
            sop = "=";
            break;
        case op_neq:
            sop = "!=";
            break;
        case op_lt:
            sop = "<";
            break;
        case op_gt:
            sop = ">";
            break;
        case op_le:
            sop = "<=";
            break;
        case op_ge:
            sop = ">=";
            break;
        case op_none:
        default:
            messageWarning("Missing operator in expression: "
                + expression.getString());
            break;
    }
    _fout << "( ";
    int ret = expression.getOp1()->accept_visitor(*this);
    _fout << " " << sop << " ";
    ret |= expression.getOp2()->accept_visitor(*this);
    _fout << " )";
    return ret;
}

int NuSMVPrinter::visitIntegerValue(IntegerValue &value) {
    _fout << value.getString();
    return 1;
}
