/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        8/25/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "../../../include/chase/backends/PySTLPrinter.hh"
#include "representation.hh"

using namespace chase;

PySTLPrinter::PySTLPrinter( std::string path ) :
    GuideVisitor(),
    _contract(nullptr)
{
    _fout.open(path);
}

PySTLPrinter::~PySTLPrinter()
{
    if(_fout.is_open()) _fout.close();
}

void PySTLPrinter::print(Contract *contract)
{
    _contract = contract;

    _fout << "from pystl import *" << std::endl;
    _fout << "import numpy as np" << std::endl;

    printDeclarations();
    printAssumptions();
    printGuarantees();

    std::string name = _contract->getName()->getString();
    _fout << name << ".checkSat()" << std::endl;
    _fout << name << ".printInfo()" << std::endl;
    _fout << name << ".checkCompat(print_sol=True)" << std::endl;
    _fout << name << ".checkConsis(print_sol=True)" << std::endl;
    _fout << name << ".checkFeas(print_sol=True)" << std::endl;
}

void PySTLPrinter::printDeclarations()
{
    std::string name = _contract->getName()->getString();
    _fout <<
        name << "= contract('" << name << "')" << std::endl;

    for( auto i = _contract->declarations.begin();
            i != _contract->declarations.end(); ++i)
    {
        Declaration * decl = *i;
        if(decl->IsA() == variable_node)
        {
            auto var = reinterpret_cast< Variable * >(decl);
            causality_t caus = var->getCausality();
            if(caus == input)
            {
                _fout << name << ".set_deter_uncontrolled_vars(['"
                    << var->getName()->getString() << "'])" << std::endl;
            }
            if(caus == output)
            {
                _fout << name << ".set_controlled_vars(['"
                << var->getName()->getString() << "'])" << std::endl;
            }
        }
        if(decl->IsA() == constant_node)
        {
            messageWarning("Unmanaged constant for STL.");
        }
    }
}

void PySTLPrinter::printAssumptions()
{
    std::string name = _contract->getName()->getString();
    _fout << name << ".set_assume('(";

    auto assumptions = _contract->assumptions.find(logic);
    if(assumptions != _contract->assumptions.end())
    {
        auto spec = assumptions->second;
        if(spec->IsA() == large_boolean_formula_node)
            _printLargeFormula(reinterpret_cast<LargeBooleanFormula*>(spec));
        else
            _fout << spec->getString();
    }
    _fout << ")')" << std::endl;
}

void PySTLPrinter::printGuarantees()
{
    std::string name = _contract->getName()->getString();
    _fout << name << ".set_guaran('(";

    auto guarantees = _contract->guarantees.find(logic);
    if(guarantees != _contract->guarantees.end())
    {
        auto spec = guarantees->second;
        if(spec->IsA() == large_boolean_formula_node)
            _printLargeFormula(reinterpret_cast<LargeBooleanFormula*>(spec));
        else
            _fout << spec->getString();
    }
    _fout << ")')" << std::endl;
}

void PySTLPrinter::_printLargeFormula(LargeBooleanFormula *lbf)
{
    BooleanOperator op = lbf->getOp();
    std::string ret;
    ret += "(" + lbf->operands[0]->getString() + ")";
    for(size_t i = 1; i < lbf->operands.size(); ++i)
    {
        if(op == op_and) ret += " & ";
        else if(op == op_or) ret += " | ";
        ret +=  "(" + lbf->operands[0]->getString() + ")";
    }
    _fout << ret;
}
