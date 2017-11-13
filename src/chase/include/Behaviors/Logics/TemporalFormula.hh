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
 * @file	chase/include/Behaviors/Logics/TemporalFormula.hh
 * @brief
 */



#ifndef TEMPORAL_FORMULA_HH
#define TEMPORAL_FORMULA_HH

#include "WellFormedFormula.hh"

namespace Behaviors {
    namespace Logics
    {
        class TemporalFormula : virtual 
                                public WellFormedFormula
        {
            public:

                TemporalFormula( temporal_operators o );
                ~TemporalFormula();

                void setOp( temporal_operators o );
                temporal_operators getOp();

                int accept_visitor( Manipulation::BaseVisitor & v );

            protected:

                temporal_operators _op;

               
        };
    }
}


#endif // TEMPORAL_FORMULA_HH
