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
 * @file	backend/include/Chase2TuLiP/GR1/GR1Builder.hh
 * @brief
 */



#ifndef GR1_BUILDER_HH
#define GR1_BUILDER_HH

#include "GR1/InitialConditionsFinder.hh"
#include "GR1/SafetyFinder.hh"
#include "GR1/ProgressFinder.hh"

namespace chase{ namespace backend
    {
        namespace GR1
        {
            class GR1Builder
            {
                public:
                    GR1Builder( Behaviors::Logics::WellFormedFormula * formula );
                    ~GR1Builder();

                    Behaviors::Logics::GR1Formula * getGR1();

                protected:

                    Behaviors::Logics::WellFormedFormula * _formula;

                    GR1Builder( const GR1Builder & );
                    GR1Builder & operator=( const GR1Builder & );
            };
        }
    }
}
#endif // GR1_BUILDER_HH
