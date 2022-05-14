/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        5/10/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"
#include "utilities/ClonedDeclarationVisitor.hh"

using namespace chase;

void LogisticsSpecsBuilder::_createProjectionMapName(names_projection_map &m,
                                                     chase::Contract *c1,
                                                     chase::Contract *c2) {
    for(auto & dec1 : c1->declarations) {
        if(dec1->IsA() != variable_node) continue;
        for(auto & dec2 : c2->declarations ) {
            if (dec2->IsA() != variable_node) continue;

            std::string n1 = dec1->getName()->getString();
            std::string n2 = dec2->getName()->getString();
            if(n1 == n2) {
                m.insert(std::pair<std::string, std::string>(n1, n2));
            }
        }
    }
}

void LogisticsSpecsBuilder::_composeVariables(
        chase::Contract *composition,
        std::map< Declaration*, Declaration* >& cmap) const {
    std::map< std::string, chase::Declaration * > names;

    for(auto ci: warehouse->component2Contract) {
        auto contract = ci.second;
        for(auto d: contract->declarations) {
            std::string name(d->getName()->getString());
            auto it = names.find(name);
            if(it == names.end()){
                auto cloned = d->clone();
                std::pair< std::string, chase::Declaration* > p1(name, cloned);
                names.insert(p1);
                composition->addDeclaration(cloned);
                std::pair< Declaration *, Declaration * > p2(d, cloned);
            } else {
                auto cloned = it->second;
                std::pair< Declaration *, Declaration * > p2(d, cloned);
            }
        }
    }

}


Contract * LogisticsSpecsBuilder::_composeWarehouse() const {
    auto composition = new Contract("Warehouse");

    std::map< Declaration*, Declaration* > declaration_map;
    _composeVariables(composition, declaration_map);

    LargeBooleanFormula * assumptions = nullptr;
    LargeBooleanFormula * guarantees = nullptr;
    for(auto ci : warehouse->component2Contract){
        // Create assumptions and guarantees.
        auto it = ci.second->assumptions.find(constraints);
        auto curr_assumptions = it->second;
        if(assumptions == nullptr){
            assumptions = new LargeBooleanFormula();
        }

        auto lbf = reinterpret_cast< LargeBooleanFormula * >(curr_assumptions);
        if(lbf == nullptr) messageError("Something wrong building formulas.");

        for(auto ibf: lbf->operands)
            assumptions->addOperand(ibf->clone());
    }




    return composition;
}
