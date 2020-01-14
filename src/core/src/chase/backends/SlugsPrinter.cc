/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/3/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "backends/SlugsPrinter.hh"

using namespace chase;

SlugsPrinter::SlugsPrinter() :
    GuideVisitor()
{
}

SlugsPrinter::~SlugsPrinter()
{
    if(fout.is_open())
        fout.close();
}

void SlugsPrinter::print(Contract *contract, std::string path)
{
    // Open the file and get the contract to be printed.
    _contract = contract;
    fout.open(path);

    // Print the declarations.
    _printDeclarations();

    _printInit();
    _printSafety();
    _printLiveness();

}



void SlugsPrinter::_printDeclarations()
{
    // Print the environment variables.
    fout << "[INPUT]" << std::endl;
    for( auto vit = _contract->declarations.begin();
        vit != _contract->declarations.end(); ++vit)
    {
        if( (*vit)->IsA() != variable_node ) continue;
        auto var = reinterpret_cast<Variable *>(*vit);
        if( var->getCausality() == input )
        {
            std::string name = var->getName()->getString();
            Type * type = var->getType();
            fout << name;
            if(type->IsA() == integer_node)
            {
                auto integ = reinterpret_cast<Integer *>(type);
                auto lv = integ->getRange()->getLeftValue();
                auto rv = integ->getRange()->getRightValue();

                if(lv->IsA() != integerValue_node ||
                    rv->IsA() != integerValue_node)
                    messageError("Non integer range.");

                auto ilv = reinterpret_cast<IntegerValue *>(lv);
                auto irv = reinterpret_cast<IntegerValue *>(rv);

                fout << ": " << ilv->getValue()
                    << "..." << irv->getValue();
            }
            fout << std::endl;
        }
    }
    fout << std::endl << std::endl;


    // Print the system variables.
    fout << "[OUTPUT]" << std::endl;
    for( auto vit = _contract->declarations.begin();
         vit != _contract->declarations.end(); ++vit)
    {
        if( (*vit)->IsA() != variable_node ) continue;
        auto var = reinterpret_cast<Variable *>(*vit);
        if( var->getCausality() == output )
        {
            std::string name = var->getName()->getString();
            Type * type = var->getType();
            fout << name;
            if(type->IsA() == integer_node)
            {
                auto integ = reinterpret_cast<Integer *>(type);
                auto lv = integ->getRange()->getLeftValue();
                auto rv = integ->getRange()->getRightValue();

                if(lv->IsA() != integerValue_node ||
                   rv->IsA() != integerValue_node)
                    messageError("Non integer range.");

                auto ilv = reinterpret_cast<IntegerValue *>(lv);
                auto irv = reinterpret_cast<IntegerValue *>(rv);

                fout << ": " << ilv->getValue()
                     << "..." << irv->getValue();
            }
            fout << std::endl;
        }
    }

    fout << std::endl << std::endl;

}



void SlugsPrinter::_printInit() {
    // Assumptions.
    auto formulae = _contract->assumptions.find(temporal_logic);
    if( formulae == _contract->assumptions.end() )
        messageError("No Logic in Assumptions");

    auto assumptions = formulae->second;

    if( assumptions->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(assumptions);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");


        _curr = std::string("");
        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {

            if( lbf->operands[f]->IsA() != unaryTemporalOperation_node &&
                    lbf->operands[f]->IsA() != binaryTemporalOperation_node )
            {
                _curr += "\n";

                lbf->operands[f]->accept_visitor(*this);

            }
        }
        if(_curr != "")
            fout<< "[ENV_INIT]" << _curr
                << std::endl << std::endl;
    }

    // Guarantees
    formulae = _contract->guarantees.find(temporal_logic);
    if( formulae == _contract->guarantees.end() )
        messageError("No Logic in Assumptions");

    auto guarantees = formulae->second;

    if( guarantees->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(guarantees);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");

        _curr = std::string("");

        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {
            if( lbf->operands[f]->IsA() != unaryTemporalOperation_node &&
                lbf->operands[f]->IsA() != binaryTemporalOperation_node )
            {
                _curr += "\n";
                lbf->operands[f]->accept_visitor(*this);
            }

        }
        if(_curr != "")
            fout<< "[SYS_INIT]" << _curr
                << std::endl << std::endl;
    }
}

void SlugsPrinter::_printSafety() {
    // Assumptions.
    auto formulae = _contract->assumptions.find(temporal_logic);
    if( formulae == _contract->assumptions.end() )
        messageError("No Logic in Assumptions");

    auto assumptions = formulae->second;

    if( assumptions->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(assumptions);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");

        _curr = std::string("");

        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {
            if( lbf->operands[f]->IsA() == unaryTemporalOperation_node )
            {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula*>(lbf->operands[f]);
                if(uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if(uto->getFormula()->IsA() == unaryTemporalOperation_node)
                {
                    auto inner = reinterpret_cast<UnaryTemporalFormula*>(
                            uto->getFormula());
                    if( inner->getOp() == op_future )
                        continue; // Is a Liveness Property.
                }
                _curr += "\n";
                uto->accept_visitor(*this);
            }
        }

        if(_curr != "")
            fout<< "[ENV_TRANS]"
                << _curr << std::endl << std::endl;
    }

    // Guarantees
    formulae = _contract->guarantees.find(temporal_logic);
    if( formulae == _contract->guarantees.end() )
        messageError("No Logic in Assumptions");

    auto guarantees = formulae->second;

    if( guarantees->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(guarantees);
        if (lbf->getOp() != op_and)
            messageError("Not a GR1 Specification.");

        _curr = std::string("");
        for (size_t f = 0; f < lbf->operands.size(); ++f) {
            if (lbf->operands[f]->IsA() == unaryTemporalOperation_node) {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula *>(lbf->operands[f]);
                if (uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if (uto->getFormula()->IsA() == unaryTemporalOperation_node) {
                    auto inner = reinterpret_cast<UnaryTemporalFormula *>(
                            uto->getFormula());
                    if (inner->getOp() == op_future)
                        continue; // Is a Liveness Property.
                }
                _curr += "\n";
                uto->accept_visitor(*this);
            }
        }
        if (_curr != "")
            fout << "[SYS_TRANS]"
                 << _curr << std::endl << std::endl;
    }
}

void SlugsPrinter::_printLiveness() {
    // Assumptions.
    auto formulae = _contract->assumptions.find(temporal_logic);
    if( formulae == _contract->assumptions.end() )
        messageError("No Logic in Assumptions");

    auto assumptions = formulae->second;

    if( assumptions->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(assumptions);
        if (lbf->getOp() != op_and)
            messageError("Not a GR1 Specification.");

        _curr = std::string("");

        for (size_t f = 0; f < lbf->operands.size(); ++f) {
            if (lbf->operands[f]->IsA() == unaryTemporalOperation_node) {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula *>(lbf->operands[f]);
                if (uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if (uto->getFormula()->IsA() == unaryTemporalOperation_node) {
                    auto inner = reinterpret_cast<UnaryTemporalFormula *>(
                            uto->getFormula());
                    if (inner->getOp() == op_future) {
                        _curr += "\n";
                        uto->accept_visitor(*this);
                    }
                }
            }
        }
        if (_curr != "")
            fout << "[ENV_LIVENESS]"
                 << _curr << std::endl << std::endl;
    }

    // Guarantees
    formulae = _contract->guarantees.find(temporal_logic);
    if( formulae == _contract->guarantees.end() )
        messageError("No Logic in Assumptions");

    auto guarantees = formulae->second;

    if( guarantees->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(guarantees);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");


        _curr = std::string("");

        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {
            if( lbf->operands[f]->IsA() == unaryTemporalOperation_node )
            {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula*>(lbf->operands[f]);
                if(uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if(uto->getFormula()->IsA() == unaryTemporalOperation_node)
                {
                    auto inner = reinterpret_cast<UnaryTemporalFormula*>(
                            uto->getFormula());
                    if( inner->getOp() == op_future ) {
                        _curr += "\n\t";
                        uto->accept_visitor(*this);
                    }
                }

            }
        }
        if (_curr != "")
            fout << "[SYS_LIVENESS]"
                  << _curr << std::endl << std::endl;
    }
}
