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
 * @file	backend/src/Chase2TuLiP/GR1/GR1Builder.cc
 * @brief
 */



#include "GR1/GR1Builder.hh"
#include <iostream>

using namespace chase;
using namespace backend;
using namespace GR1;
using namespace Behaviors;
using namespace Behaviors::Logics;

GR1Builder::GR1Builder( WellFormedFormula * formula ) :
    _formula(formula)
{
}

GR1Builder::~GR1Builder()
{
}

GR1Formula * GR1Builder::getGR1()
{
    GR1Formula * gr1 = new GR1Formula( _formula );

    InitialConditionsFinder icf;
    _formula->accept_visitor( icf );

    SafetyFinder invf;
    _formula->accept_visitor( invf );

    ProgressFinder sf;
    _formula->accept_visitor( sf );


    // Create formulas


    //
    // Start with initial conditions
    //

    WellFormedFormula * initial_conditions = NULL;
    if( ! icf.initial_conditions.empty() )
    {
        initial_conditions = icf.initial_conditions.front();

        if( icf.initial_conditions.size() > 1 )
        {
            std::list< 
                WellFormedFormula * >::iterator ic_it = 
                icf.initial_conditions.begin();
            for( ++ic_it; ic_it != icf.initial_conditions.end(); ++ic_it )
            {
                WellFormedFormula * curr = *ic_it;
                initial_conditions = new BinaryLogicCombination(
                        op_land,
                        initial_conditions,
                        curr );
            }   
        }
    }

    //
    // Then the Safety
    //

    WellFormedFormula * safety = NULL;
    if( ! invf.safety.empty() )
    {
        safety = static_cast<WellFormedFormula * >(invf.safety.front());
        safety = new UnaryTemporalFormula( op_globally, safety );

        if( invf.safety.size() > 1 )
        {
            std::list< 
                WellFormedFormula * >::iterator inv_it = 
                invf.safety.begin();
            for( ++inv_it; inv_it != invf.safety.end(); ++inv_it )
            {
                WellFormedFormula * curr = *inv_it;
                curr = new UnaryTemporalFormula(op_globally, curr);
                safety = new BinaryLogicCombination(
                        op_land,
                        safety,
                        curr );
            }   
        }
    }

    //
    // Finally the progress
    //

    WellFormedFormula * progress = NULL;
    if( ! sf.progress.empty() )
    {
        progress = static_cast< WellFormedFormula * >(sf.progress.front());
        progress = new UnaryTemporalFormula( op_future, progress );
        progress = new UnaryTemporalFormula( op_globally, progress );


        if( sf.progress.size() > 1 )
        {
            std::list< 
                WellFormedFormula * >::iterator s_it = 
                sf.progress.begin();

            for( ++s_it; s_it != sf.progress.end(); ++s_it )
            {
                WellFormedFormula * curr = *s_it;
                curr = new UnaryTemporalFormula( op_future, curr );
                curr = new UnaryTemporalFormula( op_globally, curr );
                progress = new BinaryLogicCombination(
                        op_land,
                        progress,
                        curr );
            }   
        }
    }

    gr1->setProgress(progress);
    gr1->setSafety(safety);
    gr1->setInitial(initial_conditions);


    return gr1;

}


