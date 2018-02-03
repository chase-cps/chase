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
 * @file	chase/include/Behaviors/Logics/GR1Formula.hh
 * @brief
 */



#ifndef GR1_FORMULA_HH
#define GR1_FORMULA_HH

#include "Behaviors/Behavior.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Manipulation/Manipulation.hh"

namespace Behaviors
{
    namespace Logics
    {
        class GR1Formula : public Behavior
        {
            public:

                GR1Formula( Behaviors::Logics::WellFormedFormula * f );
                ~GR1Formula();                

                WellFormedFormula * getOriginal();
                WellFormedFormula * getProgress();
                WellFormedFormula * getSafety();
                WellFormedFormula * getInitial();

                void setOriginal( WellFormedFormula * f );
                void setProgress( WellFormedFormula * f );
                void setSafety( WellFormedFormula * f );
                void setInitial( WellFormedFormula * f );

                int accept_visitor( Manipulation::BaseVisitor & v );
                std::string print();

            protected: 

                Behaviors::Logics::WellFormedFormula * _original;
                Behaviors::Logics::WellFormedFormula * _progress;
                Behaviors::Logics::WellFormedFormula * _safety;
                Behaviors::Logics::WellFormedFormula * _initial;

                GR1Formula( const GR1Formula & );
                GR1Formula & operator=( const GR1Formula & );

                void _createGR1Spec();


        };
    }
}

#endif // GR1_FORMULA_HH
