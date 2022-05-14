/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        5/13/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "Z3Printer.hh"

using namespace chase;

Z3Printer::Z3Printer(
        Warehouse * warehouse, std::string path) :
    GuideVisitor(),
    _warehouse(warehouse)

{
    _fout.open(path);
    _fout << "from z3 import *" << std::endl;
    _printVariables();
    _printContracts();

    if(_fout.is_open()) _fout.close();
}

Z3Printer::~Z3Printer() {
}

void Z3Printer::_printVariables() {
    std::set< std::string > names;
    for(auto ci: _warehouse->component2Contract){
        auto contract = ci.second;
        for(auto decl : contract->declarations) {
            std::string name(decl->getName()->getString());
            auto found = names.find(name);
            if(found == names.end()){
                _fout << name << " = " << "Real(\'" << name << "\')" << std::endl;
                names.insert(name);
            }
        }
    }
    _fout << std::endl << std::endl;
}

void Z3Printer::_printContracts() {
    for(auto ci: _warehouse->component2Contract) {
        auto contract = ci.second;
        std::string name(contract->getName()->getString());
        auto ait = contract->assumptions.find(constraints);
        if(ait == contract->assumptions.end())
            _fout << name << "_assumptions = False" << std::endl << std::endl;
        else {
            _fout << name << "_assumptions = And(True";
            auto lbf = reinterpret_cast< LargeBooleanFormula* >(ait->second);
            if(lbf == nullptr) chase::messageError("Something wrong in formula building.");
            for(auto ibf : lbf->operands)
                _fout << "," << std::endl << "\t" << ibf->getString();
            _fout << std::endl << ")" << std::endl << std::endl;
        }
        auto git = contract->guarantees.find(constraints);
        if(git == contract->guarantees.end())
            _fout << name << "_guarantees = True" << std::endl << std::endl;
        else {
            _fout << name << "_guarantees = And(True";
            auto lbf = reinterpret_cast< LargeBooleanFormula* >(git->second);
            if(lbf == nullptr) chase::messageError("Something wrong in formula building.");
            for(auto ibf : lbf->operands)
                _fout << "," << std::endl << "\t" << ibf->getString();
            _fout << std::endl << ")" << std::endl << std::endl;
        }
    }

    std::string sys_assume("system_assumptions = And(True");
    std::string sys_guaran("system_guarantees = And(True");

    _fout << "# SATURATE CONTRACTS" <<std::endl << std::endl;
    for(auto ci : _warehouse->component2Contract) {
        auto contract = ci.second;
        std::string name(contract->getName()->getString());
        _fout << name << "_guarantees = Implies(" <<
            name << "_assumptions, " << name << "_guarantees)" << std::endl;

        sys_assume += (",\n\t" + name + "_assumptions");
        sys_guaran += (",\n\t" + name + "_guarantees");
    }
    sys_assume += ")";
    sys_guaran += ")";
    _fout << std::endl;
    _fout << "# COMPOSE CONTRACTS" << std::endl << std::endl;
    _fout << sys_assume << std::endl << std::endl;
    _fout << sys_guaran << std::endl;
    _fout <<
        "system_assumptions = Or(system_assumptions, Not(system_guarantees))"
        << std::endl;

    _fout <<
    "system_guarantees = Implies(system_assumptions, system_guarantees)"
    << std::endl;

    _fout  <<
    "print(\"A and G\")" << std::endl;
    _fout << "solve(And(system_assumptions, system_guarantees))" << std::endl;

}