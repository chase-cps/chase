/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        January 17, 2018
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Specification/BaseFunctionsLibrary/MaxFailures.hh
 * @brief
 */

#include "Specification/BaseFunctionsLibrary/MaxFailuresPerStep.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

MaxFailuresPerStep::MaxFailuresPerStep() :
    RequirementFunction("max-failures-per-step",2)
{
}

MaxFailuresPerStep::~MaxFailuresPerStep()
{
}

void MaxFailuresPerStep::resolve( Requirement * req, Problem * problem )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();
    WellFormedFormula * assumptions = dynamic_cast< WellFormedFormula * >(
            contract->assumptions );

    assert(assumptions);

    // Retrieve the parameters used
    std::string par0 = req->getParameter(0);
    unsigned int par1 = atoi(req->getParameter(1).c_str());

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components =  
        Manipulation::findComponents( par0, problem->getNetwork() );

    // Get the number of components to consider in the pattern.
    unsigned int number_of_components = components.size();


    // Declare a variable that counts the number of broken components in this
    // step.
    std::string broken_counter_name("broken_");
    broken_counter_name += par0;
    Variable * broken_counter = new Variable(
            broken_counter_name,
            Behaviors::t_int, 
            0.0, 
            (double) number_of_components + 2.0);
    /// \TODO Remove the  + 2.0: is due to some issues with the back-end.

    contract->environment_variables.push_back(broken_counter);

    // Update the counter.
    for( unsigned int n_curr_broken = 0; n_curr_broken < number_of_components; 
            ++n_curr_broken )
    {
        std::string curr_value(broken_counter->GetName());
        curr_value += "=";
        curr_value += std::to_string(n_curr_broken);

        LogicProposition * propFirst = new LogicProposition(curr_value);

        std::list< std::string > conditions_for_next;
        unsigned int max_in_next_step = n_curr_broken + par1;
        for( unsigned int i = n_curr_broken; i <= max_in_next_step; ++i )
        {
            std::string next_value(broken_counter->GetName());
            next_value += "=";
            next_value += std::to_string(i);
            conditions_for_next.push_back(next_value); 
        }


        std::list< std::string >::iterator lit = conditions_for_next.begin();
        WellFormedFormula * bodyNext = static_cast< WellFormedFormula * >(
                new LogicProposition( *lit ));
        ++lit;
        for( ; lit != conditions_for_next.end(); ++lit )
        {
            bodyNext = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_lor,
                        bodyNext,
                        new LogicProposition( *lit )));
        }

        WellFormedFormula * implies = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_implies,
                    propFirst,
                    new UnaryTemporalFormula(
                        op_next,
                        bodyNext )));

        WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula(
                    op_globally,
                    implies ));

        contract->assumptions = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    assumptions,
                    wff));

        assumptions = dynamic_cast< WellFormedFormula * >(
                contract->assumptions );

        assert(assumptions);

    }


    // First case - Everything working
    std::set< Architecture::NetworkComponent * >::iterator sit = 
        components.begin();

    // First component
    Architecture::NetworkComponent * comp = *sit;

    WellFormedFormula * all_working = static_cast< WellFormedFormula * >(
            new LogicProposition( problem->getStateVariable(comp)->GetName()));

    ++sit;
    for( ; sit != components.end(); ++sit )
    {
        Architecture::NetworkComponent * comp = *sit;

        all_working = static_cast< WellFormedFormula * >( 
                new BinaryLogicCombination(
                    op_land,
                    all_working,
                    new LogicProposition( 
                        problem->getStateVariable(comp)->GetName())));

    }

    std::string curr_value(broken_counter->GetName());
    curr_value += "=0";

    LogicProposition * broken_eq_zero = new LogicProposition(curr_value);


    all_working = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_globally,
                new BinaryLogicCombination(
                    op_implies,
                    all_working,
                    broken_eq_zero )
                )
            );

    contract->assumptions = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                assumptions,
                all_working )
            );

    assumptions = dynamic_cast< WellFormedFormula * >(
            contract->assumptions );
    assert(assumptions);

    // Second case - Everything NOT working

    // First component
    //    
    sit =  components.begin();
    comp = *sit;

    WellFormedFormula * none_working = static_cast< WellFormedFormula * >(
            new LogicProposition( 
                problem->getStateVariable(comp)->GetName()));

    none_working = static_cast< WellFormedFormula * >(
            new UnaryLogicCombination( op_lnot, none_working ));


    ++sit;
    for( ; sit != components.end(); ++sit )
    {
        Architecture::NetworkComponent * comp = *sit;

        none_working = static_cast< WellFormedFormula * >( 
                new BinaryLogicCombination(
                    op_land,
                    none_working,
                    new UnaryLogicCombination(
                        op_lnot,
                        new LogicProposition( 
                            problem->getStateVariable(comp)->GetName()))));

    }

    curr_value = broken_counter->GetName();
    curr_value += "=";
    curr_value += std::to_string(number_of_components);


    LogicProposition * broken_eq_all = new LogicProposition(curr_value);

    none_working = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_globally,
                new BinaryLogicCombination(
                    op_implies,
                    none_working,
                    broken_eq_all )
                )
            );

    contract->assumptions = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land,
                assumptions,
                none_working )
            );

    assumptions = dynamic_cast< WellFormedFormula * >(
            contract->assumptions );
    assert(assumptions);



    ///////////////////////////////////////////////////////////////////////////
    // Intermediate cases. ----------------------------------------------------
    // The messy part.

    for( unsigned int number_of_broken = 1;
            number_of_broken < number_of_components;
            ++number_of_broken )
    {

        std::list< std::vector< Architecture::NetworkComponent * > > results;
        std::vector< Architecture::NetworkComponent * > first;

        std::vector< Architecture::NetworkComponent * > nodes;

        for( sit = components.begin();
                sit != components.end();
                ++sit )
        {
            nodes.push_back((*sit));
        }


        getSubsetsBySize(
                nodes, 
                nodes.size(), 
                number_of_broken,
                0, 
                first,
                results );

        std::list<
            std::vector< Architecture::NetworkComponent * > >::iterator lvit;

        for( lvit = results.begin(); lvit != results.end(); ++lvit )
        {
            std::vector< Architecture::NetworkComponent * > currv = *lvit;
            std::set< Architecture::NetworkComponent * > currs;

            for(size_t i = 0; i < currv.size(); ++i )
            {
                currs.insert(currv[i]);
            }

            if( currs.size() == number_of_broken )
            {
                WellFormedFormula * curr_combination =
                    _getCombinationFormula( currs, components, problem );

                std::string broken_value(broken_counter->GetName());
                broken_value += "=";
                broken_value += std::to_string(number_of_broken);

                WellFormedFormula * implied = static_cast< WellFormedFormula * >(
                        new LogicProposition(broken_value));

                curr_combination = static_cast< WellFormedFormula * >(
                        new UnaryTemporalFormula(
                            op_globally,
                            new BinaryLogicCombination(
                                op_implies,
                                curr_combination,
                                implied )
                            )
                        );


                contract->assumptions = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            assumptions,
                            curr_combination )
                        );

                assumptions = dynamic_cast< WellFormedFormula * >(
                        contract->assumptions );
                assert(assumptions);
            }



        }


    }
}





void MaxFailuresPerStep::getSubsetsBySize(
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

WellFormedFormula * MaxFailuresPerStep::_getCombinationFormula(
        std::set< Architecture::NetworkComponent * >& chosen,
        std::set< Architecture::NetworkComponent * >& nodes,
        Problem * problem )
{

    WellFormedFormula * wff = static_cast< WellFormedFormula * >(
            new LogicConstant(lc_true));

    std::set< Architecture::NetworkComponent * >::iterator esit;
    for( esit = nodes.begin(); esit != nodes.end() ; ++esit )
    {
        Architecture::NetworkComponent * currcomp = *esit;
        std::string statevar(problem->getStateVariable(currcomp)->GetName());
        std::set< Architecture::NetworkComponent * >::iterator sit;
        sit = chosen.find(currcomp);
        if( sit != chosen.end() )
        {
            wff = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_land,
                        wff,
                        new UnaryLogicCombination(
                            op_lnot,
                            new LogicProposition(statevar)
                            )
                        )
                    );          
        }
        else
        {
            wff = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_land,
                        wff,
                        new LogicProposition(statevar)
                        )
                    );
        }
    }


    return wff;
}

