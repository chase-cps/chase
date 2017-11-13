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
 * @file	backend/include/Chase2TuLiP/GR1/InitialConditionsFinder.hh
 * @brief
 */



#ifndef INITIAL_CONDITIONS_FINDER_HH
#define INITIAL_CONDITIONS_FINDER_HH

#include "Manipulation/GuideVisitor.hh"
#include "Behaviors/Logics/Logics.hh"

namespace chase { namespace backend
    {

        namespace GR1
        {
            class InitialConditionsFinder : public Manipulation::GuideVisitor
            {
                public:
                    InitialConditionsFinder();
                    ~InitialConditionsFinder();

                    int visitTemporalFormula( 
                            Behaviors::Logics::TemporalFormula & o );

                    int visitUnaryTemporalFormula(
                            Behaviors::Logics::UnaryTemporalFormula & o );

                    int visitBinaryTemporalFormula(
                            Behaviors::Logics::BinaryTemporalFormula & o );

                    int visitUnaryLogicCombination(
                            Behaviors::Logics::UnaryLogicCombination & o );

                    int visitLogicProposition(
                            Behaviors::Logics::LogicProposition & o );


                    std::list< 
                        Behaviors::Logics::WellFormedFormula * > initial_conditions;

                protected:

                    bool _negated;

                    InitialConditionsFinder( const InitialConditionsFinder & );
                    InitialConditionsFinder & operator=(
                            const InitialConditionsFinder & );
            };
        }
    }
}

#endif // INITIAL_CONDITIONS_FINDER_HH
