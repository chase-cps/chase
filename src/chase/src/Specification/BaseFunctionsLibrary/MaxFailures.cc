/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/src/Specification/BaseFunctionsLibrary/MaxFailures.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/MaxFailures.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

MaxFailures::MaxFailures() :
    RequirementFunction("max-failures",2)
{
}

MaxFailures::~MaxFailures()
{
}

void MaxFailures::resolve( Requirement * req, Problem * problem )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();

    // Retrieve the parameters used
    std::string par0 = req->getParameter(0);
    unsigned int par1 = atoi(req->getParameter(1).c_str());

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components =  
        Manipulation::findComponents( par0, problem->getNetwork() );


    std::set< Architecture::NetworkComponent * >::iterator sit;

    std::vector< Architecture::NetworkComponent * > nodes;

    for( sit = components.begin();
            sit != components.end();
            ++sit )
    {
        nodes.push_back((*sit));
    }
    std::list<
        std::vector< Architecture::NetworkComponent * > > results;
    std::vector< Architecture::NetworkComponent * > first;
    int safe_components = nodes.size() - par1;

    if( safe_components <= 0 ) return;

    getSubsetsBySize(nodes, nodes.size(), safe_components, 0, first, results);

    std::list< 
        std::vector< Architecture::NetworkComponent * > >::iterator lit;


    std::vector< WellFormedFormula * > formulae;

    for( lit = results.begin();
            lit != results.end();
            ++lit )
    {
        std::vector< Architecture::NetworkComponent * > currv = *lit;
        Behaviors::Variable * statevar = 
            problem->getStateVariable(currv[0]);
        WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                new LogicProposition(statevar->GetName()));

        for( size_t i = 1; i < currv.size(); ++i )
        {
            statevar = problem->getStateVariable(currv[i]);
            if( statevar != NULL )
            {
                wff = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            wff,
                            new LogicProposition(statevar->GetName() )));
            }
        }

        formulae.push_back(wff);


    }
    if( formulae.size() < 1 ) return;

    WellFormedFormula * assumption = dynamic_cast< WellFormedFormula * >(
            formulae[0] );

    for( size_t i = 1; i < formulae.size(); ++i )
    {
        assumption = dynamic_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_lor,
                    assumption,
                    formulae[i]));
    }

    WellFormedFormula * assumptions = dynamic_cast< WellFormedFormula * >(
            contract->assumptions );
    assert( assumptions );

    assumption = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_globally,
                assumption ));


    contract->assumptions = dynamic_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                assumptions,
                assumption ));

}







void MaxFailures::getSubsetsBySize(
        std::vector< Architecture::NetworkComponent * > nodes,
        int size,
        int left,
        int index,
        std::vector< 
        Architecture::NetworkComponent * >& combination,
        std::list< 
        std::vector< Architecture::NetworkComponent * > > &results)
{
    if( left == 0 )
    {
        results.push_back(combination);
        return;
    }
    for( int i = index; i < size; ++i )
    {
        combination.push_back(nodes[i]);
        getSubsetsBySize(nodes, size, left-1, i+1, combination, results);
        combination.pop_back();
    }
}


